#include "MM7150_T4.h"

volatile BOOL EC_DATA_AVAIL = FALSE, flag = FALSE;                                // EC data available for read flag
IntervalTimer slaveTimer;
//#include "system.h"
char _glob[40];
SF_VREGS _VREGS = {0};                                                     // structure of Sensor Fusion VREG registers

GET_SET_PARAMS SET_PARAMS = {0};                                          //structure of SetFeature parameters
extern SF_SENSOR SENSOR[NUM_SENS];                                  // structure of individual sensors
                                 // flag to indicate when EC has data available to be read

UINT16 usPREV_SHC_STATE = 0;     
//#include "interrups.h"

void IntPinHandler(){
    EC_DATA_AVAIL =  !digitalRead(PIN_INT);
}

void read_sensors(){
    UINT8 ucRet = FALSE;
    UINT8 ucRx_data[BUF_40];
    UINT8 ucSensorNum;
    UINT16 *usRegPtr;                                               // Pointer to individual register
    usRegPtr = (UINT16 *)&_VREGS.SHC;                                // Set the pointer to SHC (VREG00 register)

    while(EC_DATA_AVAIL)                                              //SSC7150 sensor has data available for us to read
    {   
        ucRet = i2c_cmd_WrRd (READ,                                  // Read the data from the SSC7150
                            0,                                      //num of cmd bytes
                            0,                                      //cmd buf (ignored)
                            BYTE_ADJ_VAL,                           //num of bytes to read
                            ucRx_data,                              //recv buf 
                            TRUE);                                  //actual # of bytes SSC7150 returns is in 1st two bytes of read packet, this flag(=TRUE) means "use the 1st two bytes as the actual read packet length"
        if (ucRet) return;
        EC_DATA_AVAIL =  !digitalRead(PIN_INT);
        for (ucSensorNum = 0; ucSensorNum < NUM_SENS; ucSensorNum++) //go through the sensor list to see where to put the received data 
        {              
            if (ucRx_data[2] == SENSOR[ucSensorNum].id)          //1st two bytes of read data packet hold size of packet, then data begins at byte 2
            {
                if (*usRegPtr & (1 << (ucSensorNum + VREG_SHC_ACC_EN_VAL))) // If the incoming data belongs to an enabled sensor (in SHC (VREGS00))
                {
                    parse_update_VREG_data(ucSensorNum, ucRx_data); //store data in appropriate VREG data registers
                    break;                                          //found correct enabled sensor, no reason to look further
                }
            }            
        }   
    }
}



void gets_I2C1(UINT8 *ucRdptr, UINT16 usLength, BOOL bAdjust) {

    UINT8 ucSize = 1;                                                // Set return value for size of data read for bAdjust=FALSE

    int i = 0;
    Wire.requestRG(SLAVE_ADDR, usLength, I2C_STOP, bAdjust);  // NON-blocking read (request 256 bytes)

    while(Wire.available()){
        ucRdptr[i++] = Wire.read();
    }

}

//#include "vregs.h"

                                       // UINT16 buffer to store previous SHC VREG config for comparison

//*****************************************************************************
//*****************************************************************************
//  Section : Code
//*****************************************************************************
//*****************************************************************************

/** VREG_init
* @note        Inits VREG registers, get HID config & report descriptor tables, retrieves all device features
* @param 
* @return error status 0=SUCCESS, failcodes: 0x11=HID_DESC_FAIL, 0x12=RPT_DESC_FAIL, 0x18=RESET_FAIL 
*/ 


UINT8 VREG_init() 
{
    UINT8 ucBuf[BUF_40];                                           
    UINT8 ucSensor_num;
    
        
    memset(&_VREGS, 0x00, sizeof(_VREGS));                            // Initialize VREG registers 

    if ( hid_i2c_descriptor_handler(GET_HID_DESC) )                 // get HID descriptor from SSC7150
    {
        _VREGS.stat.stat4.SHStartStatus = VREG_SHSTART_FAIL;         // update status register (VREG 0x3F) for failure to get HID descriptor
        return HID_DESC_FAIL;
    }        
    
    hid_i2c_cmd_process(ucBuf, POWER_ON, ARB_ID);                   // Issue HID Power ON command to SSC7150 (NOTE: 'ucBuf' and 'ARB_ID' are don't cares for POWER_ON command)
    _VREGS.SHC.reset = VREG_RESET_INIT;


    if ( hid_i2c_cmd_process (ucBuf, RESET_DEV_CMD, ARB_ID) )       // Issue HID Reset command  (NOTE: 'ucBuf' and 'ARB_ID' are don't cares for RESET_REG command)
    {
        _VREGS.stat.stat4.SHStartStatus = VREG_SHSTART_FAIL;         // If HID Reset fails, update status register (VREG 0x3F)
        return RESET_FAIL;
    }

    if ( hid_i2c_descriptor_handler(GET_RPT_DESC) )                 // Get HID Report descriptor from SSC7150 
    {
        _VREGS.stat.stat4.SHStartStatus = VREG_SHSTART_FAIL;         // HID report descriptor error, update status register (VREG 0x3F)
        return RPT_DESC_FAIL;
    }  
    
    for (ucSensor_num = 0; ucSensor_num <= NUM_SENS; ucSensor_num++) //go through the sensor list but only GetFeatureReport for valid ids   
    {
        if (SENSOR[ucSensor_num].id != 0xFF && SENSOR[ucSensor_num].id != 0)
        {
            hid_i2c_cmd_process(ucBuf, HID_GET_RPT_FEAT, SENSOR[ucSensor_num].id);  // Get feature report for all valid sensors and update _VREGS register

        }
    }


    _VREGS.stat.stat4.SHStartStatus = VREG_SHSTART_SUCCESS;          // HID startup successful, update status register (VREG 0x3F) 

    _VREGS.SHC.SHwake = TRUE;                                        // HID device is now awake and ready for operation
    _VREGS.SHC.SHstart = TRUE;

    return SUCCESS;
}


/** set_state_data
* @note        Set and confirm new features for a device 
* @param GET_SET_PARAMS structure containing  power state (=2: FULL, =3: LOW),sensor ID, New data rate,New sensitivity
* @return error status 0=SUCCESS, failcodes: 0x17=SET_FEAT_FAIL 
*/ 
UINT8 set_state_data(GET_SET_PARAMS SET_PARAMS)    
{
    UINT8 ucGetFeatBuf[BUF_40];                                     // GetFeature report buffer
    UINT8 ucSetFeatBuf[BUF_40];                                     // SetFeature report buffer
    UINT8 ucRetryCnt, ucGSptr, ucBufSize;


    if ( hid_i2c_cmd_process(ucGetFeatBuf, HID_GET_RPT_FEAT, SET_PARAMS.ucid) ) // Issue a get report feature command and store the get features in ucGetFeatBuf
        return SET_FEAT_FAIL;  
    
    memcpy(&ucSetFeatBuf, &ucGetFeatBuf, BUF_40);                   // Copy GetFeatBuff to bufs to use for set feature command

    // Modify fields that we would like to set in bufs (sensor state, power state, data rate, sensitivity)
    ucSetFeatBuf[RPT_REPORT_STATE] = HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_ENUM;   
    ucSetFeatBuf[RPT_PWR_STATE] = SET_PARAMS.ucPowerState;          //update the Power state parameter

    if (SET_PARAMS.bDataRateValid)                                  // update data rate value?
    {
        ucSetFeatBuf[RPT_REPORT_INTVAL_LSB] = (UINT8) SET_PARAMS.usDataRateVal; //update 16 bit data rate value
        ucSetFeatBuf[RPT_REPORT_INTVAL_MSB] = (UINT8) (SET_PARAMS.usDataRateVal >> BYTE_SHIFT);
    }
    
    if (SET_PARAMS.bSensitivityValid)                               // update sensitivity value?
    {
        ucSetFeatBuf[RPT_CHG_SENS_LSB] = (UINT8) SET_PARAMS.usSensitivityVal; //update 16 bit sensitivity value
        ucSetFeatBuf[RPT_CHG_SENS_MSB] = (UINT8) (SET_PARAMS.usSensitivityVal >> BYTE_SHIFT);
    }

    for (ucRetryCnt = 0; ucRetryCnt < MAX_RETRIES; ucRetryCnt++)    // API spec requires 3 attempts at setting new features. 
    {
        hid_i2c_cmd_process(ucSetFeatBuf, HID_SET_RPT_FEAT, SET_PARAMS.ucid);  // Send HID_SetFeature command to SSC7150 to update new features to the device
        hid_i2c_cmd_process(ucGetFeatBuf, HID_GET_RPT_FEAT, SET_PARAMS.ucid);  // Send HID_GetFeature command to SSC7150 to check if new feature request to device were updated
        
        ucBufSize = ucGetFeatBuf[RPT_SIZE_LSB];                     // size (in bytes) of packet is in 1st byte
        for (ucGSptr = 0; ucGSptr < ucBufSize; ucGSptr++)           // Check to see if the features have been updated
        {
            if ( ucGetFeatBuf[ucGSptr] != ucSetFeatBuf[ucGSptr] ) 
                break;                                              //no they weren't, try again
        }

        if ( ucGSptr == ucBufSize )                                 //that's the whole packet, ALL data matches
            break;                                                  //we're done
    }

    if (ucRetryCnt == MAX_RETRIES)
        return SET_FEAT_FAIL;                                       // Return error code
    
    else return SUCCESS;                                            // Return successful
}

/** parse_update_VREG_data
* @note        Parse input report, update data _VREGS 
* @param ucSensorNum Sensor data struct checked for unit exponent value
* @param ucInpRep Pointer to input report data buffer
* @return 
*/ 
void parse_update_VREG_data (UINT8 ucSensorNum, UINT8 *ucInpRep)
{

    switch (ucSensorNum)                                            //update VREG data registers with their respective data.
    {     
        case ACCEL_VREG_OFFSET:                                     // Accelerometer data has been read
 
            _VREGS._data.ACXD = ((ucInpRep[SENSOR[ACCEL_VREG_OFFSET].DatOffset + 1] << BYTE_SHIFT) | ucInpRep[SENSOR[ACCEL_VREG_OFFSET].DatOffset]);    // parse data from data offset found in report descriptor
            _VREGS._data.ACYD = ((ucInpRep[SENSOR[ACCEL_VREG_OFFSET].DatOffset + 3] << BYTE_SHIFT) | ucInpRep[SENSOR[ACCEL_VREG_OFFSET].DatOffset + 2]);
            _VREGS._data.ACZD = ((ucInpRep[SENSOR[ACCEL_VREG_OFFSET].DatOffset + 5] << BYTE_SHIFT) | ucInpRep[SENSOR[ACCEL_VREG_OFFSET].DatOffset + 4]);
            break;

        case GYRO_VREG_OFFSET:                                      // Gyrometer data has been read
 
            _VREGS._data.GYXD = ((ucInpRep[SENSOR[GYRO_VREG_OFFSET].DatOffset + 1] << BYTE_SHIFT) | ucInpRep[SENSOR[GYRO_VREG_OFFSET].DatOffset]);
            _VREGS._data.GYYD = ((ucInpRep[SENSOR[GYRO_VREG_OFFSET].DatOffset + 3] << BYTE_SHIFT) | ucInpRep[SENSOR[GYRO_VREG_OFFSET].DatOffset + 2]);
            _VREGS._data.GYZD = ((ucInpRep[SENSOR[GYRO_VREG_OFFSET].DatOffset + 5] << BYTE_SHIFT) | ucInpRep[SENSOR[GYRO_VREG_OFFSET].DatOffset + 4]);
            break;

        case CMP_VREG_OFFSET:                                       // Compass data has been read
  
            _VREGS._data.CMD = ((ucInpRep[SENSOR[CMP_VREG_OFFSET].DatOffset + 1] << BYTE_SHIFT) | ucInpRep[SENSOR[CMP_VREG_OFFSET].DatOffset]);
            break;
        
        case ORI_VREG_OFFSET:                                       // Orientation data has been read

            _VREGS._data.ORXD = ((ucInpRep[SENSOR[ORI_VREG_OFFSET].DatOffset + 1] << BYTE_SHIFT) | ucInpRep[SENSOR[ORI_VREG_OFFSET].DatOffset]);
            _VREGS._data.ORYD = ((ucInpRep[SENSOR[ORI_VREG_OFFSET].DatOffset + 3] << BYTE_SHIFT) | ucInpRep[SENSOR[ORI_VREG_OFFSET].DatOffset + 2]);
            _VREGS._data.ORZD = ((ucInpRep[SENSOR[ORI_VREG_OFFSET].DatOffset + 5] << BYTE_SHIFT) | ucInpRep[SENSOR[ORI_VREG_OFFSET].DatOffset + 4]);
            _VREGS._data.ORWD = ((ucInpRep[SENSOR[ORI_VREG_OFFSET].DatOffset + 7] << BYTE_SHIFT) | ucInpRep[SENSOR[ORI_VREG_OFFSET].DatOffset + 6]);
            break;
       
        case INCL_VREG_OFFSET:                                      // Inclinometer data has been read

            _VREGS._data.INXD = ((ucInpRep[SENSOR[INCL_VREG_OFFSET].DatOffset + 1] << BYTE_SHIFT) | ucInpRep[SENSOR[INCL_VREG_OFFSET].DatOffset]);
            _VREGS._data.INYD = ((ucInpRep[SENSOR[INCL_VREG_OFFSET].DatOffset + 3] << BYTE_SHIFT) | ucInpRep[SENSOR[INCL_VREG_OFFSET].DatOffset + 2]);
            _VREGS._data.INZD = ((ucInpRep[SENSOR[INCL_VREG_OFFSET].DatOffset + 5] << BYTE_SHIFT) | ucInpRep[SENSOR[INCL_VREG_OFFSET].DatOffset + 4]);
            break;

        case RAW_ACC_VREG_OFFSET:                                   // Raw data has been read
            
            _VREGS._data.RACXD = ((ucInpRep[SENSOR[RAW_VREG_OFFSET].DatOffset + 1] << BYTE_SHIFT) | ucInpRep[SENSOR[RAW_VREG_OFFSET].DatOffset]);
            _VREGS._data.RACYD = ((ucInpRep[SENSOR[RAW_VREG_OFFSET].DatOffset + 3] << BYTE_SHIFT) | ucInpRep[SENSOR[RAW_VREG_OFFSET].DatOffset + 2]);
            _VREGS._data.RACZD = ((ucInpRep[SENSOR[RAW_VREG_OFFSET].DatOffset + 5] << BYTE_SHIFT) | ucInpRep[SENSOR[RAW_VREG_OFFSET].DatOffset + 4]);
            break;
       
        case RAW_MAG_VREG_OFFSET:                                   // Raw MAG data has been read
            
            _VREGS._data.MGFXD = ((ucInpRep[SENSOR[RAW_MAG_VREG_OFFSET].DatOffset + 1] << BYTE_SHIFT) | ucInpRep[SENSOR[RAW_MAG_VREG_OFFSET].DatOffset]);
            _VREGS._data.MGFYD = ((ucInpRep[SENSOR[RAW_MAG_VREG_OFFSET].DatOffset + 3] << BYTE_SHIFT) | ucInpRep[SENSOR[RAW_MAG_VREG_OFFSET].DatOffset + 2]);
            _VREGS._data.MGFZD = ((ucInpRep[SENSOR[RAW_MAG_VREG_OFFSET].DatOffset + 5] << BYTE_SHIFT) | ucInpRep[SENSOR[RAW_MAG_VREG_OFFSET].DatOffset + 4]);
            _VREGS._data.RMGXD = ((ucInpRep[SENSOR[RAW_MAG_VREG_OFFSET].DatOffset + 7] << BYTE_SHIFT) | ucInpRep[SENSOR[RAW_MAG_VREG_OFFSET].DatOffset + 6]);
            _VREGS._data.RMGYD = ((ucInpRep[SENSOR[RAW_MAG_VREG_OFFSET].DatOffset + 9] << BYTE_SHIFT) | ucInpRep[SENSOR[RAW_MAG_VREG_OFFSET].DatOffset + 8]);
            _VREGS._data.RMGZD = ((ucInpRep[SENSOR[RAW_MAG_VREG_OFFSET].DatOffset + 11] << BYTE_SHIFT) | ucInpRep[SENSOR[RAW_MAG_VREG_OFFSET].DatOffset + 10]);
            break;
        
        case RAW_GYR_VREG_OFFSET:                                   // Raw GYRO data has been read
            
            _VREGS._data.RGYXD = ((ucInpRep[SENSOR[RAW_GYR_VREG_OFFSET].DatOffset + 1] << BYTE_SHIFT) | ucInpRep[SENSOR[RAW_GYR_VREG_OFFSET].DatOffset]);
            _VREGS._data.RGYYD = ((ucInpRep[SENSOR[RAW_GYR_VREG_OFFSET].DatOffset + 3] << BYTE_SHIFT) | ucInpRep[SENSOR[RAW_GYR_VREG_OFFSET].DatOffset + 2]);
            _VREGS._data.RGYZD = ((ucInpRep[SENSOR[RAW_GYR_VREG_OFFSET].DatOffset + 5] << BYTE_SHIFT) | ucInpRep[SENSOR[RAW_GYR_VREG_OFFSET].DatOffset + 4]);
            break;

        default:
            break;
    }
    return;
}


/** HOST_SF_LIB_VREG_read
* @note        Reads & returns contents of requested virtual (VREG) register while updating the SHC VREG to reflect the appropriate sensor. Also checks for input data & parses it into data _VREGS. 
* @param ucRegOffset VREG register of interest
* @param usData Ptr for 16 bit value
* @return completion status 0=SUCCESS, failcodes: 0x31=HID_INT_FAIL, 0x33=VREG_OFFSET_ERR,  
*/ 
UINT8 HOST_SF_LIB_VREG_read(UINT8 ucRegOffset, UINT16 *usData)
{
    UINT8 ucRet = FALSE;
    UINT8 ucRx_data[BUF_40];
    UINT8 ucSensorNum;
    UINT16 *usRegPtr;                                               // Pointer to individual register

    if (ucRegOffset > VREG_MAX_OFFSET)                              //make sure this is within the VREG register set 
        return VREG_OFFSET_ERR;

    usRegPtr = (UINT16 *)&_VREGS.SHC;                                // Set the pointer to SHC (VREG00 register)

    if (EC_DATA_AVAIL && !flag)                                              //SSC7150 sensor has data available for us to read
    {   
        ucRet = i2c_cmd_WrRd (READ,                                  // Read the data from the SSC7150
                            0,                                      //num of cmd bytes
                            0,                                      //cmd buf (ignored)
                            BYTE_ADJ_VAL,                           //num of bytes to read
                            ucRx_data,                              //recv buf 
                            TRUE);                                  //actual # of bytes SSC7150 returns is in 1st two bytes of read packet, this flag(=TRUE) means "use the 1st two bytes as the actual read packet length"
        if (ucRet) 
            return HID_INT_FAIL;

        for (ucSensorNum = 0; ucSensorNum < NUM_SENS; ucSensorNum++) //go through the sensor list to see where to put the received data 
        {              
            if (ucRx_data[2] == SENSOR[ucSensorNum].id)          //1st two bytes of read data packet hold size of packet, then data begins at byte 2
            {
                if (*usRegPtr & (1 << (ucSensorNum + VREG_SHC_ACC_EN_VAL))) // If the incoming data belongs to an enabled sensor (in SHC (VREGS00))
                {
                    parse_update_VREG_data(ucSensorNum, ucRx_data); //store data in appropriate VREG data registers
                    break;                                          //found correct enabled sensor, no reason to look further
                }
            }            
        }   
    }

    usRegPtr += (UINT16) ucRegOffset;                               //point to VREG specified by input parameter 'ucRegOffset'
    *usData = *usRegPtr;                                            //return the register contents specified by the read command
    return SUCCESS;
}

/** HOST_SF_LIB_VREG_write
* @note        Write commands to specified VREG. Reads & returns contents of requested virtual (VREG) register 
*        while updating the SHC VREG to reflect the appropriate sensor. Also checks for input data & parses it into data _VREGS. 
* @param ucRegOffset VREG register of interest
* @param usData Data to be written to device
* @return completion status 0=SUCCESS, failcodes: 0x32=VREG_ACCESS_ERR, 0x33=VREG_OFFSET_ERR, 0x31=HID_INT_FAIL, 0x17=SET_FEAT_FAIL, 0x18=RESET_FAIL, 0x1E=WAKE_CMD_FAIL, 0x1B=SLEEP_CMD_FAIL 
*/ 
UINT8 HOST_SF_LIB_VREG_write(UINT8 ucRegOffset, UINT16 usData)
{
    UINT8 ucRet = FALSE;
    UINT16 *usRegPtr, *usStat_ptr, *usSHC_ptr;                      // Pointer to individual register
    UINT16 usMask;
    UINT8 ucRx_data[BUF_40];                                        // local buffer for i2c comm
    UINT8 ucId = 0xFF;
    UINT8 ucBitOffset, ucAdjustedPtr, ucSensorNum, usTmpStatptr, ucTmpStatVal, ucTmpPtr = 0;


    if (ucRegOffset > VREG_MAX_OFFSET)                              //make sure this is within the VREG register set 
        return VREG_OFFSET_ERR;

    if ( (ucRegOffset == VREG_SL || (ucRegOffset >= VREG_ACXD && ucRegOffset <= VREG_EXPCSS3) ) ) // If the requested register is READ ONLY, return an error
         return VREG_ACCESS_ERR;

    usSHC_ptr = (UINT16 *)&_VREGS.SHC;                               // Set pointers to SHC (VREG00 register)
  
    usPREV_SHC_STATE = *usSHC_ptr;                                  //save previous state of SHC VREG contents for comparison 

    usRegPtr = usSHC_ptr + (UINT16)ucRegOffset;                     //point to VREG specified by input parameter 'ucRegOffset'

    if (_VREGS.SHC.SHstart)                                          // this bit set during VREG_init and after RESET (no user access to this bit)
    { 
        SET_PARAMS.ucPowerState = FULL_POWER;                       //initialize parameters for SetFeature call (NOTE: set to LOW_POWER when disabling sensor)
        SET_PARAMS.bSensitivityValid = FALSE;
        SET_PARAMS.bDataRateValid = FALSE;
        
        if (ucRegOffset == VREG_SHC)                                // If the command will be written to the SHC register
        {
            usStat_ptr = (UINT16 *)&_VREGS.stat;                     // Set the status register pointer to the first status register

       /*****************RESET COMMAND********************/
            if (usData & VREG_SHC_RST_BIT)                          // Reset bit has been set
            {
                if (!hid_i2c_cmd_process(0, RESET_DEV_CMD, ARB_ID)) // Issue the reset command (parameters 1 and 3 are not used)
                {
                    memset(&_VREGS, 0x00, sizeof(_VREGS));            // Initialize VREG registers to POR values
                    _VREGS.stat.stat4.ResetStatus = VREG_RST_CMD_SUCCESS; // Status update to notify command set succesful
                    _VREGS.SHC.SHstart = TRUE;
                    _VREGS.SHC.SHwake = TRUE;
                    return SUCCESS;                                 //return success (all VREG bits have been reset to POR values)
                }
                else
                {
                    _VREGS.stat.stat4.ResetStatus = VREG_RESET_CMD_FAIL; // Status update to notify command set error occurred
                    return RESET_FAIL;
                }
            }

       /*****************WAKE COMMAND********************/
            if (usData & VREG_SHC_WAKE_BIT)                         // wake bit set?
            {
                if ((usPREV_SHC_STATE & VREG_SHC_WAKE_BIT) == 0)    // this bit was not previously set, issue wake command
                {
                    //Wake_signal();                                   //assert wake signal (1 ms toggle of RE9 signal to SSC150)

                    delay(12);                                      //wait 12 ms (11 ms min per spec) after wake signal and before sending POWER_ON command to SSC7150
                                        
                    if (!hid_i2c_cmd_process(0, POWER_ON, ARB_ID))  // Issue the wake command (parameters 1 and 3 are not used)
                    {
                        _VREGS.SHC.sleep = FALSE;                    //clear the sleep bit as per API spec
                        _VREGS.SHC.SHwake = TRUE;
                        _VREGS.stat.stat4.ShSleepWakeStatus = VREG_WAKE_SUCCESS; // Status update to notify command set succesful
                                                
                        //spec says must wait a minimum of 30 ms before next command to SSC7150, so let's wait here...
                        delay(31);                                  //dDlay_ms 31 ms (30 ms min per spec)
                    }
                    else
                    {
                        _VREGS.stat.stat4.ShSleepWakeStatus = VREG_WAKE_FAIL; // Status update to notify command set error occurred
                        return WAKE_CMD_FAIL;
                    }
                }
            }
            
            /*****************SLEEP COMMAND********************/
            if (usData & VREG_SHC_SLP_BIT)                          //sleep bit set?
            {
                //check if any sensors are enabled, if they are then CLEAR SHC Sleep bit and return without setting status bit as per API spec
                if ( (usData & ~(VREG_SHC_SLP_BIT | VREG_SHC_START_BIT) ) > (VREG_SHC_SLP_BIT | VREG_SHC_START_BIT | VREG_SHC_WAKE_BIT) )
                {
                    _VREGS.SHC.sleep = FALSE;
                    return SUCCESS;                                 //although the sleep command wasn't issued, don't return any error because user shouldn't issue sleep with any sensors enabled
                }

                if ((usPREV_SHC_STATE & VREG_SHC_SLP_BIT) == 0)     // this bit was not previously set, issue sleep command
                {
                    if (!hid_i2c_cmd_process(0, SLEEP, ARB_ID))     // Issue the sleep command (parameters 1 and 3 are not used)
                    {
                        _VREGS.stat.stat4.ShSleepWakeStatus = VREG_SLEEP_SUCCESS; // Status update to notify command set succesful
                        _VREGS.SHC.sleep = TRUE;                     // Set SHC (VREG00) to show SSC7150 device is asleep   
                        _VREGS.SHC.SHwake = FALSE;                   // Clear SHC (VREG00) bit that shows device is not awake
                        
                        //spec says must wait a minimum of 70 ms before wake command, so let's wait here...
                        delay(71);                                  //dDlay_ms 71 ms (70 ms min per spec)
                                                
                        return SUCCESS;                             // no need to continue checking (since we already checked if any sensors were enabled) and we won't wake & sleep at the same time
                   }
                   else
                   {
                        _VREGS.stat.stat4.ShSleepWakeStatus = VREG_SLEEP_FAIL;  // Status update to notify command set error occurred
                        return SLEEP_CMD_FAIL;
                   }
                }
            }

        /*****************ENABLE SENSOR COMMANDS********************/
            ucTmpStatVal = 0;                                       //initialize for the case of disabling ALL sensors
            
            for (ucBitOffset = VREG_SHC_ACC_EN_VAL; ucBitOffset <= VREG_SHC_RAWGYR_EN_VAL; ucBitOffset++) // Runs through bits relevant to sensor enable/disable (in VREG00) and executes command
            {
                ucAdjustedPtr = ucBitOffset;
            
                if (ucBitOffset == VREG_SHC_RAWMAG_EN_VAL)          // bit 12 in VREG00 corresponds to Raw Mag. This is bit0 of VREG3D (Stat2)
                    usStat_ptr++;                                   //  so we need to increment the status reg pointer
            
                if ( ((1 << ucBitOffset) & usData) || (usPREV_SHC_STATE & (1 << ucBitOffset)) ) //is enable bit set now OR was it previously set?
                {

                    usTmpStatptr = ucBitOffset;                     // do some pre-calculation for sensor stat bit position

                    if (usTmpStatptr >= VREG_SHC_RAWMAG_EN_VAL)     // Set appropriate offset if the sensor falls in stat2
                        usTmpStatptr = ucBitOffset - 8;
                    ucTmpStatVal = VREG_ENABLE_SUCCESS;             // success status value for VREG status reg


                    if ((1 << ucBitOffset) & usData)                // this sensor's enable bit is set
                    {
                        if ((usPREV_SHC_STATE & (1 << ucBitOffset)) == 0) // this bit was not previously set, need to set this sensor to FULL_POWER      
                        {                           
                            SET_PARAMS.ucid = SENSOR[ucAdjustedPtr - VREG_SHC_ACC_EN_VAL].id;
                            
                            if (set_state_data(SET_PARAMS))         // Set this sensor to FULL POWER
                                ucTmpStatVal = VREG_ENA_DIS_FAIL;   // Error occurred, write an error code to the sensor status register

                            *usStat_ptr &= ~(STATUS1_MASK << (2*(usTmpStatptr - VREG_SHC_ACC_EN_VAL))); //clear stat bits before updating
                            *usStat_ptr |= (ucTmpStatVal << (2*(usTmpStatptr - VREG_SHC_ACC_EN_VAL)));  // Write applicable status value to VREG status register
                            *usSHC_ptr |= (1 << ucBitOffset);       // Set the sensor enable bit in VREG00          
                        }
                        //else if this sensor's enable bit was previously set, no need to do anything 
                    }

                    else if (usPREV_SHC_STATE & (1 << ucBitOffset)) // this sensor's enable bit is not set now, but it was enabled previously, send LOW POWER command 
                    {
                        ucTmpStatVal = VREG_DISABLE_SUCCESS;        //successful disable status value

                        SET_PARAMS.ucid = SENSOR[ucAdjustedPtr - VREG_SHC_ACC_EN_VAL].id;
                        SET_PARAMS.ucPowerState = LOW_POWER;        //set sensor to LOW POWER
                        if (set_state_data(SET_PARAMS))             // Sensor should be disabled
                            ucTmpStatVal = VREG_ENA_DIS_FAIL;       // Error occurred, write an error code to the sensor status register

                        *usStat_ptr &= ~(STATUS1_MASK << (2*(usTmpStatptr - VREG_SHC_ACC_EN_VAL))); //clear stat bits before updating
                        *usStat_ptr |= (ucTmpStatVal << (2*(usTmpStatptr - VREG_SHC_ACC_EN_VAL))); // Write appropriate status value to the status register
                        *usSHC_ptr &= ~(1 << ucBitOffset);          // Clear the sensor enable bit in VREG00        
                    }
                }
            }
            
            if (ucTmpStatVal == VREG_ENA_DIS_FAIL) 
                return SET_FEAT_FAIL;
        }   //end of SHC register

    /*****************SENSITIVITY-CHANGE COMMAND********************/
        if (ucRegOffset >= VREG_ACSEN && ucRegOffset <= VREG_RGYSEN) // data will be written to VREG sensitivity register
        {
            usStat_ptr = (UINT16 *)&_VREGS.stat;                     // Reset the status register pointer to the first status register (VREG3C)
            usStat_ptr++;                                           // Increment to status register (VREG3D) for change in sensitivity (CS) stat bits
        
            for (ucTmpPtr = VREG_ACSEN; ucTmpPtr <= VREG_RGYSEN; ucTmpPtr++) // Loop through the VREG sensitivity registers
            {
                if ( (ucRegOffset == ucTmpPtr) &&                   // If current register matches desired one, and that sensor is enabled...
                    (*usSHC_ptr & (1 << (ucTmpPtr + VREG_ACSEN))) )
                {
                    ucAdjustedPtr = ucTmpPtr - VREG_ACSEN;          //offset from first sensitivity VREG
                    
                    ucId = SENSOR[ucAdjustedPtr].id;                // Set variable equal to id of current sensor
                    break;
                }
            }
            
            if ((ucTmpPtr - VREG_ACSEN) < 4)                        // Sensitivity status of accel, gyro, compass, and orientation, are in VREG 0x3D (3-bits each) starting at bit 4
                ucAdjustedPtr = (3*ucAdjustedPtr) + 4;              // calculate proper ptr value
                    
            else if ((ucTmpPtr - VREG_ACSEN) >= 4 && (ucTmpPtr - VREG_ACSEN) < 9) //Sensitivity status of inclinometer, 2 reserved sensors, raw accelerometr, and raw manometer are in VREG 0x3E
            {
                usStat_ptr++;                                       // point to next VREG status register (VREG3E)
                ucAdjustedPtr = 3 * (-(4 - (ucTmpPtr - VREG_ACSEN))); // calculate proper ptr value
            }

            else                                                    
            {                                                       // Last sensor (raw gyrometer) is in VREG 0x3F
                usStat_ptr += 2;                                    // pointer to last VREG status register
                ucAdjustedPtr = 0;
            }

            SET_PARAMS.ucid = ucId;
            SET_PARAMS.bSensitivityValid = TRUE;                    //update with valid sensitivity value
            SET_PARAMS.usSensitivityVal = usData;
 
            if (set_state_data(SET_PARAMS))                         // If sensor is enabled, sensitivity register will be written to
            {
                *usStat_ptr &= ~(STATUS2_MASK << ucAdjustedPtr);    // Clear status bits before writing to them 
                *usStat_ptr |= (VREG_UPDATE_SENS_FAIL << ucAdjustedPtr);   // Set status bits with Update Sensitivity error (one-shot status that is only valid immediately after a VREGWrite to Sensitivity update because it is shared with Data Rate update)
                return SET_FEAT_FAIL;
            }
            
            else
            {
                *usRegPtr = usData;                                 // write succesful, update new sensitivity value in sensitivity register
                *usStat_ptr &= ~(STATUS2_MASK << ucAdjustedPtr);    // Clear status bits before writing to them            
                *usStat_ptr |= (VREG_UPDATE_SENS_SUCCESS << ucAdjustedPtr); // set status bits with Update Sensitivity Success status (one-shot status that is only valid immediately after a VREGWrite to Sensitivity update because it is shared with Data Rate update)
            }
        }                                                           //end of SENSITIVITY command
    
    /*****************DATA-RATE-CHANGE COMMAND********************/
        if (ucRegOffset >= VREG_ACDXR && ucRegOffset <= VREG_RGYDR) // data will be written to VREG data rate register
        {
            usStat_ptr = (UINT16 *)&_VREGS.stat;                     // Set the status register pointer to the first status register
            usStat_ptr++;                                           // Increment to status register (VREG3D) for data rate stat bits

            for(ucTmpPtr = VREG_ACDXR; ucTmpPtr <= VREG_RGYDR; ucTmpPtr++) // Loop through VREG data rate registers
            {
                if ( (ucRegOffset == ucTmpPtr) &&                   // If current register matches desired one, and that sensor is enabled...
                    (*usSHC_ptr & (1 << (ucTmpPtr - 8))) )
                {
                    ucAdjustedPtr = ucTmpPtr - VREG_ACDXR;          //offset from first data rate VREG
                    

                    ucId = SENSOR[ucAdjustedPtr].id;                // Set variable equal to id of current sensor
                    break;
                }
            }
            
            if ((ucTmpPtr - VREG_ACDXR) < 4)                        // Data rate status of accel, gyro, compass, and orientation, are in VREG 0x3D (3-bits each) starting at bit 4
                ucAdjustedPtr = (3*ucAdjustedPtr) + 4;              // calculate proper ptr value
                
            else if ((ucTmpPtr - VREG_ACDXR) >= 4 && (ucTmpPtr - VREG_ACDXR) < 9) //Data rate status of inclinometer, 2 reserved sensors, raw accelerometr, and raw manometer are found in VREG 0x3E
            {
                usStat_ptr++;                                       // point to next VREG status register
                ucAdjustedPtr = 3 * (-(4 - (ucTmpPtr - VREG_ACDXR))); // calculate proper ptr value
            }
              
            else            
            {                                                       // Last sensor (raw gyrometer) is in VREG 0x3F
                usStat_ptr += 2;                                    // pointer to last VREG status register
                ucAdjustedPtr = 0;
            }
                
            SET_PARAMS.ucid = ucId;
            SET_PARAMS.bDataRateValid = TRUE;                       //update sensor with valid data rate value
            SET_PARAMS.usDataRateVal = usData;
            
            if (set_state_data(SET_PARAMS))                         // If sensor is enabled, data rate register will be written to
            {
                *usStat_ptr &= ~(STATUS2_MASK << ucAdjustedPtr);    // Clear status bits before writing to them   
                *usStat_ptr |= (VREG_UPDATE_DATARATE_FAIL << ucAdjustedPtr); // Update status bits with error (one-shot status that is only valid immediately after a VREGWrite to Data Rate update because it is shared with Sensitivity update)
                return SET_FEAT_FAIL;
            }
            
            else
            {
                *usRegPtr = usData;                                 // write succesful, update new sensitivity value in sensitivity register
                *usStat_ptr &= ~(STATUS2_MASK << ucAdjustedPtr);    // Clear status bits before writing to them    
                *usStat_ptr |= (VREG_UPDATE_DATARATE_SUCCESS << ucAdjustedPtr); // Update status bits with success status (one-shot status that is only valid immediately after a VREGWrite to Data Rate update because it is shared with Sensitivity update)
            }
        }                                                           //end of DATA_RATE command

    /*****************STATUS-BIT-CLEAR COMMAND********************/
        if (ucRegOffset >= VREG_STAT1 && ucRegOffset <= VREG_STAT4) // offset is in status registers field
        {
            usStat_ptr = (UINT16 *)&_VREGS.stat;                     // Reset the status register pointer to the first status register
                
            for (ucTmpPtr = 0; ucTmpPtr < 8; ucTmpPtr++)            //validate current status field = enable_disable_failed
            {
                usMask = (VREG_ENA_DIS_FAIL << (ucTmpPtr * 2) );    //mask individual sensor status fields
                if ( ( usData & usMask) == 0 )                      //clear these stat bits from input data if valid fields exist in status register
                {
                    if ( ucRegOffset == VREG_STAT1)                 //all 8 sensor status fields are valid for possilbe update
                    {
                        if (( *usStat_ptr & usMask) == usMask )     //valid status field contents
                        {
                            *usStat_ptr &= ~usMask;                 //clear these two bits of status field preserving the rest of the register
                        }
                    }
                    else if ((ucRegOffset == VREG_STAT2) && (ucTmpPtr < 2)) // only 1st two status fields of VREG_STAT2 are valid for possible update
                    {
                        if (( *(usStat_ptr+1) & usMask) == usMask ) //valid status field contents
                        {
                            *(usStat_ptr+1) &= ~usMask;             //clear these two bits of status field preserving the rest of the register
                        }
                    }
                }
            }
        }
    }

    if (EC_DATA_AVAIL && !flag)                                              // Check if the device has new data
    {                                                              
         ucRet = i2c_cmd_WrRd (READ,                                // Read the data from the SSC7150
                            0,                                      //num of cmd bytes
                            0,                                      //cmd buf (ignored)
                            BYTE_ADJ_VAL,                           //num of bytes to read
                            ucRx_data,                              //recv buf 
                            TRUE);                                  //actual # of bytes SSC7150 returns is in 1st two bytes of read packet, this flag(=TRUE) means "use the 1st two bytes as the actual read packet length"

       if (ucRet) 
           return HID_INT_FAIL;

        for (ucSensorNum = 0; ucSensorNum < NUM_SENS; ucSensorNum++) //go through the sensor list to see where to put the received data
        {          
            if (ucRx_data[2] == SENSOR[ucSensorNum].id)           //1st two bytes of read data packet hold size of packet, then data begins at byte 2
            {
                if (*usSHC_ptr & (1 << (ucTmpPtr + VREG_SHC_ACC_EN_VAL))) // If the incoming data belongs to an enabled sensor, store it in VREG data registers
                { 
                    parse_update_VREG_data(ucSensorNum, ucRx_data);
                    break;
                }
            }
        }
    }
    
    return SUCCESS;
}


//#include "i2cio.h"



volatile uint16_t status = 0;

void error_handler(String str, UINT16 offset, UINT16 errval) {
  char err_buf[64];

  //output to uart
  sprintf(err_buf, "***Error in function: %s:%02X errval=0x%X*** \r\n", str.c_str(), offset, errval);
  Serial.print(err_buf);
}

volatile BOOL TIMER_1MS_FLG = FALSE;                                        // 1 msec timer interrupt flag


//*****************************************************************************
//*****************************************************************************
//  Section : Code
//*****************************************************************************
//*****************************************************************************

/** gets_I2C1
* @note        Read bytes from sensor device via i2c1 channel slave
* @param ucRdptr pointer to data buffer from slave
* @param usLength number of bytes to read from slave
* @param bAdjust  first bytes of rx packet has number of bytes returned by read, if =TRUE use this new data length
* @return
*/
/*
void gets_I2C1(UINT8 *ucRdptr, UINT16 usLength, BOOL bAdjust) {

  UINT16 i = 0;
  UINT8 ucSize = 1;                                                // Set return value for size of data read for bAdjust=FALSE
  UINT16 usStat = 0;


  while (usLength--)
  {
    if (usLength)                                               // bytes to be read
    {
      ucRdptr[i] = I2C_Read(_I2C_ACK);
      i++;
    }
    else {
      ucRdptr[i++] = I2C_Read(_I2C_NACK);
    }

    if (bAdjust && i == 2)                                      // Read first 2 bytes which have the length of the packet
    {
      usLength = ((ucRdptr[1] << BYTE_SHIFT) | ucRdptr[0]) - 2; // Actual length is value of first 2 bytes minus 2 (because we read 2 already)
      ucSize = usLength;
    }
  }

  if (!ucSize)
  {
    error_handler("i2c ", 0, I2C_ERROR);                     //flag as general i2c error (does not return)
  }

}

*/
/** i2c_cmd_WrRd
* @note        i2c write,read, and combined write/read commands, start timer2 interrupt to exit on unresponsive i2c bus
* @param ucCmd error code
* @param ucBytes_wr Number of bytes to write to slave
* @param ucData_wr Pointer to data buffer to send to slave
* @param usBytes_rd Number of bytes to read from slave
* @param ucData_rd Pointer to data buffer from slave
* @param bAdjust Use 1st 2 bytes returned as new length (=TRUE)
* @return I2C_SUCCESS(=0), I2C_BUF_OVRFLO(=0x22)
*/
UINT8 i2c_cmd_WrRd(UINT8 ucCmd, UINT8 ucBytes_wr, UINT8 *ucData_wr, UINT16 usBytes_rd, UINT8 *ucData_rd, BOOL bAdjust)
{

  if (ucBytes_wr > BUF_150)                                       // sanity check for maximum buffer size
    return I2C_BUF_OVRFLO;                                      // return i2c buffer overflow error code to calling routine

  switch (ucCmd)
  {
  case WRITE:
    Wire.beginTransmission(SLAVE_ADDR);   // Slave address
    Wire.write(ucData_wr,ucBytes_wr); // Write string to I2C Tx buffer (incl. string null at end)
    Wire.endTransmission();           // Transmit to Slave
    break;

  case READ:

    gets_I2C1(ucData_rd, usBytes_rd, bAdjust);          // Read in multiple bytes

    break;

  case WR_RD:


    Wire.beginTransmission(SLAVE_ADDR);   // Slave address
    Wire.write(ucData_wr,ucBytes_wr); // Write string to I2C Tx buffer (incl. string null at end)
    //Serial.println("1");
    Wire.endTransmission(true);           // Transmit to Slave
    //Serial.println("11");
    gets_I2C1(ucData_rd, usBytes_rd, bAdjust);  
    /*int i = 0;
    Wire.beginTransmission(SLAVE_ADDR,1);
    Wire.endTransmission(false);
    while(Wire.available()){
       ucData_rd [i++] = Wire.read();
    }
    Serial.println(i);*/


    break;
  }

  return I2C_SUCCESS;
}


//#include "sf.h"


SF_SENSOR SENSOR[NUM_SENS] = {0};                                         //structure of individual sensors
SF_SENSOR TEMP_SF[NUM_SENS] = {0};                                        //temporary structure of individual sensors for "bookkeeping"
HID_DESCRIPTOR_TABLE HID_FIELD = {0};                                     //structure for HID Descriptor
UINT8 HID_DESC[HID_CNFG_LEN] = {0};                                       //buffer to store the HID config descriptor table
UINT8 RPT_DESC[HID_RPT_LEN] = {0};                                        //buffer to store HID Report Descriptor table
UINT8 RAW_SENSOR_CNT = 0;                                           //for backward compatibility to F/W with all-in-one RAW sensor data

extern SF_VREGS _VREGS;                                              //structure containing the VREG registers 
extern volatile BOOL TIMER_1MS_FLG;                                 // Timer counter variable 
                                 //HIDI2_HOST_INT indicates EC data available


//*****************************************************************************
//*****************************************************************************
//  Section : Code
//*****************************************************************************
//*****************************************************************************

/** hid_i2c_descriptor_handler
* @note        Retrieve either the descriptor or report tables from the SSC7150 device Note: call GET_HID_DESC 1st
* @param ucCmd_req command to execute (GET_HID_DESC/GET_RPT_DESC)
* @return completion_status 0=SUCCESS, 0x10=ID_FAIL, 0x11=HID_DESC_FAIL, 0x12=RPT_DESC_FAIL, 0x14=REP_PARS_FAIL
*/
UINT16 hid_i2c_descriptor_handler(UINT8 ucCmd_req)
{
    UINT8 ucRet = FALSE;
    UINT8 ucTx_data[BUF_40];                                        // buffer for general writes to i2c slave


    switch(ucCmd_req)
    {  
        case GET_HID_DESC:                                          //read the HID Config Descriptor from SSC7150
            ucTx_data[0] = HID_DESC_CMD_LSB;                        // HID descriptor table request is 0x00 01
            ucTx_data[1] = HID_DESC_CMD_MSB;

            ucRet = i2c_cmd_WrRd ( WR_RD,                           //read the HID Config Descriptor from SSC7150
                                DESC_CMD_LEN,                       //num of cmd bytes
                                ucTx_data,                          //cmd buf 
                                HID_CNFG_LEN,                       //num of bytes to read
                                HID_DESC,                           //recv buf
                                FALSE);                             //flag indicating that we specified the number of bytes to read explicitly
        
            Serial.println("--------------------------------------------------------");
            Serial.println("HID Descriptor");
            for (int i = 0 ; i < 10 ; i++)
            Serial.println(HID_DESC[i], HEX);
                        Serial.println("--------------------------------------------------------");

            if (ucRet)
                return HID_DESC_FAIL;                               //read HID Config Descriptor failed
            
            HID_FIELD.wHIDDescLen = ( (HID_DESC[1] << BYTE_SHIFT) | HID_DESC[0] );  // As an error check, parse out descriptor table length
                
            if (HID_FIELD.wHIDDescLen != HID_CNFG_LEN)              // If descriptor table length != 30 there is a problem with the transmission
                return HID_DESC_FAIL;
                
            HID_FIELD.wRepDescLen = ( (HID_DESC[5] << BYTE_SHIFT) | HID_DESC[4] );  // Parse out report descriptor length and store in appropriate variable
                
            if ( !HID_FIELD.wRepDescLen || HID_FIELD.wRepDescLen == 0xFFFF || (HID_FIELD.wRepDescLen > HID_RPT_LEN) ) // Check to see if the report descriptor size is valid
                return HID_DESC_FAIL;
                
            HID_FIELD.wRepDescReg = ( (HID_DESC[6] << BYTE_SHIFT) | HID_DESC[7] ); // Parse out report descriptor register and store in appropriate variable

            HID_FIELD.wCmdReg = ( (HID_DESC[17] << BYTE_SHIFT) | HID_DESC[16] ); // Parse out command register and store in appropriate variable

            HID_FIELD.wDatReg = ( (HID_DESC[19] << BYTE_SHIFT) | HID_DESC[18] ); // Parse out data register and store in appropriate variable

            if ( !( HID_FIELD.wVenID = ( (HID_DESC[21] << BYTE_SHIFT) | HID_DESC[20] )) ) // Parse out vendor ID and store in appropriate variable
                return ID_FAIL;

            if ( !( HID_FIELD.wProdID = ( (HID_DESC[23] << BYTE_SHIFT) | HID_DESC[22] )) ) // Parse out product ID and store in appropriate variable
                return ID_FAIL;
                
            HID_FIELD.wVerID = ( (HID_DESC[25] << BYTE_SHIFT) | HID_DESC[24] ); // Parse out version ID and store in appropriate variable
             
            _VREGS.IDs.PID = HID_FIELD.wProdID;                      // Update virtual register fields
            _VREGS.IDs.VID = HID_FIELD.wVenID;
            _VREGS.IDs.DID = HID_FIELD.wVerID;

            break;

        case GET_RPT_DESC:                                          //read HID Report Descriptor table from SSC7150
            
            ucTx_data[0] = (HID_FIELD.wRepDescReg >> BYTE_SHIFT);   // Report table request is 0x02 00
            ucTx_data[1] = HID_FIELD.wRepDescReg;

            ucRet = i2c_cmd_WrRd (WR_RD,                            //read the HID Report Descriptor from SSC7150
                                DESC_CMD_LEN,                       //num of cmd bytes
                                ucTx_data,                          //cmd buf
                                HID_FIELD.wRepDescLen,              //num of bytes to read
                                RPT_DESC,                           //recv buf
                                FALSE);                             //flag indicating that we specified the number of bytes to read explicitly
            /*Serial.println("--------------------------------------------------------");
            Serial.print("Report Descriptor ");
            Serial.println(HID_FIELD.wRepDescLen);
            for (int i = 0 ; i < 25 ; i++)
            Serial.print(i),
            Serial.print(" -> "),
            Serial.println(RPT_DESC[i], HEX);
                        Serial.println("--------------------------------------------------------");
                     */
            if (ucRet)
                return RPT_DESC_FAIL;                               // Attempt to acquire the report descriptor table from the HID device

            if (report_parse(RPT_DESC))                             // Parse the descriptor for sensor information
               return REP_PARS_FAIL;  
            
            break;
    }

    return SUCCESS;
}


/** hid_i2c_cmd_process
* @note        Send commands or retrieve data to/from SSC7150 device.   Note: calls to GET_HID_DESC/GET_RPT_DESC must already have completed
* @param ucCmdDatbuf ptr to cmd or data buf
* @param ucCmd_req command 
* @param ucReport_id sensor id 
* @return completion_status 0=SUCCESS, failcode: 0x16=GET_FEAT_FAIL, 0x17=SET_FEAT_FAIL, 0x18=RESET_FAIL, 0x19=SET_RPT_FAIL, 0x1A=POWER_ON_FAIL, 0x1B=SLEEP_CMD_FAIL, 0x1C=HID_GET_RPT_INPT_FAIL, 0x1D=HID_GET_RPT_FEAT_FAIL 
*/
UINT8 hid_i2c_cmd_process(UINT8 *ucCmdDatbuf, UINT8 ucCmd_req, UINT8 ucReport_id)
{
    UINT8 ucRetStat;
    UINT8 ucTx_data[BUF_40];                   
    UINT8 ucSensPtr, ucTmpPtr;
    UINT8 ucCmdBufMaxSize = 0;                  
    UINT16 *usVREGSptr = (UINT16 *)&_VREGS;                          // Pointer to individual virtual registers; initialized to start of _VREGS struct

    
    switch(ucCmd_req)
    {
        
        case RESET_DEV_CMD:                                         //HID Reset command                      
            ucTx_data[0] = HID_FIELD.wCmdReg;                       //command field bytes from HID config table                    
            ucTx_data[1] = (HID_FIELD.wCmdReg >> BYTE_SHIFT);       
            ucTx_data[2] = RESET_CMD_LSB;                           //HID Reset command opcode low byte              
            ucTx_data[3] = RESET_CMD_MSB;                           //HID Reset command opcode high byte

            // send the reset command to SSC7150
            ucRetStat = i2c_cmd_WrRd (WRITE,                        //WRITE command packet to SSC7150 
                                CMD_LEN,                            //num of cmd bytes 
                                ucTx_data,                          //cmd buf 
                                0,                                  //num of bytes to read             
                                ucCmdDatbuf,                        //recv buf
                                FALSE);                             //flag indicating that we specified the number of bytes to read explicitly
            
            if (ucRetStat != SUCCESS)
                return RESET_FAIL;
            
            for(int i = 0 ; i < 50 && !EC_DATA_AVAIL ; i++)
              delay(20);
            if (!EC_DATA_AVAIL)                                  // EC interrupt asserted (data is available)
                return RESET_FAIL;                                  // timeout occured without device responding with interrupt
                
            ucRetStat = i2c_cmd_WrRd (READ,                         // EC_DATA_AVAIL flag was set indicating SSC7150 has data available to be read in response to the RESET CMD
                                    0,                              //num of cmd bytes
                                    ucTx_data,                      //cmd buf
                                    2,                              //num of bytes to read
                                    ucCmdDatbuf,                    //recv buf 
                                    FALSE);                         //flag indicating that we specified the number of bytes to read explicitly
                
            if (ucRetStat != SUCCESS)
                return RESET_FAIL; 
            
            if (ucCmdDatbuf[0] != 0 && ucCmdDatbuf[1] != 0)         // expect 1st two bytes of data packet from SSC7150 in response to RESET command to be "00 00"
                return RESET_FAIL;                                  // invalid data found
 
            _VREGS.SHC.reset = VREG_RESET_SUCCESS;                   // Clear the reset VREG to indicate successful 
                   
            break;


        case POWER_ON:      
            ucTx_data[0] = HID_FIELD.wCmdReg;                       //command field bytes from HID config table 
            ucTx_data[1] = (HID_FIELD.wCmdReg >> BYTE_SHIFT);                              
            ucTx_data[2] = POWER_CMD_LSB;                           //HID Power command opcode low  byte for POWER Device ON
            ucTx_data[3] = POWER_CMD_MSB;                           //HID Power command opcode high byte

            ucRetStat = i2c_cmd_WrRd (WRITE,                        // Issue power on command to SSC7150
                                CMD_LEN,                            //num of cmd bytes
                                ucTx_data,                          //cmd buf
                                0,                                  //num of bytes to read
                                ucCmdDatbuf,                        //recv buf
                                FALSE);                             //flag indicating that we specified the number of bytes to read explicitly

            if (ucRetStat != SUCCESS)
                return POWER_ON_FAIL;                               // command failed

            break;


        case SLEEP:         

            ucTx_data[0] = HID_FIELD.wCmdReg;                       //command field bytes from HID config table
            ucTx_data[1] = (HID_FIELD.wCmdReg >> BYTE_SHIFT);            
            ucTx_data[2] = SLEEP_ON_LSB;                            //HID Power command opcode low byte for Device SLEEP
            ucTx_data[3] = POWER_CMD_MSB;                           //HID Power command opcode high byte

            ucRetStat = i2c_cmd_WrRd (WRITE,                        // Issue sleep command to SSC7150
                                CMD_LEN,                            //num of cmd bytes
                                ucTx_data,                          //cmd buf 
                                0,                                  //num of bytes to read
                                ucCmdDatbuf,                        //recv buf
                                FALSE);                             //flag indicating that we specified the number of bytes to read explicitly

            if (ucRetStat != SUCCESS)
                return SLEEP_CMD_FAIL;                              // command failed         
            
            break;

        case HID_GET_RPT_INPT:
                        
            ucCmdDatbuf[0] = ucReport_id;                           //the HOST_SF_LIB_HID_Get_Report expects the sensor id in byte[0] of passed buffer
                        
            if ( HOST_SF_LIB_HID_Get_Report(GET_RRT_INPT, ucCmdDatbuf, 0) )
                return HID_GET_RPT_FEAT_FAIL;                       // command failed
                         
            break;


        case HID_GET_RPT_FEAT:

            ucCmdDatbuf[0] = ucReport_id;                           //the HOST_SF_LIB_HID_Get_Report expects the sensor id in byte[0] of passed buffer
           
            if ( HOST_SF_LIB_HID_Get_Report(GET_RPT_FEAT, ucCmdDatbuf, 0) )
                return HID_GET_RPT_FEAT_FAIL;                       // command failed
            if ((ucCmdDatbuf[2] == 0) || (ucCmdDatbuf[2] > NUM_SENS) ) // check for valid ID range in feature report
                return HID_GET_RPT_FEAT_FAIL;                       // invalid reportID
            //now parse the parameters in returned report feature based on offsets derived earlier from parsing the HID Report Descriptor
            for (ucSensPtr = 0; ucSensPtr < NUM_SENS; ucSensPtr++)  // traverse through the sensor list looking for matching sesnor id
            {   

                if (SENSOR[ucSensPtr].id == ucReport_id)
                {        
                    usVREGSptr = &_VREGS.sensitivity.ACSEN + ucSensPtr; // Set pointer to appropriate sensitivity register (as offset from accelerometer sensitivity VREG02)

                    //retrieve offset pointer to sensitivty parameters for this sensor device (from the Report Descriptor table). Add offset to initial pointer for the actual desired "ACSEN" data within the feature report packet and store this value in VREG register 
                    *usVREGSptr = ( (ucCmdDatbuf[SENSOR[ucSensPtr].SensOffset + GF_SENS_OFFSET_MSB] << BYTE_SHIFT) | ucCmdDatbuf[SENSOR[ucSensPtr].SensOffset + GF_SENS_OFFSET_LSB]); // Use info about offsets to set _VREGS to appropriate data
                    
                    usVREGSptr = &_VREGS.data_rt.ACDXR + ucSensPtr;  // Set pointer to appropriate data rate register
 
      
                    *usVREGSptr = ((ucCmdDatbuf[SENSOR[ucSensPtr].DatRtOffset + GF_DATR_OFFSET_MSB] << BYTE_SHIFT) | ucCmdDatbuf[SENSOR[ucSensPtr].DatRtOffset + GF_DATR_OFFSET_LSB]); //point to appropriate data fields within the feature report and store in VREG reg


                    usVREGSptr = (UINT16 *)&(_VREGS.expo.exp1);      //point to appropriate data fields within the feature report and store in VREG reg  NOTE: each exponent VREG holds 4 devices' 4bit exponent value fields
                    if (ucSensPtr > 3)                              // sensor indexes from 4 to 7 = sensors that have unit exponents in the 2nd exponent register (VREG37)
                        usVREGSptr++;                               // Increment ptr to VREG exponent register to be searched

                    if (ucSensPtr > 7)                              // sensor indexes > 7 = sensors that have unit exponents in the 3rd exponent register (VREG38)
                        usVREGSptr++;                               // Increment ptr to VREG exponent register to be searched

                    ucTmpPtr= ucSensPtr % 4;                        // Find the appropriate offset for this sensor's unit exponent

                    *usVREGSptr &= ~(0xF << (4*ucTmpPtr));          // Clear unit exponent data   

                    if (SENSOR[ucSensPtr].DatExp)
                    {
                       *usVREGSptr |= (SENSOR[ucSensPtr].DatExp << (4*ucTmpPtr));  // Set the unit exponent data to the appropriate VREG  
                    }


                    break;              
                }   
            }                        

            break;
     
        case HID_SET_RPT_FEAT:
            //for non-Vendor commands, the sensor id is NOT passed in byte[0] to 'HOST_SF_LIB_HID_Set_Report'
            ucCmdBufMaxSize = ucCmdDatbuf[RPT_SIZE_LSB];            //get size of GetReportFeature Packet
            
            if ((ucCmdBufMaxSize == 0) || (ucCmdBufMaxSize == 0xFF)) //is the size reasonable?
                return SET_FEAT_FAIL;
                                        
            if ( HOST_SF_LIB_HID_Set_Report(SET_RPT_FEAT, ucCmdDatbuf, ucCmdBufMaxSize) )
                return SET_FEAT_FAIL;                               // command failed

            break;

        default:
            break;
    }

    return SUCCESS;
}

/** report_parse
* @note        Parse data retrieved from SSC7150 HID Report Descriptor, calls and populate required local structs
* @param *ucBuf Buffer holding the report descriptor table
* @return completion_status 0=SUCCESS, failcodes: 0x14=REP_PARS_FAIL, 0x15=NO_EOC_FAIL 
*/
UINT8 report_parse(UINT8 *ucBuf)
{
    UINT8 *ucBptr = ucBuf;                                          // HID Descriptor byte pointer
    UINT8 *ucStrt = ucBptr;                                         // Pointer to hold start address for later reference
    UINT8 ucSensptr = 0;                                            // sensor number variable for struct array traverse
    UINT8 ucUsageOffset;                                            // offset of desired HID_USAGE_SENSOR_PROERTY field in report descriptor for later use in finding parameters within the GetReportFeature data
    BOOL bFlag = FALSE;                                             // Flag variable to notify that desired fields have been reached

   

    memset(TEMP_SF, 0xFF, sizeof(TEMP_SF));                         // Set all of the structs in the TEMP array to 0xFF so we can know if a sensor wasn't found in the report
    
    while( (ucBptr - ucStrt) <= HID_FIELD.wRepDescLen )             // Continue looping until end of report
    {
        if ( *ucBptr == HID_COLLECTION )                            // Look for HID_COLLECTION(Physical) which should be start of REPORT ID (sensor device) info in report descriptor table  
        {
            ucBptr++;                                               // Increment the pointer to look at the next byte
            
            if ( *(ucBptr++) == HID_PHYSICAL && *(ucBptr++) == HID_REPORT_ID ) // Check if the next two bytes meet the next required identifier needs   
            {
                ucUsageOffset = 0;                                  // clear offset of desired HID_USAGE_SENSOR_PROERTY field in report descriptor for later use in finding parameters within the GetReportFeature data                                    
                TEMP_SF[ucSensptr].id = *ucBptr;                    // Store first sensor device ID number in out temp struct
                
                while(*(ucBptr++) != HID_USAGE_SENSOR_TYPE);        // Parse until sensor type indicator found  (in HID table: HID_USAGE_SENSOR_TYPE_MOTION_ACCELEROMETER_3D)
                
                TEMP_SF[ucSensptr].type = *ucBptr;                  // Store sensor type in struct      
                bFlag = TRUE;                                       // Set the flag to show we are in a field of data that we desire
            }
        }
        
        if ( *ucBptr == HID_END_COLLECTION )                        // Look for end of this sensor's collection 
        {
            ucBptr++;
            
            if( *ucBptr == HID_END_COLLECTION ) 
                break;                                              // end of the report has occurred
            
            else if(*(ucBptr++) == HID_REPORT_ID)                   // Next report ID has been found
            {   
                ucUsageOffset = 0;                                  // clear offset of desired HID_USAGE_SENSOR_PROERTY field in report descriptor for later use in finding parameters within the GetReportFeature data
                TEMP_SF[ucSensptr].id = *ucBptr;                    // Store location of sensor ID within the HID Report in struct
                
                while(*(ucBptr++) != HID_USAGE_SENSOR_TYPE);        // Look for sensor type identifier (ie HID_USAGE_SENSOR_TYPE_MOTION_ACCELEROMETER_3D)
                
                TEMP_SF[ucSensptr].type = *(ucBptr++);              // Store location of sensor type within the HID Report in struct   
                bFlag = TRUE;                                       // Set the flag to show we are in a field of data that we desire
            }
        }

        if (bFlag)                                                  // Check if we should proceed parsing within the HID_USAGE_SENSOR_Properties or simply continue incrementing until a new ID is found
        {
            while((ucBptr - ucStrt) <= HID_FIELD.wRepDescLen)       // Search for relevant features HID_USAGE_SENSOR_Properties
            {
                if (*ucBptr == HID_USAGE_SENSOR_PROPERTY)             
                {
                    ucBptr++;
                    if (*ucBptr == HID_USAGE_SENSOR_PROPERTY_CONN_TYPE) // Seach for sensor connection type and increment offset variable because this field is undesired  (in HID table:HID_USAGE_SENSOR_PROPERTY_SENSOR_CONNECTION_TYPE)
                    {
                        ucBptr++;
                        if (*(ucBptr++) == 0x03)                    // last parameter of HID_USAGE_SENSOR_PROPERTY_SENSOR_CONNECTION_TYPE (0x0A,0x09,0x03) 
                            ucUsageOffset++;                        //increment HID_USAGE_SENSOR_PROPERTY_... offset "pointer" (for later use in finding parameters within the GetReportFeature data)
                    }
                    
                    if (*ucBptr == HID_USAGE_SENSOR_PROPERTY_RPT_STATE)  // Search for reporting state and increment offset variable because this field is undesired  (in HID table:HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE)
                    {
                        ucBptr++;
                        if (*(ucBptr++) == 0x03)                    // last parameter of HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL (0x0A,0x0E,0x03) 
                            ucUsageOffset++;                        //increment HID_USAGE_SENSOR_PROPERTY_... offset "pointer" (for later use in finding parameters within the GetReportFeature data)
                    }

                    if (*ucBptr == HID_USAGE_SENSOR_PROPERTY_PWR_STATE)  // Search for power state and increment offset variable because this field is undesired  (in HID table : HID_USAGE_SENSOR_PROPERTY_POWER_STATE)
                    {
                        ucBptr++;
                        if (*(ucBptr++) == 0x03)                    // last parameter of HID_USAGE_SENSOR_PROPERTY_POWER_STATE (0x0A,0x19,0x03) 
                            ucUsageOffset++;                        //increment HID_USAGE_SENSOR_PROPERTY_... offset "pointer" (for later use in finding parameters within the GetReportFeature data)
                     }

                    if (*ucBptr == HID_USAGE_SENSOR_STATE_1)        // Search for sensor state and increment offset variable because this field is undesired  (in HID table:HID_USAGE_SENSOR_STATE)
                    {
                        ucBptr++;
                        if (*(ucBptr++) == 0x02)                    // last parameter of HID_USAGE_SENSOR_STATE (0x0A,0x01,0x02) 
                            ucUsageOffset++;                        //increment HID_USAGE_SENSOR_PROPERTY_... offset "pointer" (for later use in finding parameters within the GetReportFeature data)
                     }

                    if (*ucBptr == HID_USAGE_SENSOR_PROPERTY_RPT_INT)  // Search for reporting interval. We desire this value so store it in our struct and then increment the offset  (in HID table:HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL)
                    {
                        ucBptr++;
                        if (*(ucBptr++) == 0x03)                    // last parameter of HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL (0x0A,0x0E,0x03)
                            TEMP_SF[ucSensptr].DatRtOffset = ucUsageOffset++; //save & increment HID_USAGE_SENSOR_PROPERTY_... offset "pointer" (for later use in finding parameters within the GetReportFeature data)
                    }

                    ucBptr++;                                       //increment position ptr

                    // NOTE: There is a different identifier for RAW data and that is why two identifiers are checked for here
                    if ( (*ucBptr == HID_USAGE_SENSOR_DATA_ACCU) || (*ucBptr == HID_USAGE_SENSOR_DATA_RAW_ACCU) )  // Search for sensor accuracy & increment offset variable because this field is undesired (in HID table:HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_ACCURACY))
                        ucUsageOffset++;                            //increment HID_USAGE_SENSOR_PROPERTY_... offset "pointer" (for later use in finding parameters within the GetReportFeature data)

                    if ( (*ucBptr == HID_USAGE_SENSOR_DATA_RES) || (*ucBptr == HID_USAGE_SENSOR_DATA_RAW_RES) )    // Search for sensor resolution and increment offset variable because this field is undesired (in HID table: HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_RESOLUTION))
                        ucUsageOffset++;                            //increment HID_USAGE_SENSOR_PROPERTY_... offset "pointer" (for later use in finding parameters within the GetReportFeature data)

                    if ( (*ucBptr == HID_USAGE_SENSOR_DATA_MOD_SENS) || (*ucBptr == HID_USAGE_SENSOR_DATA_RAW_MOD_SENS) )  // Search for sensor sensitivity (HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS))
                    {
                        TEMP_SF[ucSensptr].SensOffset = ucUsageOffset++; //save & increment HID_USAGE_SENSOR_PROPERTY_... offset "pointer" (for later use in finding parameters within the GetReportFeature data)                         
                        
                        while(*(ucBptr++) != HID_UNIT_EXP);         // Increment until exponent value of the data is found
                        
                        TEMP_SF[ucSensptr].SensExp = *ucBptr;       // Store this value in the temp struct
                            break;
                    }

                    if ( (*ucBptr == HID_USAGE_SENSOR_DATA_MOD_MAX) || (*ucBptr == HID_USAGE_SENSOR_DATA_RAW_MOD_MAX) )   // Search for sensor MAX val and increment offset variable because this field is undesired HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_MAX)
                        ucUsageOffset++;                            //increment HID_USAGE_SENSOR_PROPERTY_... offset "pointer" (for later use in finding parameters within the GetReportFeature data)
                    
                    if ( (*ucBptr == HID_USAGE_SENSOR_DATA_MOD_MIN)  || (*ucBptr == HID_USAGE_SENSOR_DATA_RAW_MOD_MIN) )  // Search for sensor MIN val and increment offset variable because this field is undesired HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_MIN)
                        ucUsageOffset++;                            //increment HID_USAGE_SENSOR_PROPERTY_... offset "pointer" (for later use in finding parameters within the GetReportFeature data)
                }
                
                else ucBptr++;                                      // If a new identifier has not yet been reached, continue traversing report descriptor 
            }
            
            ucUsageOffset = 0;                                      //reset HID_USAGE_SENSOR_PROPERTY_... offset "pointer" (for later use in finding parameters within the GetReportFeature data)
            
            while((ucBptr - ucStrt) <= HID_FIELD.wRepDescLen)       // Search for relevant input features
            {
                if(*ucBptr == HID_USAGE_SENSOR_)                         
                {
                    ucBptr++;                                       // Continue to next byte
                    
                    if (*ucBptr == HID_USAGE_SENSOR_STATE_1)                  
                    {
                        ucBptr++;
                        if(*(ucBptr++) == HID_USAGE_SENSOR_STATE_2) // Search for HID usage sensor state and increment offset variable because this field is undesired
                            ucUsageOffset++;                        //increment HID_USAGE_SENSOR_PROPERTY_... offset "pointer" (for later use in finding parameters within the GetReportFeature data)
                    }

                    if (*ucBptr == HID_USAGE_SENSOR_EVENT_1)               
                    {
                        ucBptr++;                                   // Continue to next byte
                        if (*(ucBptr++) == HID_USAGE_SENSOR_EVENT_2)   // Search for HID usage sensor event and increment the offset variable
                        {    
                            ucUsageOffset++;                        //increment HID_USAGE_SENSOR_PROPERTY_... offset "pointer" (for later use in finding parameters within the GetReportFeature data)
                            while(*(ucBptr++) != HID_END_COLLECTION); // end of the sensor event field signifies the start of desired input data
                            
                            TEMP_SF[ucSensptr].DatOffset = ucUsageOffset + 3; //save HID_USAGE_SENSOR_PROPERTY_... offset "pointer" (for later use in finding parameters within the GetReportFeature data) NOTE: offset an additional 3 for extra data received on GPIO interrupt
                            
                            while(*(ucBptr++) != HID_UNIT_EXP);     // Search for, and store, the unit exponent value for the input data
                            
                            TEMP_SF[ucSensptr].DatExp = *ucBptr;
                            
                            break;
                        }
                    }
                }
                else ucBptr++;                                      // If desired identifiers haven't been reached, continue traversing HID Report Descriptor
            }
            
            bFlag = FALSE;                                          // Reset the flag to 0 to show that we are done with descriptor data from this report ID
            ucSensptr++;                                            // Increment to the next sensor in the struct array of sensors
        }

        else ucBptr++;                                              // If desired identifiers haven't been reached, continue traversing HID Report Descriptor
    }

    if ((ucBptr - ucStrt) > (HID_FIELD.wRepDescLen + 1))            // Sanity check to ensure the loop exited at the end of the report descriptor
        return NO_EOC_FAIL;

    for(ucSensptr = 0; ucSensptr < NUM_SENS; ucSensptr++)           // Sanity check to see if the desired fields were parsed correctly
    {
        if (TEMP_SF[ucSensptr].id == 0xFF)                          // If the ID == 0xFF it means that the sensor isn't present in the device
            continue;
        if (TEMP_SF[ucSensptr].id != (ucSensptr+1))                 // Make sure every filled struct has a valid sensor ID
             return REP_PARS_FAIL; 
    }

    memset(&SENSOR, 0xFF, sizeof(SENSOR));                          // Init & Rearrange structs into order compatible with _VREGS structure            

    for (ucSensptr = 0; ucSensptr < NUM_SENS; ucSensptr++)          // go through the entire sensor list looking for sensor types
    {
        if (TEMP_SF[ucSensptr].type == ACCEL_SENSOR_TYPE)           // Sensor type identifier for accelerometer
        {
            SENSOR[ACCEL_VREG_OFFSET] = TEMP_SF[ucSensptr];         // Store this struct as the first in the new order
            _VREGS.SL.accel = TRUE;                                  // VREG01 accelerometer available bit(0) set
        }
        
        if (TEMP_SF[ucSensptr].type == GYRO_SENSOR_TYPE)            // Sensor type identifier for gyrometer
        {
            SENSOR[GYRO_VREG_OFFSET] = TEMP_SF[ucSensptr];          // Store this struct as the second in the new order
            _VREGS.SL.gyro = TRUE;                                   // VREG01 gyrometer available bit(1) set
        }
        
        if (TEMP_SF[ucSensptr].type == CMP_SENSOR_TYPE)             // Sensor type identifier for compass
        {
            SENSOR[CMP_VREG_OFFSET] = TEMP_SF[ucSensptr];           // Store this struct as the third in the new order
            _VREGS.SL.cmp = TRUE;                                    // VREG01 compass available bit(2) set
        }
        
        if (TEMP_SF[ucSensptr].type == ORI_SENSOR_TYPE)             // Sensor type identifier for orientation sensor
        {
            SENSOR[ORI_VREG_OFFSET] = TEMP_SF[ucSensptr];           // Store this struct as the fourth in the new order
            _VREGS.SL.ori = TRUE;                                    // VREG01 orientation available bit(3) set
        }
        
        if (TEMP_SF[ucSensptr].type == INCL_SENSOR_TYPE)            // Sensor type identifier for inclinometer
        {
            SENSOR[INCL_VREG_OFFSET] = TEMP_SF[ucSensptr];          // Store this struct as the fifth in the new order
            _VREGS.SL.incl = TRUE;                                   // VREG01 gyrometer available bit(4) set
        }
        
        if (TEMP_SF[ucSensptr].type == RAW_SENSOR_TYPE)             // Sensor type identifier for raw data
        {                                                           // NOTE: there is no VREG01 bit for raw sensors available 
            SENSOR[RAW_VREG_OFFSET+RAW_SENSOR_CNT] = TEMP_SF[ucSensptr]; // Store this struct as the 8th/9th/10th in the new order
            RAW_SENSOR_CNT++;                                       // NOTE: pre-production SSC7150 F/W only enumerates 1 RAW sensor, so keep running count
        }     
    }
    
    return SUCCESS;
}


/** ret_exponent
* @note        Returns the exponent scaler for a requested data field
* @param ucAdj_SensN sensor num of interest
* @return dMult multiplier value
*/ 
double ret_exponent (UINT8 ucAdj_SensN)
{
    UINT8 ucVreg_ptr = VREG_EXP1;                                             // Function status variable
    double dMult = 1;                                               // Unit exponent scaler to be returned
    UINT16 wExpo;                                                   // 'Buffer' to hold the exponent register's contents
    UINT8 ucTemp_expo = 0;                                          // Variable to hold the 4 bit exponent value


    if (ucAdj_SensN > 3)                                            // sensor indexes from 4 to 7 = sensors that have unit exponents in the 2nd exponent register (VREG37)
        ucVreg_ptr++;                                               // Increment ptr to VREG exponent register to be searched

    if (ucAdj_SensN > 7)                                            // sensor indexes > 7 = sensors that have unit exponents in the 3rd exponent register (VREG38)
        ucVreg_ptr++;                                               // Increment ptr to VREG exponent register to be searched
      
    ucAdj_SensN= ucAdj_SensN % 4;                                   // Find the appropriate offset for this sensor's unit exponent


    HOST_SF_LIB_VREG_read (ucVreg_ptr, &wExpo);             // Read this sesnor's data unit exponent value from the VREG
    
    ucTemp_expo = (UINT8)((wExpo >> (4 * ucAdj_SensN)) & 0xF);       // get the 4 exp bits for desired sensor

    if (ucTemp_expo >= 0 && ucTemp_expo <= 7)                        // These values are all positive exponents
        dMult = pow(10, ucTemp_expo);

    if(ucTemp_expo >= 8 && ucTemp_expo <= 0x0F)                      // These values are all negative exponents (ie. to right of decimal place)
        dMult = pow(10, (int16_t)(-16 + ucTemp_expo));

    return dMult;                                                    //return Unit exponent scaler
}

/** HOST_SF_LIB_HID_Set_Report
* @note        Sends the HID I2C set report command to device
* @param type refers to feature or output report; type = 3 - feature, 2 - output
* @param ReportBuffer pointer to data that needs to be sent to the device
* @param size specifies the size, in bytes,of the report buffer 
* @return completion_status 0=SUCCESS, failcode: 0x19=SET_RPT_FAIL
*/
UINT8 HOST_SF_LIB_HID_Set_Report(UINT8 type, UINT8 *ReportBuffer, UINT8 size)
{
    UINT8 ucTx_data[BUF_150];
    UINT8 ucRetStat;
    UINT8 ucCmdBytePtr;
    BOOL bVendorCmd = FALSE;

    if (size <= 0 || size >= (BUF_150 - RPT_LEN))
        return SET_RPT_FAIL;
    
    if (ReportBuffer[0] == FLASH_UPDATE_RPT_ID)                     // the flash update (Vendor) commands have a different format
        bVendorCmd = TRUE;
    
    ucTx_data[0] = HID_FIELD.wCmdReg;                               //command field bytes from HID config table
    ucTx_data[1] = (HID_FIELD.wCmdReg >> BYTE_SHIFT);               // Command register MSB
    if (bVendorCmd)
        ucTx_data[2] = type | ReportBuffer[0];                      // HID Set command opcode low byte which includes the sensor's ReportID, high byte report type
    else
        ucTx_data[2] = type | ReportBuffer[2];
    ucTx_data[3] = SET_OPCODE;                                      // HID SetReport command opcode high byte
    ucTx_data[4] = HID_FIELD.wDatReg;                               //data field bytes from HID config table
    ucTx_data[5] = (HID_FIELD.wDatReg >> BYTE_SHIFT);

    ucCmdBytePtr = RPT_LEN;
    
    if (bVendorCmd)                                                 // flash update (vendor) commands have a different format
        ReportBuffer++;                                             //skips 1st byte of input buffer
    else size++;

    while (size--)
    {
        ucTx_data[ucCmdBytePtr++] = *(ReportBuffer++);              // Append input to the command
    }

    ucRetStat = i2c_cmd_WrRd (WRITE,                                //issue SetReportOutput command to SSC7150
                        ucCmdBytePtr,                               //num of cmd bytes
                        ucTx_data,                                  //cmd buf
                        0,                                          //num of bytes to read initially
                        0,                                          //recv buf
                        FALSE);                                     //actual # of bytes SSC7150 returns is in 1st two bytes of read packet, this flag(=TRUE) means "use the 1st two bytes as the actual read packet length"

    if (ucRetStat != SUCCESS)
    {
        return SET_RPT_FAIL;
    }

    return ucRetStat;
}

/** HOST_SF_LIB_HID_Get_Report
* @note        Sends the HID I2C Get Report command to device
* @param type refers to feature or input report; type = 3 - feature, 1 - input
* @param ReportBuffer pointer towhich the feature report data is read into 
* @param size specifies the size, in bytes, of the report buffer 
* @return completion_status 0=I2C_SUCCESS, failcode: 0x1C=HID_GET_RPT_INPT_FAIL,0x1D=HID_GET_RPT_FEAT_FAIL
*/
UINT8 HOST_SF_LIB_HID_Get_Report(UINT8 type, UINT8* ReportBuffer, UINT8 size)
{
    UINT8 ucTx_data[BUF_150];
    UINT8 ucRetStat;
    UINT8 ucCmdBytePtr;

    ucTx_data[0] = HID_FIELD.wCmdReg;                               //command field bytes from HID config table
    ucTx_data[1] = (HID_FIELD.wCmdReg >> BYTE_SHIFT);               // Command register MSB
    ucTx_data[2] = type | ReportBuffer[0];                          // HID Get command opcode low byte which includes the sensor's ReportID, high byte report type
    ucTx_data[3] = GET_RPT_CMD_MSB;                                 // HID GetReport command opcode high byte
    ucTx_data[4] = HID_FIELD.wDatReg;                               //data field bytes from HID config table
    ucTx_data[5] = (HID_FIELD.wDatReg >> BYTE_SHIFT);

    ucCmdBytePtr = RPT_LEN;

    ucRetStat = i2c_cmd_WrRd (WR_RD,                                 //issue SetReportOutput command to SSC7150
                        ucCmdBytePtr,                               //num of cmd bytes
                        ucTx_data,                                  //cmd buf
                        BYTE_ADJ_VAL,                               //num of bytes to read initially
                        ReportBuffer,                               //recv buf
                        TRUE);                                      //actual # of bytes SSC7150 returns is in 1st two bytes of read packet, this flag(=TRUE) means "use the 1st two bytes as the actual read packet length"
    
    /*Serial.println("--------------------------------------------------------");
    Serial.println("GRP report");
    for (int i = 0 ; i < 30 ; i++)
      Serial.println(ReportBuffer[i], HEX);
    Serial.println("--------------------------------------------------------");
    */
    if (ucRetStat != SUCCESS)
    {   Serial.println("ERROR");
        if (type == GET_RPT_FEAT)
            return HID_GET_RPT_FEAT_FAIL;
        else
            return HID_GET_RPT_INPT_FAIL;
    }

    return ucRetStat;
}


//#include "flashupdate.h"




UINT16 seq_number = 0;
UINT32 image_crc = 0;
BOOL img_wr_done = FALSE;


char ErrorString[][50] =                                            //List of errors returned by SSC7150 firmware
{
        "SPI Communication failure",
        "Flash Protect Error",
        "Flash Write Error",
        "Flash Read Error",
        "Decrypt Signature Error",
        "Packet Framing Error",
        "Packet Checksum Error",
        "Header verification Error",
        "Verification Failure",
        "Command Timeout",
        "Wrong SP Parmeters",
        "Wrong Flash parameters",
        "TAG Write Error",
        "Invalid TAGs \nPlease verify the input file",
        "Image Length Error",
        "Invalid CRC Error",
        "Invalid Flash Address",
        "Invalid Data Length",
        "Unknown Error, Error code out of range"
};

//*****************************************************************************
//*****************************************************************************
//  Section : Code
//*****************************************************************************
//*****************************************************************************

/** flash_info_command
* @note        Implements flash info(+ reset) command during flash update
* @param ucCmdDatbuf pointer to data to be sent as part of flash info command
* @param cmd command byte for reset or bootloader_info sub-command
* @return completion_status 0=SUCCESS, failcode: 0x17=SET_FEAT_FAIL,0x16=GET_FEAT_FAIL, 0x41=FLSH_INFO_ERR
*/
UINT8 flash_info_command(UINT8 *ucCmdDatbuf, UINT8 cmd)
{
   
     if (!ucCmdDatbuf)                                              //sanity check on the buffer
        return FLSH_INFO_ERR;

    /* Form the SET FEAT packet */
    ucCmdDatbuf[0] = FLASH_UPDATE_RPT_ID;                           // set the report id
    ucCmdDatbuf[1] = 0x12;                                          // Data length LSB
    ucCmdDatbuf[2] = 0;                                             // Data length MSB
    ucCmdDatbuf[3] = FLASH_UPDATE_RPT_ID;                           // Report ID
    ucCmdDatbuf[4] = FLASH_INFO_CMD;                                // flash update command
    
    ucCmdDatbuf[5] = cmd;                                           //Reset or BtLdr Info command

    if ( HOST_SF_LIB_HID_Set_Report(SET_RPT_FEAT, ucCmdDatbuf, 5))  //send parameters to SSC7150 via Set Feature command packet
        return SET_FEAT_FAIL;

    //Get the response from the device    
    ucCmdDatbuf[0] = FLASH_UPDATE_RPT_ID;                           // set the report id
    
    if ( HOST_SF_LIB_HID_Get_Report(GET_RPT_FEAT, ucCmdDatbuf, 0) )  //get response parameters from SSC7150 via Get Feature 
        return GET_FEAT_FAIL;

        
    if (ucCmdDatbuf[9] == TRUE)                                     // Check the status code in returned packet
        return SUCCESS;

    else
    {
        // If any error
        memcpy(ucCmdDatbuf, ErrorString[ucCmdDatbuf[10] - 0xA1], 50);
        return FLSH_INFO_ERR;
    }        
}

/** flash_write_command
* @note        Implements flash write command during flash update
* @param ucCmdDatbuf data buffer to write to device 
* @return completion_status 0=SUCCESS, failcode: 0x19=SET_RPT_FAIL,0x41=FLSH_WRITE_ERR
*/
UINT8 flash_write_command(UINT8 *ucCmdDatbuf)
{
    UINT8 i, length;    
    UINT16 chksum = 0;
        UINT8 hid_i2c_rpt_buf[BUF_150];    

    if (!ucCmdDatbuf)                                               //sanity check on the buffer
        return FLSH_WRITE_ERR;

    seq_number++;
    
    // Form the SET FEAT packet 
    hid_i2c_rpt_buf[0] = FLASH_UPDATE_RPT_ID;                       // set the report id
    hid_i2c_rpt_buf[1] = 0x8F;                                      // Data length LSB
    hid_i2c_rpt_buf[2] = 0;                                         // Data length MSB
    hid_i2c_rpt_buf[3] = FLASH_UPDATE_RPT_ID;                       // Report ID
    hid_i2c_rpt_buf[4] = FLASH_WRITE_CMD;                           //flash update command
    hid_i2c_rpt_buf[5] = seq_number & 0x00FF;                       // sequence number LSB
    hid_i2c_rpt_buf[6] = (seq_number & 0xFF00) >> 8;                // sequence number MSB
    hid_i2c_rpt_buf[7] = 0;
    hid_i2c_rpt_buf[8] = 0;

    // byte 7 & 8 are checksum values, data starts from byte 9 
    memcpy(&hid_i2c_rpt_buf[FLSH_WR_DATA_INDEX], ucCmdDatbuf, PACKET_LENGTH);

    length = FLSH_WR_DATA_INDEX + PACKET_LENGTH;

    // For first packet we need to add total packet numbers 
    if (seq_number == 1)
    {
        hid_i2c_rpt_buf[++length] = TOTAL_64BYTE_PKTS & 0x00FF;         // sequence number LSB
        hid_i2c_rpt_buf[++length] = (TOTAL_64BYTE_PKTS & 0xFF00) >> 8;  // sequence number MSB
    
        // First packet contains CRC of unencrypted image we need to store it for verification purpose in future
        image_crc = ucCmdDatbuf[19];
        image_crc = ((image_crc << 8) | ucCmdDatbuf[18]);
        image_crc = ((image_crc << 8) | ucCmdDatbuf[17]);
        image_crc = ((image_crc << 8) | ucCmdDatbuf[16]);
    }

    // Calculate checksum
    for(i=3; i<=(length); i++)
    {
        chksum += hid_i2c_rpt_buf[i];
    }

    // Append checksum
    hid_i2c_rpt_buf[7] = chksum & 0x00FF;                           // checkkum LSB
    hid_i2c_rpt_buf[8] = (chksum & 0xFF00) >> 8;                    // checksum MSB

    // Send SET OUT Report to module 
    if (HOST_SF_LIB_HID_Set_Report(SET_RPT_OUT, hid_i2c_rpt_buf, length))    
    {
        memcpy(ucCmdDatbuf, "SET_RPT_FAIL", 12);                    //return error string
        return SET_RPT_FAIL;
    }

    //Get the response from the device   
    hid_i2c_rpt_buf[0] = FLASH_UPDATE_RPT_ID;                       // set the report id again as it is overwritten by last transaction

    
    if ( HOST_SF_LIB_HID_Get_Report(GET_RPT_FEAT, hid_i2c_rpt_buf,0))
    {
        memcpy(ucCmdDatbuf, "SET_RPT2_FAIL", 12);                   //return error string
        return SET_RPT_FAIL;
    }

    // Check the status code 
    if (hid_i2c_rpt_buf[5] == TRUE)    
    {
        sprintf((char *)ucCmdDatbuf, "%d", seq_number);
        
        if (seq_number >= (TOTAL_64BYTE_PKTS/2))                    // If all packets sent 
            img_wr_done = TRUE;
        return SUCCESS;
    }
    else
    {           //error
        seq_number = 1;
        memcpy(ucCmdDatbuf, ErrorString[hid_i2c_rpt_buf[6] - 0xA1], 50); //get error msg from F/W
        return FLSH_WRITE_ERR;
    } 
}


/** flash_verify_command
* @note        Implements flash verify command during flash update
* @param ucCmdDatbuf contains the CRC received from image
* @return completion_status 0=SUCCESS, failcode: 0x1C=HID_GET_RPT_INPT_FAIL,0x19=SET_RPT_FAIL,0x43=FLSH_VERIFY_ERR,0x44=FLSH_CRC_ERR
*/
UINT8 flash_verify_command(UINT8 *ucCmdDatbuf)
{
    UINT32 prgm_crc;
        UINT8 hid_i2c_rpt_buf[BUF_150];    
    
   
    // Form the SET FEAT packet 
    hid_i2c_rpt_buf[0] = FLASH_UPDATE_RPT_ID;                       // set the report id
    hid_i2c_rpt_buf[1] = 18;                                        // Data length lower byte
    hid_i2c_rpt_buf[2] = 0;                                         // Data length upper byte
    hid_i2c_rpt_buf[3] = FLASH_UPDATE_RPT_ID;                       // Report ID
    hid_i2c_rpt_buf[4] = FLASH_READ_CMD;                            // Actual flash update command

    if (HOST_SF_LIB_HID_Set_Report(SET_RPT_FEAT, hid_i2c_rpt_buf, 4))
      return SET_RPT_FAIL;

    hid_i2c_rpt_buf[0] = FLASH_UPDATE_RPT_ID;                       // set the report id again as it is overwritten by last transaction

    if ( HOST_SF_LIB_HID_Get_Report(GET_RRT_INPT, hid_i2c_rpt_buf,0))
        return HID_GET_RPT_INPT_FAIL;

    if (hid_i2c_rpt_buf[5] == TRUE)
    {
        prgm_crc = hid_i2c_rpt_buf[12];
        prgm_crc = ((prgm_crc << 8) | hid_i2c_rpt_buf[11]);
        prgm_crc = ((prgm_crc << 8) | hid_i2c_rpt_buf[10]);
        prgm_crc = ((prgm_crc << 8) | hid_i2c_rpt_buf[9]);

        if (image_crc == prgm_crc)
            return SUCCESS;
        else
            return FLSH_CRC_ERR;                                    //CRC_ERROR
    }
    
    else
    {
        // If any error
        memcpy(ucCmdDatbuf, ErrorString[hid_i2c_rpt_buf[10] - 0xA1], 50);
        return FLSH_VERIFY_ERR;
    }        
}


//#include "sensor.h"




void accel::init(int rate, int sensitivity) {
  // Enable the Accelerometer
  // To support multi-threaded applications, Read-Modify-Write the VREG:
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata | VREG_SHC_ACC_EN))) // Enable the accelerometer (ACC_EN)
      error_handler("VRWr", VREG_SHC, ucRetStat);  // error display
  }
  else
    error_handler("VRRd", VREG_SHC, ucRetStat);       // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_ACSEN, sensitivity)))   // Change Accelerometer sensitivity
    error_handler("VRWr", VREG_ACSEN, ucRetStat);     // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_ACDXR, rate))) // Change acc data rate
    error_handler("VRWr", VREG_ACDXR, ucRetStat);     // error display
  dMult = ret_exponent(ACCEL_VREG_OFFSET);            // Get unit exponent for this sensor's data
}

void accel::read(){

  // get acc readings
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_ACXD, &wdata)))  // Read data from the accelerometer datax register
    error_handler("VRRd", VREG_ACXD, ucRetStat);  // error display

  dDataX = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_ACYD, &wdata)))  // Read data from the accelerometer datay register
    error_handler("VRRd", VREG_ACYD, ucRetStat);  // error display

  dDataY = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_ACZD, &wdata)))  // Read data from the accelerometer dataz register
    error_handler("VRRd", VREG_ACZD, ucRetStat);  // error display

  dDataZ = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value


  //sprintf(_glob, "Acc:: X: %4.3f Y: %4.3f Z: %4.3f",dDataX, dDataY, dDataZ);
  //Serial.println(_glob);

}

void accel::finish(){

      //Disable the Accelerometer on exit
      if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
      {
        if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata & ~VREG_SHC_ACC_EN))) // Disable the accelerometer (clear the ACC_EN bit)
          error_handler("VRWr", VREG_SHC, ucRetStat); // error display
      }
      else
        error_handler("VRRd", VREG_SHC, ucRetStat);   // error display
}




void orient::init(int rate, int sensitivity) {
  // Enable the Orientation sensor
  // To support multi-threaded applications, Read-Modify-Write the VREG:
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata | VREG_SHC_ORI_EN))) // Enable the orientation (ORI_EN)
      error_handler("VRWr", VREG_SHC, ucRetStat);  // error display
  }
  else
    error_handler("VRRd", VREG_SHC, ucRetStat);       // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_ORSEN, sensitivity)))    // Change ORI sensitivity
    error_handler("VRWr", VREG_ORSEN, ucRetStat);     // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_ORDR, rate)))     // Change ORI data rate
    error_handler("VRWr", VREG_ORDR, ucRetStat);      // error display
  dMult = ret_exponent(ORI_VREG_OFFSET);              // Get unit exponent for this sensor's data
}
void orient::read() {


  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_ORWD, &wdata)))  // Read data from the orientation sensor dataw register
    error_handler("VRRd", VREG_ORWD, ucRetStat);  //error display

    //Serial.print("Wdata: "), Serial.print(wdata), Serial.print("\tdMult: "), Serial.println(dMult);
  dDataW = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_ORXD, &wdata))) // Read data from the orientation sensor datax register
    error_handler("VRRd", VREG_ORXD, ucRetStat);  // error display


    //Serial.print("WdataX: "), Serial.print(wdata), Serial.print("\tdMultX: "), Serial.println(dMult);
  dDataX = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_ORYD, &wdata))) // Read data from the orientation sensor datay register
    error_handler("VRRd", VREG_ORYD, ucRetStat);  // error display

  dDataY = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_ORZD, &wdata))) // Read data from the orientation sensor dataz register
    error_handler("VRRd", VREG_ORZD, ucRetStat);  // error display

  dDataZ = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

  //sprintf(_glob, "Ori:: X: %4.3f Y: %4.3f Z: %4.3f W: %4.3f",dDataX, dDataY, dDataZ, dDataW);
  //Serial.println(_glob);
    
}
void orient::finish() {
      //Disable the orientation sensor on exit
      if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
      {
        if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata & ~VREG_SHC_ORI_EN))) // Disable the orientation sensor (clear the ORI_EN bit)
          error_handler("VRWr", VREG_SHC, ucRetStat);      // error display
      }
      else
        error_handler("VRRd", VREG_SHC, ucRetStat); // error display
    
}


void inclin::init(int rate, int sensitivity) {
  // Enable the Inclinometer
  // To support multi-threaded applications, Read-Modify-Write the VREG:
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata | VREG_SHC_INC_EN))) // Enable the Inclinometer (INC_EN)
      error_handler("VRWr", VREG_SHC, ucRetStat);  // error display
  }
  else
    error_handler("VRRd", VREG_SHC, ucRetStat);       // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_INSEN, sensitivity)))    // Change sensitivity
    error_handler("VRWr", VREG_INSEN, ucRetStat);     // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_INDR, rate)))    // Change data rate
    error_handler("VRWr", VREG_INDR, ucRetStat);      // error display
  dMult = ret_exponent(INCL_VREG_OFFSET);             // Get unit exponent for this sensor's data

}
void inclin::read() {

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_INXD, &wdata))) // Read data from the inclinometer datax register
      error_handler("VRRd", VREG_INXD, ucRetStat);  // error display

    dDataX = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_INYD, &wdata)))  // Read data from the inclinometer datay register
      error_handler("VRRd", VREG_INYD, ucRetStat);  // error display

    dDataY = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_INZD, &wdata)))  // Read data from the inclinometer dataz register
      error_handler("VRRd", VREG_INZD, ucRetStat);  // error display

    dDataZ = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value
}
void inclin::finish() {

      // Disable the Inclinometer on exit
      if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
      {
        if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata & ~VREG_SHC_INC_EN))) // Disable the Inclinometer (clear the INC_EN bit)
          error_handler("VRWr", VREG_SHC, ucRetStat);      // error display
      }
      else
        error_handler("VRRd", VREG_SHC, ucRetStat); // error display
}


void compass::init(int rate, int sensitivity) {
  // Enable the Compass
  // To support multi-threaded applications, Read-Modify-Write the VREG :
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata | VREG_SHC_CMP_EN))) // Enable the compass (CMP_EN)
      error_handler("VRWr", VREG_SHC, ucRetStat);  // error display
  }
  else
    error_handler("VRRd", VREG_SHC, ucRetStat);       // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_CMSEN, sensitivity)))   // Change CMP sensitivity
    error_handler("VRWr", VREG_CMSEN, ucRetStat);     // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_CMDR, rate)))    // Change CMP data rate
    error_handler("VRWr", VREG_CMDR, ucRetStat);      // error display
  dMult = ret_exponent(CMP_VREG_OFFSET);              // Get unit exponent for this sensor's data
}

void compass::read() {

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_CMD, &wdata))) // Read data from the compass data register
      error_handler("VRRd", VREG_CMD, ucRetStat);   // error display

    dDataX = (int16_t)wdata * dMult;                    // Note: for the compass there is only one data field to be retrieved

}

void compass::finish() {

      //Disable the Compass on exit
      if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
      {
        if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata & ~VREG_SHC_CMP_EN))) // Disable the compass (clear the CMP_EN bit)
          error_handler("VRWr", VREG_SHC, ucRetStat);      // error display
      }
      else
        error_handler("VRRd", VREG_SHC, ucRetStat); // error display
}


void gyro::init(int rate, int sensitivity) {
  // Enable the Gyrometer
  // To support multi-threaded applications, Read-Modify-Write the VREG:
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata | VREG_SHC_GYRO_EN))) // Enable the Gyrometer (GYRO_EN)
      error_handler("VRWr", VREG_SHC, ucRetStat);  // error display
  }
  else
    error_handler("VRRd", VREG_SHC, ucRetStat);       // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_GYSEN, sensitivity)))  // Change gyro sensitivity
    error_handler("VRWr", VREG_GYSEN, ucRetStat);     // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_GYDR, rate)))    // Change gyro data rate
    error_handler("VRWr", VREG_GYDR, ucRetStat);      // error display

  dMult = ret_exponent(GYRO_VREG_OFFSET);             // Get unit exponent for this sensor's data
}

void gyro::read() {
    // get gyro readings
    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_GYXD, &wdata)))  // Read data from the gyrometer datax register
      error_handler("VRRd", VREG_GYXD, ucRetStat);  // error display

    dDataX = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_GYYD, &wdata)))  // Read data from the gyrometer datay register
      error_handler("VRRd", VREG_GYYD, ucRetStat);  // error display

    dDataY = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_GYZD, &wdata)))  // Read data from the gyrometer dataz register
      error_handler("VRRd", VREG_GYZD, ucRetStat);  // error display

    dDataZ = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value
}

void gyro::finish() {
  //Disable the Gyrometer on exit
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata & ~VREG_SHC_GYRO_EN))) // Disable the Gyrometer (clear the GYRO_EN bit)
      error_handler("VRWr", VREG_SHC, ucRetStat); // error display
  }
  else
    error_handler("VRRd", VREG_SHC, ucRetStat); // error display
}



void rawAccel::init() {
  // Enable the Raw Accelerometer
  // To support multi-threaded applications, Read-Modify-Write the VREG:
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata | VREG_SHC_RAWACC_EN | VREG_SHC_ACC_EN))) // Enable the raw accelerometer (RAWACC_EN)& corresponding ACCEL sensor
                                                       //rev 0.10 spec : 'for any raw sensor to be enabled properly, the corresponding
                                                       //sensors must also be enabled. For RAW ACCEL corresponding sensor : ACCEL

      error_handler("VRWr", VREG_SHC, ucRetStat);  // error display
  }
  else  {
    error_handler("VRRd", VREG_SHC, ucRetStat);       // error display
  }

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_RACSEN, RAW_SENSITIVITY_MIN))) // Set Raw Acc (and corresponding sensor (Acc)) sensitivity to "0" to
                                        //achieve the desired data rate for raw sensors (see spec).
    error_handler("VRWr", VREG_RACSEN, ucRetStat);    // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_ACSEN, RAW_SENSITIVITY_MIN))) // Set Acc sensitivity to "0" to achieve the desired data rates for raw sensors.
    error_handler("VRWr", VREG_ACSEN, ucRetStat);    // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_RACDR, RAW_DATA_RATE))) // Change Raw Acc data rate
    error_handler("VRWr", VREG_RACDR, ucRetStat);     //error display

  dMult = ret_exponent(RAW_ACC_VREG_OFFSET);          // Get unit exponent for this sensor's data
}

void rawAccel::read() {
    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_RACXD, &wdata)))  // Read data from the raw accelerometer datax register
      error_handler("VRRd", VREG_RACXD, ucRetStat); // error display

    dDataX = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_RACYD, &wdata))) // Read data from the raw accelerometer datay register
      error_handler("VRRd", VREG_RACYD, ucRetStat); // error display

    dDataY = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_RACZD, &wdata)))  // Read data from the raw accelerometer dataz register
      error_handler("VRRd", VREG_RACZD, ucRetStat); // error display

    dDataZ = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value
}

void rawAccel::finish() {

  // Disable the Raw Accelerometer & corresponding ACCEL sensor on exit
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata & ~(VREG_SHC_RAWACC_EN | VREG_SHC_ACC_EN)))) // Disable the raw accelerometer (clear the RAWACC_EN bit) & corresponding ACC_EN
      error_handler("VRWr", VREG_SHC, ucRetStat);      // error display
  }
  else
    error_handler("VRRd", VREG_SHC, ucRetStat); // error display
}

void rawMagnet::init() {
  // Enable the Raw Magnometer
  // To support multi-threaded applications, Read-Modify-Write the VREG :
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata | VREG_SHC_RAWMAG_EN | VREG_SHC_CMP_EN))) // Enable the  Raw Magnometer (RAWMAG_EN) & corresponding CMP sensor
                                                       //rev 0.10 spec: 'for any raw sensor to be enabled properly, the corresponding
                                                       //sensors must also be enabled. For RAW MAG corresponding sensor : CMP
      error_handler("VRWr", VREG_SHC, ucRetStat);  // error display
  }
  else
    error_handler("VRRd", VREG_SHC, ucRetStat);       // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_RMGSEN, RAW_SENSITIVITY_MIN)))  // Set Raw Acc (and corresponding sensor (Cmp)) sensitivity to "0" to
                                         //achieve the desired data rate for raw sensors (see spec).
    error_handler("VRWr", VREG_RMGSEN, ucRetStat);    // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_CMSEN, RAW_SENSITIVITY_MIN))) // Set Cmp sensitivity to "0" to achieve the desired data rates for raw sensors.
    error_handler("VRWr", VREG_CMSEN, ucRetStat);    // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_RMGDR, RAW_DATA_RATE)))  // Change data rate
    error_handler("VRWr", VREG_RMGDR, ucRetStat);     // error display

  dMult = ret_exponent(RAW_MAG_VREG_OFFSET);          // Get unit exponent for this sensor's data
}

void rawMagnet::read() {
    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_RMGXD, &wdata)))  // Read data from the raw magnometer datax register
      error_handler("VRRd", VREG_RMGXD, ucRetStat); // error display

    dDataX = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_RMGYD, &wdata)))  // Read data from the raw magnometer datay register
      error_handler("VRRd", VREG_RMGYD, ucRetStat); // error display

    dDataY = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_RMGZD, &wdata)))  // Read data from the raw magnometer dataz register
      error_handler("VRRd", VREG_RMGZD, ucRetStat); // error display

    dDataZ = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value
}

void rawMagnet::finish() {
  //Disable the Raw Magnometer on exit
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata & ~(VREG_SHC_RAWMAG_EN | VREG_SHC_CMP_EN)))) // Disable the Raw Magnometer (clear the RAWMAG_EN bit) & corresponding CMP_EN
      error_handler("VRWr", VREG_SHC, ucRetStat);      // error display
  }
  else
    error_handler("VRRd", VREG_SHC, ucRetStat); // error display
}


void rawGyro::init() {
  // Enable the Raw Gyrometer
  // To support multi-threaded applications, Read-Modify-Write the VREG:
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata | VREG_SHC_RAWGYR_EN | VREG_SHC_GYRO_EN))) // Enable the Raw Gyrometer (GYR_EN) & corresponding GYRO sensor
                                                        //rev 0.10 spec: 'for any raw sensor to be enabled properly, the corresponding
                                                        //sensors must also be enabled. For RAW GYRO corresponding sensor : GYRO
      error_handler("VRWr", VREG_SHC, ucRetStat);  // error display
  }
  else
    error_handler("VRRd", VREG_SHC, ucRetStat);       // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_RGYSEN, RAW_SENSITIVITY_MIN))) // Set Raw Gyro (and corresponding sensor (Gyro)) sensitivity to "0" to
                                        //achieve the desired data rate for raw sensors (see spec).
    error_handler("VRWr", VREG_RACSEN, ucRetStat);    // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_GYSEN, RAW_SENSITIVITY_MIN))) // Set Gyro sensitivity to "0" to achieve the desired data rates for raw sensors.
    error_handler("VRWr", VREG_GYSEN, ucRetStat);    // error display


  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_RGYDR, RAW_DATA_RATE)))  // Change data rate
    error_handler("VRRd", VREG_RGYDR, ucRetStat);     // error display

  dMult = ret_exponent(RAW_GYR_VREG_OFFSET);          // Get unit exponent for this sensor's data
}

void rawGyro::read() {
    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_RGYXD, &wdata))) // Read data from the raw gyrometer datax register
      error_handler("VRRd", VREG_RGYXD, ucRetStat); // error display

    dDataX = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_RGYYD, &wdata))) // Read data from the raw gyrometer datay register
      error_handler("VRRd", VREG_RGYYD, ucRetStat); // error display

    dDataY = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_RGYZD, &wdata))) // Read data from the raw gyrometer dataz register
      error_handler("VRRd", VREG_RGYZD, ucRetStat); // error display

    dDataZ = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value
}

void rawGyro::finish() {
  //Disable the Raw Gyro on exit
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata & ~(VREG_SHC_RAWGYR_EN | VREG_SHC_GYRO_EN)))) // Disable the raw gyro (clear the GYR_EN bit) & corresponding GYRO_EN
      error_handler("VRWr", VREG_SHC, ucRetStat);      //error display
  }
  else
    error_handler("VRRd", VREG_SHC, ucRetStat); // error display
}

void init_done(){
    flag = TRUE;
    slaveTimer.begin(read_sensors,1000);
}
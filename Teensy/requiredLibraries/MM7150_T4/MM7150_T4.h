//#include <i2c_driver.h>
//#include <i2c_driver_wire.h>
#include <Wire_rogui.h>

typedef unsigned char UINT8;        // 8-bit variables
typedef unsigned char BOOL;
   
typedef unsigned int UINT16;      // 16-bit variables
typedef unsigned long UINT32;
#define TRUE 1
#define FALSE 0


//#include <app.h>

#define MENU_ITEMS          14

#define MENU_STR_LEN        19
#define MIN_MENU_SLCT            2


#define MAX_MENU_SLCT            12       
#define MAX_LINE_NUM            13

#define ACCEL_MENU_SLCT     2       //accelerometer
#define GYRO_MENU_SLCT      3       //gyrometer
#define COMP_MENU_SLCT      4       //compass
#define ORI_MENU_SLCT       5       //orientation
#define INCL_MENU_SLCT      6       //inclinometer
#define RAW_ACC_MENU_SLCT   7       //raw accelerometer
#define RAW_MAG_MENU_SLCT   8       //raw magnometer
#define RAW_GYRO_MENU_SLCT  9       //raw gyrometer
#define SLP_MENU_SLCT                10                //sleep 
#define WAKE_MENU_SLCT                11                //wake
#define FLASH_UPDATE_MENU_SLCT  12  //flash update



#define DBNCE_VAL           300     // Debounce timer value (empirically derived)
#define RAW_DATA_RATE       100
#define RAW_SENSITIVITY     100
#define RAW_SENSITIVITY_MIN 0x0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>

#define PIN_INT 21
void IntPinHandler();
void init_done();
void read_sensors();
class sensor {
    public:
        UINT8 ucRetStat;                                                // return status value
        UINT16 wdata;                                                   // pre-exponent-scaled data
        float dDataW = 0, dDataY = 0, dDataZ = 0, dDataX = 0;          // Data field variables
        UINT16 wRMWdata;
        float dMult = 1;
};

class accel : public sensor{ public: void init(int rate, int sensitivity); void read(); void finish(); };

class orient : public sensor{ public: void init(int rate, int sensitivity); void read(); void finish(); };

class inclin : public sensor{ public: void init(int rate, int sensitivity); void read(); void finish(); };

class compass : public sensor{ public: void init(int rate, int sensitivity); void read(); void finish(); };

class gyro : public sensor{ public: void init(int rate, int sensitivity); void read(); void finish(); };

class rawAccel : public sensor{ public: void init(); void read(); void finish(); };

class rawMagnet : public sensor{ public: void init(); void read(); void finish(); };

class rawGyro : public sensor{ public: void init(); void read(); void finish(); };

//#include "System.h"
void Wake_signal(void);

//#include "interrupts.h"
void InitTimer();
void InitI2CTimer();
void StartI2CTimer();
void StopI2CTimer();

//#include "VREGS.h"

#define VREG_RESET_INIT                 1
#define VREG_RESET_SUCCESS              0
#define VREG_SH_AWAKE                   1
#define VREG_SH_ASLEEP                  0

#define VREG_SHSTART_SUCCESS            1                           //status codes
#define VREG_SHSTART_FAIL               3
#define ARB_ID                          1                           // Arbitrary report ID to be used for POWER_ON and RESET commands (these commands do not require a specific ID)
#define MAX_RETRIES                     3                           // as per SF API spec

#define FULL_POWER                      0x02
#define LOW_POWER                       0x03

#define VREG_RST_CMD_SUCCESS            1
#define VREG_RESET_CMD_FAIL             3

#define VREG_WAKE_SUCCESS               2
#define VREG_WAKE_FAIL                  6
#define VREG_SLEEP_SUCCESS              1
#define VREG_SLEEP_FAIL                 5

#define VREG_ENABLE_SUCCESS             1
#define VREG_DISABLE_SUCCESS                2
#define VREG_ENA_DIS_FAIL               3

#define VREG_UPDATE_SENS_SUCCESS        1
#define VREG_UPDATE_SENS_FAIL           5
#define VREG_UPDATE_DATARATE_SUCCESS    2
#define VREG_UPDATE_DATARATE_FAIL       6

//****************************************************************************
//****************************************************************************
//  Section : Structures
//****************************************************************************
//****************************************************************************
/*****************************************************************************
                           VIRTUAL REGISTER - VREGs                               
******************************************************************************/

/*********************SENSOR HUB CONFIG REGISTER*****************************/

#define VREG_SHC                    0x00                            //Sensor Hub Configuration : VREG 0x00
#define VREG_SHC_WAKE_BIT           (1<<0)
#define VREG_SHC_SLP_BIT            (1<<1)
#define VREG_SHC_START_BIT          (1<<2)
#define VREG_SHC_RST_BIT            (1<<3)
#define VREG_SHC_ACC_EN             (1<<4)
#define VREG_SHC_GYRO_EN            (1<<5)
#define VREG_SHC_CMP_EN             (1<<6)
#define VREG_SHC_ORI_EN             (1<<7)
#define VREG_SHC_INC_EN             (1<<8)
#define VREG_SHC_RAWACC_EN          (1<<11)
#define VREG_SHC_RAWMAG_EN          (1<<12)
#define VREG_SHC_RAWGYR_EN          (1<<13)

#define VREG_SHC_ACC_EN_VAL         4
#define VREG_SHC_RAWACC_EN_VAL      11
#define VREG_SHC_RAWMAG_EN_VAL      12
#define VREG_SHC_RAWGYR_EN_VAL      13


struct SENSOR_HUB_CONFIG                                       
{
    unsigned SHwake:1;                                              // Set when device is awake
    unsigned sleep:1;                                               // Set to enter sleep; clears shWAKE bit
    unsigned SHstart:1;                                             // HID cannot be enabled unless set
    unsigned reset:1;                                               // Set to start HID reset and set all VREGS to POR
    unsigned AccelEnable:1;                                         // Set to enable accelerometer
    unsigned GyroEnable:1;                                          // Set to enable gyrometer
    unsigned CmpEnable:1;                                           // Set to enable compass
    unsigned OriEnable:1;                                           // Set to enable orientation sensor
    unsigned InclEnable:1;                                          // Set to enable inclinometer
    unsigned BarEnable:1;                                           // Set to enable BAR sensor
    unsigned AlsEnable:1;                                           // Set to enable ALS sensor
    unsigned RawAccelEnable:1;                                      // Set to enable user to read raw accelerometer data
    unsigned RawMagEnable:1;                                        // Set to enable user to read raw magnometer data
    unsigned RawGyroEnable:1;                                       // Set to enagle user to read raw gryrometer data
    unsigned :2;
};

/*************************SENSOR LIST REGISTER***************************
The list of available sensors can be acquired by parsing the report
descriptor table. Set bit when a sensor is foud to be availble.
************************************************************************/
#define VREG_SL         0x01                                        //Sensor List: VREG 0x01
struct SENSOR_LIST
{
    unsigned accel:1;                                               //sensor available bit
    unsigned gyro:1;           
    unsigned cmp:1;
    unsigned ori:1;
    unsigned incl:1;
    unsigned res:11;
};

/*********************SENSIVITY REGISTERS*******************************
 Sensitivities of each sensor can be found by running GetFeature for
 each of the sensors and retrieving that parameter
************************************************************************/
#define VREG_ACSEN      0x02                                        //Acc Sensitivity: VREG 0x02
#define VREG_GYSEN      0x03                                        //Gyro Sensitivity: VREG 0x03
#define VREG_CMSEN      0x04                                        //Cmp Sensitivity: VREG 0x04
#define VREG_ORSEN      0x05                                        //Ori Sensitivity: VREG 0x05
#define VREG_INSEN      0x06                                        //Incl Sensitivity: VREG 0x06
#define VREG_RES07      0x07                                        //Reserved: VREG 0x07
#define VREG_RES08      0x08                                        //Reserved: VREG 0x08
#define VREG_RACSEN     0x09                                        //Raw Acc Sensitivity: VREG 0x09
#define VREG_RMGSEN     0x0A                                        //Raw Mag Sensitivity: VREG 0x0A
#define VREG_RGYSEN     0x0B                                        //Raw Gyro Sensitivity: VREG 0x0B

struct SENSIT_REGS
{
    UINT16 ACSEN;                                                   // Acc Sensitivity : VREG 0x02
    UINT16 GYSEN;                                                   // Gyro Sensitivity : VREG 0x03
    UINT16 CMSEN;                                                   // Cmp Sensitivity : VREG 0x04
    UINT16 ORSEN;                                                   // Ori Sensitivity : VREG 0x05
    UINT16 INSEN;                                                   // Incl Sensitivity : VREG 0x06
    UINT16 RES_VREG07;                                              // Reserved : VREG 0x07
    UINT16 RES_VREG08;                                              // Reserved : VREG 0x08
    UINT16 RACSEN;                                                  // Raw Acc Sensitivity : VREG 0x09
    UINT16 RMGSEN;                                                  // Raw MAg Sensitivity : VREG 0x0A
    UINT16 RGYSEN;                                                  // Raw Gyro Sensitivity : VREG 0x0B
};
/*************************DATA RATE REGISTERS***************************
 Data rates of each sensor can be found by running get feature for
 each of the sensors and retrieving that parameter
************************************************************************/
#define VREG_ACDXR      0x0C                                        //Acc DataRate: VREG 0x0C
#define VREG_GYDR       0x0D                                        //Gyro DataRate: VREG 0x0D
#define VREG_CMDR       0x0E                                        //Cmp DataRate: VREG 0x0E
#define VREG_ORDR       0x0F                                        //Ori DataRate: VREG 0x0F
#define VREG_INDR       0x10                                        //Incl DataRate: VREG 0x10
#define VREG_RES11      0x11                                        //Reserved: VREG 0x11
#define VREG_RES12      0x12                                        //Reserved: VREG 0x12
#define VREG_RACDR      0x13                                        //Raw Acc DataRate: VREG 0x13
#define VREG_RMGDR      0x14                                        //Raw Mag DataRate: VREG 0x14
#define VREG_RGYDR      0x15                                        //Raw Gyro DataRate: VREG 0x15

struct DATA_RT_REGS
{
    UINT16 ACDXR;                                                   // Acc DataRate : VREG 0x0C
    UINT16 GYDR;                                                    // Gyro DataRate : VREG 0x0D
    UINT16 CMDR;                                                    // Cmp DataRate : VREG 0x0E
    UINT16 ORDR;                                                    // Ori DataRate : VREG 0x0F
    UINT16 INDR;                                                    // Incl DataRate : VREG 0x10
    UINT16 RES_VREG11;                                              // Reserved : VREG 0x11
    UINT16 RES_VREG12;                                              // Reserved : VREG 0x12
    UINT16 RACDR;                                                   // Raw Acc DataRate : VREG 0x13
    UINT16 RMGDR;                                                   // Raw Mag DataRate : VREG 0x14
    UINT16 RGYDR;                                                   // Raw Gyro DataRate : VREG 0x15
};

/***************************DATA REGISTERS*******************************
 These registers should be updated with relevant data as it comes in
 during interrupts.
************************************************************************/
#define VREG_ACXD       0x16                                        //AccX Data: VREG 0x16
#define VREG_ACYD       0x17                                        //AccY Data: VREG 0x17
#define VREG_ACZD       0x18                                        //AccZ Data: VREG 0x18
#define VREG_RACXD      0x19                                        //Raw AccX Data: VREG 0x19
#define VREG_RACYD      0x1A                                        //Raw AccY Data: VREG 0x1A
#define VREG_RACZD      0x1B                                        //Raw AccZ Data: VREG 0x1B
#define VREG_GYXD       0x1C                                        //GyroX Data: VREG 0x1C
#define VREG_GYYD       0x1D                                        //GyroY Data: VREG 0x1D
#define VREG_GYZD       0x1E                                        //GyroZ Data: VREG 0x1E
#define VREG_RGYXD      0x1F                                        //Raw GyroX Data: VREG 0x1F
#define VREG_RGYYD      0x20                                        //Raw GyroY Data: VREG 0x20
#define VREG_RGYZD      0x21                                        //Raw GyroZ Data: VREG 0x21
#define VREG_CMD        0x22                                        //Cmp Data: VREG 0x22
#define VREG_MGFXD      0x23                                        //Mag FluxX Data: VREG 0x23
#define VREG_MGFYD      0x24                                        //Mag FluxY Data: VREG 0x24
#define VREG_MGFZD      0x25                                        //Mag FluxZ Data: VREG 0x25
#define VREG_RMGXD      0x26                                        //Raw MagX Data: VREG 0x26
#define VREG_RMGYD      0x27                                        //Raw MagY Data: VREG 0x27
#define VREG_RMGZD      0x28                                        //Raw MagZ Data: VREG 0x28
#define VREG_ORXD       0x29                                        //OriX Data: VREG 0x29
#define VREG_ORYD       0x2A                                        //OriY Data: VREG 0x2A
#define VREG_ORZD       0x2B                                        //OriZ Data: VREG 0x2B
#define VREG_ORWD       0x2C                                        //OriW Data: VREG 0x2C
#define VREG_INXD       0x2D                                        //InclX Data: VREG 0x2D
#define VREG_INYD       0x2E                                        //InclY Data: VREG 0x2E
#define VREG_INZD       0x2F                                        //InclZ Data: VREG 0x2F
#define VREG_RES30      0x30                                        //Reserved: VREG 0x30
#define VREG_RES31      0x31                                        //Reserved: VREG 0x31
#define VREG_RES32      0x32                                        //Reserved: VREG 0x32
#define VREG_RES33      0x33                                        //Reserved: VREG 0x33
#define VREG_RES34      0x34                                        //Reserved: VREG 0x34
#define VREG_RES35      0x35                                        //Reserved: VREG 0x35

struct DATA_REGS
{
    UINT16 ACXD;                                                    // AccX Data : VREG 0x16
    UINT16 ACYD;                                                    // AccY Data : VREG 0x17
    UINT16 ACZD;                                                    // AccZ Data : VREG 0x18
    UINT16 RACXD;                                                   // Raw AccX Data : VREG 0x19
    UINT16 RACYD;                                                   // Raw AccY Data : VREG 0x1A
    UINT16 RACZD;                                                   // Raw AccZ Data : VREG 0x1B
    UINT16 GYXD;                                                    // GyroX Data : VREG 0x1C
    UINT16 GYYD;                                                    // GyroY Data : VREG 0x1D
    UINT16 GYZD;                                                    // GyroZ Data : VREG 0x1E
    UINT16 RGYXD;                                                   // Raw GyroX Data : VREG 0x1F
    UINT16 RGYYD;                                                   // Raw GyroY Data : VREG 0x20
    UINT16 RGYZD;                                                   // Raw GyroZ Data : VREG 0x21
    UINT16 CMD;                                                     // Cmp Data : VREG 0x22
    UINT16 MGFXD;                                                   // Mag FluxX Data: VREG 0x23
    UINT16 MGFYD;                                                   // Mag FluxY Data: VREG 0x24
    UINT16 MGFZD;                                                   // Mag FluxZ Data: VREG 0x25
    UINT16 RMGXD;                                                   // Raw MagX Data : VREG 0x26
    UINT16 RMGYD;                                                   // Raw MagY Data : VREG 0x27
    UINT16 RMGZD;                                                   // Raw MagZ Data : VREG 0x28
    UINT16 ORXD;                                                    // OriX Data : VREG 0x29
    UINT16 ORYD;                                                    // OriY Data : VREG 0x2A
    UINT16 ORZD;                                                    // OriZ Data : VREG 0x2B
    UINT16 ORWD;                                                    // OriW Data : VREG 0x2C
    UINT16 INXD;                                                    // InclX Data : VREG 0x2D
    UINT16 INYD;                                                    // InclY Data : VREG 0x2E
    UINT16 INZD;                                                    // InclZ Data : VREG 0x2F
    UINT16 RES_VREG30;                                              // Reserved : VREG 0x30
    UINT16 RES_VREG31;                                              // Reserved : VREG 0x31
    UINT16 RES_VREG32;                                              // Reserved : VREG 0x32
    UINT16 RES_VREG33;                                              // Reserved : VREG 0x33
    UINT16 RES_VREG34;                                              // Reserved : VREG 0x34
    UINT16 RES_VREG35;                                              // Reserved : VREG 0x35
};

//**********************************ID Registers****************************************
#define VREG_PID        0x36                                        //Product ID: VREG 0x36
#define VREG_VID        0x37                                        //Vendor ID: VREG 0x37
#define VREG_DID        0x38                                        //Device ID: VREG 0x38

struct IDS
{
    UINT16 PID;                                                     //ProductID : VREG 0x36   Note:  The product ID can be retrieved from the HID descriptor table
    UINT16 VID;                                                     //VendorID : VREG 0x37  Note: The vendor ID can be retrieved from the HID descriptor table
    UINT16 DID;                                                     //DeviceID : VREG 0x38 Note: The device ID can be retrieved from the HID descriptor table
};

/*************************EXPONENT REGISTERS***************************
**********************************************************************/
#define VREG_EXP1       0x39                                        //Exponent1: VREG 0x39
struct EXPONENT1                    
{
    unsigned expA:4;
    unsigned expG:4;
    unsigned expC:4;
    unsigned expO:4;
};

#define VREG_EXP2       0x3A                                        //Exponent2: VREG 0x3A
struct EXPONENT2                     
{
    unsigned expIN:4;
    unsigned expB:4;
    unsigned expAL:4;
    unsigned expRA:4;
};

#define VREG_EXP3       0x3B                                        //Exponent3: VREG 0x3B
struct EXPONENT3                     
{
    unsigned expRM:4;
    unsigned expRG:4;
    unsigned res:8;
};

//**********************Structure of the Exponents ************************
struct EXPONENT
{
    struct EXPONENT1 exp1;
    struct EXPONENT2 exp2;
    struct EXPONENT3 exp3;
};

#define VREG_EXPCSS1    0x3C                                        //ExponentCS1: VREG 0x3C
struct EXPONENTCS1                   
{
    unsigned expCSA:4;
    unsigned expCSG:4;
    unsigned expCSC:4;
    unsigned expCSO:4;
};

#define VREG_EXPCSS2    0x3D                                        //ExponentCS2: VREG 0x3D
struct EXPONENTCS2
{
    unsigned expSCIN:4;
    unsigned expCSB:4;
    unsigned expCSAL:4;
    unsigned expCSRA:4;
};

#define VREG_EXPCSS3    0x3E                                        //ExponentCS3: VREG 0x3E
struct EXPONENTCS3                   
{
    unsigned expCSRM:4;
    unsigned expCSRG:4;
    unsigned res:8;
};

//**********************Structure of the Exponents ************************
struct EXPONENTCS
{
    struct EXPONENTCS1 expCS1;
    struct EXPONENTCS2 expCS2;
    struct EXPONENTCS3 expCS3;
};

//***********************Structure for Status******************************
#define VREG_STAT1      0x3F                                        //Status 1: VREG 0x3F
struct STATUS1                       
{
    unsigned Accstatus:2;
    unsigned GyroStatus:2;
    unsigned CmpStatus:2;
    unsigned OriStatus:2;
    unsigned IncStatus:2;
    unsigned ResStatus1:2;
    unsigned ResStatus2:2;
    unsigned RawAccStatus:2;
};

#define VREG_STAT2      0x40                                        //Status 2 VREG 0x40
#define STATUS1_MASK    0x03
#define STATUS2_MASK    0x07
struct STATUS2                       
{
    unsigned RawMagStatus:2;
    unsigned RawGyroStatus:2;
    unsigned AccCSData:3;
    unsigned GyroCSData:3;
    unsigned CmpCSData:3;
    unsigned OriCSData:3;
};

#define VREG_STAT3      0x41                                        //Status 3: VREG 0x41
struct STATUS3                       
{
    unsigned InclCSData:3;
    unsigned resCSData1:3;
    unsigned resCSData2:3;
    unsigned RawAccCSData:3;
    unsigned RawMagCSData:3;
    unsigned res:1;
};

#define VREG_STAT4      0x42                                        //Status 4: VREG 0x42
struct STATUS4                       
{
    unsigned RawGyroCSDAta:3;
    unsigned SHStartStatus:2;
    unsigned ResetStatus:2;
    unsigned ShSleepWakeStatus:3;
    unsigned res:6;
};

struct STATUS
{
    struct STATUS1 stat1;
    struct STATUS2 stat2;
    struct STATUS3 stat3;
    struct STATUS4 stat4;
};


typedef struct SF_VREGS                                             // The entire set of virtual registers
{
    struct SENSOR_HUB_CONFIG SHC;
    struct SENSOR_LIST SL;
    struct SENSIT_REGS sensitivity;
    struct DATA_RT_REGS data_rt;
    struct DATA_REGS _data;
    struct IDS IDs;
    struct EXPONENT expo;
    struct EXPONENTCS expCS;
    struct STATUS stat;
} SF_VREGS;


typedef struct SENSOR                                               // parameters for individual sensors
{
   UINT8 id;
   UINT8 type;
   UINT8 DatRtOffset;
   UINT8 SensOffset;
   UINT8 SensExp;
   UINT8 DatOffset;
   UINT8 DatExp;
} SF_SENSOR, SF_TEMP;

typedef struct GET_SET_PARAMS
{
    UINT8 ucid;
    UINT8 ucPowerState;
    UINT16 usSensitivityVal;
    BOOL bSensitivityValid;
    UINT16 usDataRateVal;
    BOOL bDataRateValid;
} GET_SET_PARAMS;

UINT8 VREG_init();
UINT8 HOST_SF_LIB_VREG_read(UINT8, UINT16 *);
UINT8 HOST_SF_LIB_VREG_write(UINT8, UINT16 );
void parse_update_VREG_data (UINT8, UINT8 *);

//#include "i2cIO.h"

#define CONFIG_I2C_VAL  0x8000
#define CONFIG_BRG_VAL  8
#define WRITE           1
#define READ            2
#define WR_RD           3
#define SLAVE_ADDR      0x40

void gets_I2C1(UINT8 *, UINT16, BOOL);
UINT8 i2c_cmd_WrRd(UINT8, UINT8,  UINT8 *, UINT16,  UINT8 *, BOOL);


//#include "sf.h"

#define GET_HID_DESC            1
#define GET_RPT_DESC            2
#define RESET_DEV_CMD           1
#define POWER_ON                2
#define SLEEP                   3
#define HID_GET_RPT_INPT        4
#define HID_GET_RPT_FEAT        5
#define HID_SET_RPT_OUTP        6
#define HID_SET_RPT_FEAT        7

#define HID_DESC_CMD_LSB        01
#define HID_DESC_CMD_MSB        00
#define DESC_CMD_LEN            2
                                 
#define BUF_40                  40                                  //temp buffer size = 40 bytes
#define BUF_30                  30                                  //temp buffer size = 30 bytes
#define BUF_150                 150
#define HID_CNFG_LEN            BUF_30
#define HID_RPT_LEN             3500                                //size of HID Report Descriptor


#define BYTE_ADJ_VAL            3

#define HID_COLLECTION          0xA1
#define HID_PHYSICAL            0x00
#define HID_REPORT_ID           0x85

#define GF_SENS_OFFSET_MSB      7
#define GF_SENS_OFFSET_LSB      6
#define GF_DATR_OFFSET_MSB      4
#define GF_DATR_OFFSET_LSB      3


#define TIMEOUT_11MS            11                                  // set timeout for 11 ms per API spec
#define TIMEOUT_5SEC            5000                                // RESET timeout 5 secs per HID spec
#define NUM_SENS                12                                  // max number of sensors per API spec

#define VREG_MAX_OFFSET         0x3F                                // Maximum VREG offset

#define RESET_CMD_MSB           0x01                                // RESET command bytes
#define RESET_CMD_LSB           0x00
#define CMD_LEN                 0x04

#define POWER_CMD_MSB           0x08                                // POWER command bytes
#define POWER_CMD_LSB           0x00
#define SLEEP_ON_LSB            0x01                                // SLEEP command bytes

#define GET_RPT_CMD_MSB         0x02                                // GET REPORT commmand bytes
#define GET_RRT_INPT            0x10
#define GET_RPT_FEAT            0x30

#define RPT_LEN                 6

#define SET_RPT_OUT             0x20                                // SET REPORT command bytes
#define SET_RPT_FEAT            0x30
#define SET_OPCODE              0x03

#define BYTE_SHIFT              8

// Report Feature Defines
#define RPT_SIZE_LSB            0
#define RPT_REPORT_STATE        4
#define RPT_PWR_STATE           5
#define RPT_SENS_STATE          6
#define RPT_REPORT_INTVAL_LSB   7
#define RPT_REPORT_INTVAL_MSB   8
#define RPT_CHG_SENS_LSB        13
#define RPT_CHG_SENS_MSB        14

// REPORT PARSE MACROS
#define HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_ENUM 0x02
#define HID_USAGE_SENSOR_TYPE                   0x09
#define HID_USAGE_SENSOR_PROPERTY               0x0A
#define HID_USAGE_SENSOR_PROPERTY_CONN_TYPE     0x09                //HID byte sequence : 0x0A,0x09,0x03
#define HID_USAGE_SENSOR_PROPERTY_RPT_STATE     0x16                //HID byte sequence : 0x0A,0x16,0x03
#define HID_USAGE_SENSOR_PROPERTY_PWR_STATE     0x19                //HID byte sequence : 0x0A,0x19,0x03
#define HID_USAGE_SENSOR_                       0x0A
#define HID_USAGE_SENSOR_STATE_1                0x01                //HID byte sequence : 0x0A,0x01,0x02
#define HID_USAGE_SENSOR_STATE_2                0x02                //HID byte sequence : 0x0A,0x01,0x02
#define HID_USAGE_SENSOR_EVENT_1                0x02                //HID byte sequence : 0x0A,0x02,0x02
#define HID_USAGE_SENSOR_EVENT_2                0x02                //HID byte sequence : 0x0A,0x02,0x02
#define HID_USAGE_SENSOR_PROPERTY_RPT_INT       0x0E                //HID byte sequence : 0x0A,0x0E,0x03


#define HID_USAGE_SENSOR_DATA_ACCU              0x44                //the sensor data accuracy value is the #define HID_USAGE_SENSOR_DATA(a,b) value (where (a|b)).
                                                                    // For example, for the accelerometer: 	HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_ACCURACY)
                                                                    //  HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION | HID_USAGE_SENSOR_DATA_MOD_ACCURACY = 0x04 | 0x40 = 0x44 this is accuracy value for Non-RAW sensors
#define HID_USAGE_SENSOR_DATA_RAW_ACCU          0x45                //the sensor data accuracy value is the #define HID_USAGE_SENSOR_DATA(a,b) value (where (a|b)).
                                                                    // For example, for RAW sensor: 	HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_CUSTOM_VALUE,HID_USAGE_SENSOR_DATA_MOD_ACCURACY),
                                                                    //  HID_USAGE_SENSOR_DATA_CUSTOM_VALUE | HID_USAGE_SENSOR_DATA_MOD_ACCURACY = 0x05 | 0x40 = 0x45 this is accuracy value for RAW sensors

#define HID_USAGE_SENSOR_DATA_RES               0x54                //see above comment for 'HID_USAGE_SENSOR_DATA_ACCU'substituting 'HID_USAGE_SENSOR_DATA_MOD_RESOLUTION' for 'HID_USAGE_SENSOR_DATA_MOD_ACCURACY'
                                                                    //  HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_RESOLUTION)
                                                                    //  where #define HID_USAGE_SENSOR_DATA_MOD_RESOLUTION 0x50
#define HID_USAGE_SENSOR_DATA_RAW_RES           0x55                //  HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_CUSTOM_VALUE,HID_USAGE_SENSOR_DATA_MOD_RESOLUTION)

#define HID_USAGE_SENSOR_DATA_MOD_SENS          0x14                //see above comment for 'HID_USAGE_SENSOR_DATA_ACCU'substituting 'HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS' for 'HID_USAGE_SENSOR_DATA_MOD_ACCURACY'
                                                                    // HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS)
                                                                    // where #define HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS 0x10
#define HID_USAGE_SENSOR_DATA_RAW_MOD_SENS      0x15                // HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_CUSTOM_VALUE,HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS)

#define HID_USAGE_SENSOR_DATA_MOD_MAX           0x24                //see above comment for 'HID_USAGE_SENSOR_DATA_ACCU'substituting 'HID_USAGE_SENSOR_DATA_MOD_MAX' for 'HID_USAGE_SENSOR_DATA_MOD_ACCURACY'
                                                                    //HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_MAX)
                                                                    // where #define HID_USAGE_SENSOR_DATA_MOD_MAX 0x20
#define HID_USAGE_SENSOR_DATA_RAW_MOD_MAX       0x25                //HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_CUSTOM_VALUE,HID_USAGE_SENSOR_DATA_MOD_MAX)

#define HID_USAGE_SENSOR_DATA_MOD_MIN           0x34                //see above comment for 'HID_USAGE_SENSOR_DATA_ACCU'substituting 'HID_USAGE_SENSOR_DATA_MOD_MIN' for 'HID_USAGE_SENSOR_DATA_MOD_ACCURACY'
                                                                    //HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_MIN)
                                                                    // where #define HID_USAGE_SENSOR_DATA_MOD_MIN 0x30
#define HID_USAGE_SENSOR_DATA_RAW_MOD_MIN       0x35                //HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_CUSTOM_VALUE,HID_USAGE_SENSOR_DATA_MOD_MIN)

#define HID_UNIT_EXP                            0x55                //  HID_UNIT_EXPONENT = 0x55

#define HID_END_COLLECTION      0xC0

#define ACCEL_VREG_OFFSET       0                                   // SENSOR TYPES (set in order that they are stored in VREGS struct)
#define GYRO_VREG_OFFSET        1
#define CMP_VREG_OFFSET         2
#define ORI_VREG_OFFSET         3
#define INCL_VREG_OFFSET        4
#define RAW_VREG_OFFSET         7           
#define RAW_ACC_VREG_OFFSET     7
#define RAW_MAG_VREG_OFFSET     8
#define RAW_GYR_VREG_OFFSET     9

#define ACCEL_SENSOR_TYPE       0x73
#define GYRO_SENSOR_TYPE        0x76               
#define CMP_SENSOR_TYPE         0x83               
#define ORI_SENSOR_TYPE         0x8A               
#define INCL_SENSOR_TYPE        0x86               
#define RAW_SENSOR_TYPE         0xE1

UINT16 hid_i2c_descriptor_handler(UINT8 );
UINT8 hid_i2c_cmd_process(UINT8 *, UINT8 , UINT8 );
double ret_exponent(UINT8);
UINT8 report_parse(UINT8 *);

UINT8 HOST_SF_LIB_HID_Set_Report(UINT8, UINT8*, UINT8);
UINT8 HOST_SF_LIB_HID_Get_Report(UINT8, UINT8*, UINT8);

typedef struct HID_DESCRIPTOR_TABLE                                 // Struct to store HID descriptor fields
{
    UINT16 wHIDDescLen;                                             // MSB 1; LSB 0
    UINT16 wRepDescLen;                                             // MSB 5; LSB 4
    UINT16 wRepDescReg;                                             // MSB 6; LSB 7
    UINT16 wMaxInpLen;                                              // MSB 11; LSB 10
    UINT16 wCmdReg;                                                 // MSB 17; LSB 16
    UINT16 wDatReg;                                                 // MSB 19; LSB 18
    UINT16 wVenID;                                                  // MSB 21; LSB 20
    UINT16 wProdID;                                                 // MSB 23; LSB 22
    UINT16 wVerID;                                                  // MSB 25; LSB 24
} HID_DESCRIPTOR_TABLE;

//#include "flashupdate.h"

#define GET_OPCODE                  0x02
#define FLASH_UPDATE_RPT_ID         0xE
#define FLASH_UPDATE_RESET_CMD      0x55
#define FLASH_UPDATE_BLDR_INFO_CMD  0xAA
#define LABEL_INFO_INDEX            15
#define FLASH_INFO_CMD              0x1
#define FLASH_WRITE_CMD             0x3
#define FLASH_READ_CMD              0x4


#define TOTAL_64BYTE_PKTS           1636                            //(SF memory area to be 102400 bytes, hence 104448/64 + 4 pkts added during encryption)
#define PACKET_LENGTH               128
#define FLSH_WR_DATA_INDEX          9                               // during flash write actual data starts from index 9
#define MAX_ERRORS                  20

//#include "err.h"

// Error code definitions 
#define SUCCESS             0 

//i2c definitions
#define I2_TIMEOUT_PERIOD 500                                       //500 ms

#define I2C_SUCCESS     0
#define I2C_STOP 1
#define I2C_ERROR       (0x21)
#define I2C_BUF_OVRFLO  (0x22)
#define WRITE_COLL      (0x23)
#define NOT_ACK         (0x24)
#define BUS_COLL        (0x25)
#define RX_OVRFLO       (0x26)
#define HID_DESC_RET    (0x27)
#define REP_DESC_RET    (0x28)
#define I2C_TIMEOUT_ERR (0x29)

#define ACKSTAT_bit     (1<<15)
#define BCL_bit         (1<<10)
#define IWOL_bit        (1<<7)
#define I2COV_bit       (1<<6)

//sf.h error definitions
//error codes
#define ID_FAIL                 (0x10)
#define HID_DESC_FAIL           (0x11)
#define RPT_DESC_FAIL           (0x12)
#define REP_PARS_FAIL           (0x14)
#define NO_EOC_FAIL             (0x15)
#define GET_FEAT_FAIL           (0x16)
#define SET_FEAT_FAIL           (0x17)
#define RESET_FAIL              (0x18)
#define SET_RPT_FAIL            (0x19)
#define POWER_ON_FAIL           (0x1A)
#define SLEEP_CMD_FAIL          (0x1B)
#define HID_GET_RPT_INPT_FAIL   (0x1C)
#define HID_GET_RPT_FEAT_FAIL   (0x1D)
#define WAKE_CMD_FAIL           (0x1E)
   
#define HID_INT_FAIL            (0x31)
#define VREG_ACCESS_ERR         (0x32)
#define VREG_OFFSET_ERR         (0x33)

#define FLSH_INFO_ERR           (0x41)
#define FLSH_WRITE_ERR          (0x42)
#define FLSH_VERIFY_ERR         (0x43)
#define FLSH_CRC_ERR            (0x44)
//****************************************************************************
//****************************************************************************
//  Section : Function Declarations
//****************************************************************************
//****************************************************************************

void i2cIO_error(UINT8);
void error_handler( String , UINT16, UINT16);
void StartI2CTimer();                                               //use timer2 interrupt to recover from unresponsive i2c transactions
void StopI2CTimer();

//#include "sensors.h"

void _accel();
void _compass();
void _gyro();
void _inclin();
void _orient();
void _rawAccel();
void _rawGyro();
void _rawMagnet();


//****************************************************************************
//****************************************************************************
//  Section : Defines
//****************************************************************************
//****************************************************************************

//menu selection 

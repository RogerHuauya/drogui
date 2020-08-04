#include "spi.h"
#include "io.h"
void initSPI(){

    RPINR20bits.SDI1R = 98; //SDI1
    RPOR11bits.RP104R = 5; //SDO1
    RPOR4bits.RP79R = 6; // SCK1
    //RPOR8bits.RP79R = 7; //SS1


    pinMode(PRTF, 2, INPUT);
    pinMode(PRTF, 3, OUTPUT);
    pinMode(PRTF, 8, OUTPUT);
    pinMode(PRTD, 15, OUTPUT);
    spiStop();

    /* The following code sequence shows SPI register configuration for Master mode */
    IFS0bits.SPI1IF = 0; // Clear the Interrupt flag
    IEC0bits.SPI1IE = 0; // Disable the interrupt
    // SPI1CON1 Register Settings
    SPI1CON1bits.DISSCK = 0; // Internal serial clock is enabled
    SPI1CON1bits.DISSDO = 0; // SDOx pin is controlled by the module
    SPI1CON1bits.MODE16 = 0; // Communication is word-wide (16 bits)
    SPI1CON1bits.MSTEN = 1; // Master mode enabled
    SPI1CON1bits.SMP = 1; // Input data is sampled at the middle of data output time
    SPI1CON1bits.CKE = 1; // Serial output data changes on transition from
    // Idle clock state to active clock state
    SPI1CON1bits.CKP = 1; // Idle state for clock is a low level;
    // active state is a high level
    SPI1STATbits.SPIEN = 1; // Enable SPI module
    // Interrupt Controller Settings
    //IFS0bits.SPI1IF = 0; // Clear the Interrupt flag
    //IEC0bits.SPI1IE = 1; // Enable the interrupt
}
// Full Duplex SPI Functions
char spiExchangeByte(char b)
{
    SPI1BUF = b;
    while(!SPI1STATbits.SPIRBF);
    return SPI1BUF;
}

void spiExchangeBlock(char *block, int blockSize)
{
    char *data = block;
    while(blockSize--)
    {
        *data = spiExchangeByte(*data );
        data++;
    }
}

// Half Duplex SPI Functions
void spiWriteBlock(char *block, int blockSize)
{
    char *data = block;
    while(blockSize--)
    {
        spiExchangeByte(*data++);
    }
}

void spiReadBlock(char *block, int blockSize)
{
    char *data = block;
    while(blockSize--)
    {
        *data++ = spiExchangeByte(0);
    }
}

void spiWriteByte(char byte)
{
    SPI1BUF = byte;
}

char spiReadByte(void)
{
    return SPI1BUF;
}

bool spiAvailable(){
    return SPI1STATbits.SPIRBF;
}

void spiStart(){
    digitalWrite(CS, LOW);
}

void spiStop(){
    digitalWrite(CS, HIGH);
}
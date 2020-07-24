#ifndef SPI_H
#define SPI_H
#include <xc.h>
void initSPI();
int spiWrite(char data);

char spiRead();
char spiAvailable();

#endif
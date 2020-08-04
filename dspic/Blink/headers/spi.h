#ifndef SPI_H
#define SPI_H
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#define CS PRTF, 3
void initSPI();

// Full Duplex SPI Functions
char spiExchangeByte(char b);
void spiExchangeBlock(char *block, int blockSize);

// Half Duplex SPI Functions
void spiWriteBlock(char *block, int blockSize);
void spiReadBlock(char *block, int blockSize);

void spiWriteByte(char byte);

char spiReadByte(void);

bool spiAvailable();
void spiStart();
void spiStop();

#endif
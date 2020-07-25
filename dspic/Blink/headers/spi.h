#ifndef SPI_H
#define SPI_H
#include <xc.h>
void initSPI();

// Full Duplex SPI Functions
uint8_t spiExchangeByte(uint8_t b);
void spiExchangeBlock(void *block, int blockSize);

// Half Duplex SPI Functions
void spiWriteBlock(void *block, int blockSize);
void spiReadBlock(void *block, int blockSize);

void spiWriteByte(uint8_t byte);

uint8_t spiReadByte(void);

#endif
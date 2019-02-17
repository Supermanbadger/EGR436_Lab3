/*
 * spi.h
 *
 *  Created on: Jan 20, 2019
 *      Author: colli
 */

#ifndef SPI_H_
#define SPI_H_

//Prototypes
void SPI_PortInit();
int SPI_SendByte(uint8_t txData);
int SPI_SendCommand(uint8_t cmd);
int SPI_SendData(uint16_t byteCount, uint8_t * dataPointer);
int SPI_ReadByte(uint8_t * rxData);
int SPI_ReadData(uint8_t rxData[]);

//Macros
#define CLOCK_IN 0x00



#endif /* SPI_H_ */

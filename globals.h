/*
 * globals.h
 *
 *  Created on: Jan 19, 2019
 *      Author: Collin
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

//Macros

#define YES 1
#define NO  0


#define END_OF_TXT      03  //Char to control the end of commands
#define END_OF_TRAN     04

#define BUFFER_SIZE 1024
#define UART_BUFFER_SIZE 24
#define MAX_CMD_SIZE 10
#define MAX_NAME_SIZE 50

//Globals
uint8_t SPI_TXFlag; //Flag set to alert program that data is ready to be sent
uint8_t SPI_RXFlag; //Flag set to alert program that data is ready to be parsed

uint8_t SPI_TXBuff [BUFFER_SIZE]; //Buffers for both received and sent data
uint8_t SPI_RXBuff [BUFFER_SIZE];

uint16_t SPI_TXWrtIndex; //Indexes for buffer operation
uint16_t SPI_TXReadIndex;
uint16_t SPI_RXWrtIndex;
uint16_t SPI_RXReadIndex;

uint8_t UARTFlag;

//UART Serial Buffer handles
uint8_t RxBuffer[BUFFER_SIZE];
uint8_t RxRead[BUFFER_SIZE];
uint8_t TxBuffer[BUFFER_SIZE];
int RxWriteIndex;
int RxReadIndex;
int RxReadTo;
int RxReadTo2;
uint8_t FileName[UART_BUFFER_SIZE];
uint8_t FileText[BUFFER_SIZE];
int ReadType;

#endif /* GLOBALS_H_ */

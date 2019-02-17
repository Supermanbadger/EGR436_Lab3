/*
 * BT.h
 *
 *  Created on: Feb 15, 2019
 *      Author: Tom
 */

#ifndef BT_H_
#define BT_H_


uint8_t BT_init();
int CheckFullCommandBT();
int UART_ParseCommandBT(uint8_t * inString, uint16_t len, uint8_t * outString);
int UART_ParseIndexBT(uint8_t * inString, uint16_t len, uint8_t * index);


#endif /* BT_H_ */

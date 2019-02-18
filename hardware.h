/*
 * hardware.h
 *
 *  Created on: Jan 14, 2019
 *      Author: colli
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

//Prototypes
void LFXT_Init ();
void LedOutput_Init();
void Input_Init();
void TimerA1_Init();
void TimerA2_Init();
int BPMToTicks(int bpm);
void TimerA2_Delay(uint16_t ms);
//void TA1_0_IRQHandler();




#endif /* HARDWARE_H_ */

/*
 * serial.c
 *
 *  Created on: Jan 14, 2019
 *      Author: Collin Maker
 *
 *
 */
#include "msp.h"
#include "globals.h"
#include "serial.h"

//================================================================================
//Initializes UART for serial communications
//================================================================================
void UART0_init(void)
{
    /*ONLY WORKS FOR BAUD OF 19200*/
    EUSCI_A0->CTLW0 |=1; //Reset register=1 to allow config
    EUSCI_A0->MCTLW = 0;// Disable oversampling
    EUSCI_A0->CTLW0 = 0x0081; //1 S bit, no parity, SMCLK, 8-bit data
    EUSCI_A0->BRW = 26;  /*3MHz SMCLK/115200 = 26.0416 */

    P1->SEL0 |=  0x0C;  //Setup pins 1.3 & 1.2 for UART communication
    P1->SEL1 &=~ 0x0C;

    EUSCI_A0->CTLW0 &=~ 1; //End config of EUSCI
    EUSCI_A0->IE |= 0x01; //Interrupt enable

    NVIC_SetPriority(EUSCIA0_IRQn, 4);
    NVIC_EnableIRQ(EUSCIA0_IRQn);
}

void UART2_init(void)
{
    /*ONLY WORKS FOR BAUD OF 19200*/
    EUSCI_A2->CTLW0 |=1; //Reset register=1 to allow config
    EUSCI_A2->MCTLW = 0;// Disable oversampling
    EUSCI_A2->CTLW0 = 0x0081; //1 S bit, no parity, SMCLK, 8-bit data
    EUSCI_A2->BRW = 26;  /*3MHz SMCLK/115200 = 26.0416 */

    P3->SEL0 |=  0x0C;  //Setup pins 3.3 & 3.2 for UART communication
    P3->SEL1 &=~ 0x0C;

    EUSCI_A2->CTLW0 &=~ 1; //End config of EUSCI
    EUSCI_A2->IE |= 0x01; //Interrupt enable

    NVIC_SetPriority(EUSCIA2_IRQn, 4);
    NVIC_EnableIRQ(EUSCIA2_IRQn);
}

//================================================================================
//Function used to see if a full command has been received, if returns true
// call ReadFromBuffer to parse command from buffer
//================================================================================
int CheckFullCommand()
{
    int i;
    int count = 0;
    for(i=RxReadIndex; i!=RxWriteIndex; i = (i+1)%BUFFER_SIZE)
    {

        if(RxBuffer[i] == END_OF_TRAN)
        {
            RxReadTo = ++i;
            return 1;

        }
    }
    return 0; //Command not yet finished
}
//================================================================================
//Function will read from previous RxReadIndex up until the RxReadTo set in CheckFullCommand()
//================================================================================
void ReadFromBuffer()
{
    int i = 0;

    for(RxReadIndex; RxReadIndex != RxReadTo; RxReadIndex = (RxReadIndex + 1) % BUFFER_SIZE)
    {
        RxRead[i++] = RxBuffer[RxReadIndex];

    }
    RxRead[i] = '\0';
    RxReadIndex++; //Moves past the newline character
    UARTFlag = 0;
}
//================================================================================
//Function will repeat the last received and parsed command from the computer
// back to the computer
//================================================================================
void EchoCommand()
{
    int i;
    for(i = 0; i < BUFFER_SIZE; i++)
    {
        if(RxRead[i] == '\0')
        {
            break;
        }
        while((EUSCI_A0->IFG&0x02) == 0);
        EUSCI_A0->TXBUF=RxRead[i];
    }
}

int UART_ParseCommand(uint8_t * inString, uint16_t len, uint8_t * outString)
{
    int i;
    for(i = 0; i < len; i++)
    {
        if(inString[i] == END_OF_TXT)
        {
            break;
        }
        *outString = inString[i];
        outString++;
    }
    //strncpy(outString, inString, i);

    if(i == len)
    {
        return -1; //No command found in string
    }
}

int UART_ParseFile(uint8_t * inString, uint16_t len, uint8_t * outString, uint16_t * fileLen)
{
    int i;
    *fileLen = 0;

    for(i = 0; i < len; i++)
    {
        if(inString[i] == END_OF_TXT)
        {
            break;
        }
    }
    i+=2; //Skip EOT and Null char in buff
    for(i; i < len ; i++)
    {
        if(inString[i] == END_OF_TRAN)
        {
            break;
        }
        *outString = inString[i];
        outString++;
        *fileLen+=1;
    }

    return 0;
}
int UART_ParseIndex(uint8_t * inString, uint16_t len, uint8_t * index)
{
    int i, j=0;
    uint8_t temp[100];

    for(i = 0; i < len; i++)
    {
        if(inString[i] == END_OF_TXT)
        {
            break;
        }
    }
    i+=2; //Skip EOT and Null char in buff

    for(i; i < len ; i++)
    {
        if(inString[i] == END_OF_TRAN)
        {
            break;
        }
        temp[j] = inString[i];
        j++;
    }
    *index = atoi(temp);

    return 0;

}

void UART_ReturnData(uint8_t * data, uint16_t len,uint8_t port)
{
    int i;
    if (port == 0)
    {
        for(i = 0; i < len; i++)
        {
            while((EUSCI_A0->IFG&0x02) == 0);
            EUSCI_A0->TXBUF=data[i];
        }
    }
    else if (port == 2)
    {
        for(i = 0; i < len; i++)
        {
            while((EUSCI_A2->IFG&0x02) == 0);
            EUSCI_A2->TXBUF=data[i];
        }
    }
}

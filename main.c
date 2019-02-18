#include "msp.h"
#include "globals.h"
#include "spi.h"
#include "flash.h"
#include "poems.h"
#include <stdio.h>
#include "hardware.h"
#include "serial.h"
#include "strings.h"
#include "BT.h"




/**
 * main.c
 */
//Local Prototypes
void InitHardware();
void InitSoftware();

void main(void)
{
    //-------------------------------------------

    int8_t err;
    //-------------------------------------------

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    InitHardware();
    InitSoftware();
    err = BT_init();

    if (err)
    {
        err = BT_init();
    }

    InitSoftware();


	while (1)   //Main program loop
	{
	    Lab3_MainMenu();

	}
}

void InitHardware()
{
    __disable_irq();
    SPI_PortInit();
    UART0_init();
    UART2_init();
    LedOutput_Init();
    TimerA2_Init();
    __enable_irq();
}


void InitSoftware()
{
    UARTFlag = 0;
    //Define pointer index
    RxWriteIndex = 0;
    RxReadIndex = 0;

}
/************************************
 * Operation of IRQ:
 *  TX flag should be re-enabled if another
 *  byte is ready to be sent. This will be useful for low
 *  power mode later.
 *
 *  Ideally writing etc will be handled in
 *  individual functions as needed.
 *
 *  Process should be:
 *      Set Flags to 0
 *      Write data to register;
 *      Wait (Sleep eventually) for flag
 *      Clear flag
 *      Write more data
 *      Repeat until process complete
 *
 ************************************/

void EUSCIA3_IRQHandler(void)//Does not operate as intended at the moment
{
    if(EUSCI_A3->IFG & UCTXIFG) //Set flag if data transfer is still operating
    {
        SPI_TXFlag = 1; //TX successful
        EUSCI_A3->IFG &= ~UCTXIFG;
    }
    if(EUSCI_A3->IFG & UCRXIFG)
    {
        SPI_RXFlag = 1; //RX successful
        EUSCI_A3->IFG &= ~(UCTXIFG|UCRXIFG);
    }
    return;

}

void EUSCIA0_IRQHandler(void)
{

    RxBuffer[RxWriteIndex] = EUSCI_A0->RXBUF;   //Takes char from buffer and puts writes to serial buffer
    RxWriteIndex = (RxWriteIndex + 1) % BUFFER_SIZE;    //Increments the circular buffer write index
    P2->OUT ^= 2;   //Toggles led for debug
    UARTFlag = 1;   //Set UART flag to begin parsing of the buffer
}

void EUSCIA2_IRQHandler(void)
{

    RxBuffer[RxWriteIndex] = EUSCI_A2->RXBUF;   //Takes char from buffer and puts writes to serial buffer
    RxWriteIndex = (RxWriteIndex + 1) % BUFFER_SIZE;    //Increments the circular buffer write index
    P2->OUT ^= 2;   //Toggles led for debug
    UARTFlag = 2;   //Set UART flag to begin parsing of the buffer
}

void Lab3_MainMenu(void)
{

    uint8_t data[MAX_ADDR];
    uint16_t len;
    uint16_t addr, free, total;
    uint8_t index;
    uint8_t command[100];
    uint8_t* fileName;
    uint8_t* fileTxt;
    uint16_t* txtTemp;

    if(UARTFlag==1)    //Characters have been received from the PC
    {

        while(!CheckFullCommand());  //Look for newline character in the buffer

        ReadFromBuffer();   //Get full command

        UART_ParseCommand(RxRead, sizeof(RxRead), command);

        if (strstr(command, "STORE") != NULL)
        {
            UART_ParseFile(RxRead, sizeof(RxRead), data, &len);
            Flash_StoreFile(data, len);

            sprintf(data, "Received, File size: %dB\n", len);
            UART_ReturnData(data, strlen(data),0);
        }

        if (strstr(command, "CLEAR") != NULL)
        {
            Flash_FormatDevice();

            sprintf(data, "Formatted\n", len);
            UART_ReturnData(data, strlen(data),0);
        }

        if (strstr(command, "DIR") != NULL)
        {
            Flash_DisplayIndex(data);

            UART_ReturnData(data, strlen(data),0);
        }
        if (strstr(command, "MEM") != NULL)
        {
            Flash_GetMemSize(&free, &total);

            sprintf(data, "Storage: %dB Free of %dB\n", free, total);
            UART_ReturnData(data, strlen(data),0);

        }
        if (strstr(command, "DELETE") != NULL)
        {
            UART_ParseIndex(RxRead, sizeof(RxRead), &index);
            Flash_DeleteFile(index);

            sprintf(data, "Deleted %d\n", index);
            UART_ReturnData(data, strlen(data),0);
        }
        if (strstr(command, "READ") != NULL)
        {
            UART_ParseIndex(RxRead, sizeof(RxRead), &index);
            Flash_ReadFile(index, data, &len);

            UART_ReturnData(data, strlen(data),0);
        }


        memset(command, 0, sizeof(command));
        memset(data, 0, sizeof(data));
    }
    else if (UARTFlag == 2)
    {
        while(!CheckFullCommandBT());  //Look for newline character in the buffer

        ReadFromBufferBT();   //Get full command

        UART_ParseCommandBT(RxRead, sizeof(RxRead), command);

        if (strstr(command, "CLEAR") != NULL)
        {
            Flash_FormatDevice();

            sprintf(data, "Formatted\n", len);
            UART_ReturnData(data, strlen(data),2);
        }

        if (strstr(command, "DIR") != NULL)
        {
            Flash_DisplayIndex(data);

            UART_ReturnData(data, strlen(data),2);
        }
        if (strstr(command, "MEM") != NULL)
        {
            Flash_GetMemSize(&free, &total);

            sprintf(data, "Storage: %dB Free of %dB\n", free, total);
            UART_ReturnData(data, strlen(data),2);

        }
        if (strstr(command, "DELETE") != NULL)
        {
            UART_ParseIndexBT(RxRead, sizeof(RxRead), &index);
            Flash_DeleteFile(index);

            sprintf(data, "Deleted %d\n", index);
            UART_ReturnData(data, strlen(data),2);
        }
        if (strstr(command, "READ") != NULL)
        {
            UART_ParseIndexBT(RxRead, sizeof(RxRead), &index);
            Flash_ReadFile(index, data, &len);

            UART_ReturnData(data, strlen(data),2);
        }


        memset(command, 0, sizeof(command));
        memset(data, 0, sizeof(data));

    }

}


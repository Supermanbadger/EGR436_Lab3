/*
 * BT.c
 *
 *  Created on: Feb 15, 2019
 *      Author: Tom Quist
 */
#include "msp.h"
#include "globals.h"
#include "serial.h"
#include "flash.h"
#include "stdio.h"

uint8_t BT_init()
{
    uint8_t data[MAX_ADDR];
    uint16_t len;


    strcpy(data,"AT");

    UART_ReturnData(data, strlen(data),2);

    //Manual way to get around command syntax but to ensure BT module is responding
    while (UARTFlag != 2);
    UARTFlag = 0;
    while (UARTFlag != 2);
    RxReadTo = 3;
    /////
    ReadFromBuffer();


    if (strstr(RxRead, "OK") != NULL)
    {


        memset(data, 0, sizeof(data));
        strncpy(data,"AT+START",8);
        UART_ReturnData(data, strlen(data),2);

        //printf("Good");
        return 0;
    }

    else
    {
        return -1;
    }

}

int CheckFullCommandBT()
{
    int i;
    int count = 0;
    for(i=RxReadIndex; i!=RxWriteIndex; i = (i+1)%BUFFER_SIZE)
    {

        if(RxBuffer[i] == '@')
        {
            RxReadTo = ++i;
            return 1;

        }
    }
    return 0; //Command not yet finished
}

int UART_ParseCommandBT(uint8_t * inString, uint16_t len, uint8_t * outString)
{
    int i;
    for(i = 0; i < len; i++)
    {
        if(inString[i] == '@')
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

int UART_ParseIndexBT(uint8_t * inString, uint16_t len, uint8_t * index)
{
    int i, j=0;
    uint8_t temp[100];

    for(i = 0; i < len; i++)
    {
        if(inString[i] == '@')
        {
            break;
        }
    }
    i+=2; //Skip EOT and Null char in buff

    for(i; i < len ; i++)
    {
        if(inString[i] == '#')
        {
            break;
        }
        temp[j] = inString[i];
        j++;
    }
    *index = atoi(temp);

    return 0;

}







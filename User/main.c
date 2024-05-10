//////////////////////////////////////////////////////////////////////////////////
//??????????????????????????????????????????????
//  ????????   : 1.8??LCD 4??????????(STM32???)
/******************************************************************************
//????????????STM32F103C8
//              GND   ?????
//              VCC   ??5V??3.3v???
//              SCL   ??PA5??SCL??
//              SDA   ??PA7??SDA??
//              RES   ??PB0
//              DC    ??PB1
//              CS    ??PA4
//							BL		??PB10
*******************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "QDTFT_demo.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include <stdio.h>
#include "stdarg.h"
#include "Serial.h"
#include "Delay.h"
#include "W25Q64.h"
unsigned char findOK()
{
    unsigned char found = 0;
    char rxData;
    char tempString[50] = {0};
    unsigned int i = 0, j;
    unsigned int len = Serial_GetRxFlag();

    if (len > 0)
    {
        const char* a = Serial_get_seril_buffer();

        // 将缓冲区内容复制到 tempString
        for (i = 0; i < len && i < sizeof(tempString) - 1; i++)
        {
            tempString[i] = a[i];
        }
        tempString[i] = '\0'; // 添加终止符，使 tempString 成为字符串

        // 在 tempString 中查找 "OK"
        for (j = 0; j <= len - 2 /*至少要有两个字符才能构成"OK"*/; j++)
        {
            if (tempString[j] == 'O' && tempString[j + 1] == 'K')
            {
                found = 1;
                break;
            }
        }
    }

    return found;
}
unsigned char connectWifi()
{

}
unsigned char connect()
{
    Serial_SendString("AT+CIPSTART=\"UDP\",\"192.168.0.103\",7001,7001,0\r\n");
    delay_ms(100);
    return findOK();
}
unsigned char recv(char *temp) {
    char rxDate;
    unsigned char size;
    char* tempString;
    unsigned int i = 0,j=0;
    unsigned long long timeCount = 0;
    j=Serial_GetRxFlag();
    tempString=Serial_get_seril_buffer();
    if (j>0) {
        timeCount = 0;
        size = 0;
        for (i = 0; i < j; i++) {
            if (tempString[i] == ':') {
                timeCount = 1;
            }
            if (timeCount == 1) {
                temp[size++] = tempString[i];
            }
        }
        temp[i] = '\0'; // 添加终止符，使 tempString 成为字符串
        return j;
    }else  {
        return 0;
    }

}

unsigned char udpSend(char *s ,int size)
{
    Serial_Printf("AT+CIPSEND=%d\r\n",size);
    delay_ms(50);
    Serial_SendString(s);
    return findOK();
}
//l:1adata
unsigned char saveToW25()
{
    uint8_t * data;
    unsigned char size=Serial_GetRxFlag();
    unsigned char temp;
    unsigned int i;
    if (size>0)
    {
        data=Serial_get_seril_buffer();
        for (i = 0; i < size; ++i) {
            if (data[i]=='l' && data[i+1]==':')
            {
                temp=data[i+2];
                W25Q64_SectorErase(0x000000);
                W25Q64_PageProgram(0,data+i+3,temp);
                Serial_SendString("ok");
            }else if (data[i]=='r' && data[i+1]==':')
            {
                temp=data[i+2];
                W25Q64_ReadData(0,data+3+i,temp);
                Serial_SendArray(data+i+3,temp);
            }
        }
    }
}
int main(void)
{

    Serial_Init();
		delay_init(72);
    W25Q64_Init();
			//W25Q64_SectorErase(0x000000);
 //   W25Q64_PageProgram(0,data,3);
    while(1)
    {
        saveToW25();
			delay_ms(100);
    }
}





/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

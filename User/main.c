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
#define W25QDelayTime 1000
static unsigned int writePosition;
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
void showCHAR(unsigned char* s,unsigned char length,unsigned char x,unsigned char y) {
    void showAHanji(unsigned char s[]);
    unsigned char i;
    for (i=0;i<length;i++)
    {
        Lcd_SetRegion(x+i*16,y,x+i*16+16,y+16);
        showAHanji(s+i*32);
    }
}
void showAHanji(unsigned char s[])
{
    unsigned char i,j,temp;

    for (i=0;i<32;i+=2)
    {
        temp=0x80;
        for ( j = 0; j < 8; ++j) {
            temp=temp>>1;
            if(s[i]& temp)
            {

                LCD_WriteData_16Bit(RED);

            }else
            {
                LCD_WriteData_16Bit(WHITE);
            }
        }
        temp=0x80;
        for ( j = 8; j < 17; ++j) {
            temp=temp>>1;
            if(s[i+1]& temp)
            {
                LCD_WriteData_16Bit(RED);
            } else
            {
                LCD_WriteData_16Bit(WHITE);
            }

        }
    }
}
unsigned char udpSend(char *s ,int size)
{
    Serial_Printf("AT+CIPSEND=%d\r\n",size);
    delay_ms(50);
    Serial_SendString(s);
    return findOK();
}
int  getPOsition(char* s)
{
    return (((unsigned char )s[0]-0x81)*192+((unsigned char )s[1]-0x40));
}
void showString(char *s)
{
    unsigned char data[16];
    unsigned int x,y=0;
    while (s!=0){
        int pos= getPOsition(s);
        W25Q64_ReadData(pos,data,16);
        showCHAR(data,1,x*16,y*8);
        x++;
        if (x==8){
            x=0;
            y++;
        }
        s+=2;
    }
}

//l:1adata
unsigned char saveToW25()
{
    uint8_t * data;
    uint16_t size=Serial_GetRxFlag();
    unsigned int temp;
    unsigned int i;
    if (size>0)
    {
        data=Serial_get_seril_buffer();
        for (i = 0; i < size; ++i) {
            if (data[i]=='l' && data[i+1]==':')
            {
                temp=data[i+2];
                if (temp==0)
                {
                    temp=0x100;
                }
                if (size<temp+2)
                {
                    delay_ms(W25QDelayTime);
                    size=Serial_GetRxFlag();
                    if (size<temp+2)
                    {
                        Serial_SendString("E:l1");//too short
                        break;
                    }
                }
                W25Q64_PageProgram(writePosition,data+i+3,temp);
                writePosition+=temp;
                Serial_SendString("lok");
            }else if (data[i]=='r' && data[i+1]==':')
            {
                //low first;
                temp=data[i+2];

                if (size<6)
                {
                    delay_ms(W25QDelayTime);
                    size=Serial_GetRxFlag();
                    if (size<6)
                    {
                        Serial_SendString("E:r1");//too short
                        break;
                    }
                }
                size=6+i;
                for ( ;  size>3+i ; size--) {
                    temp=temp<<8;
                    temp+=data[size-1];
                }
                size=data[i+2];
                if (size==0){
                    size=0x100;
                }
                W25Q64_ReadData(temp,data+3+i,size);
                Serial_SendArray(data+i+3,size);
            }else if (data[i]=='c' && data[i+1]==':')
            {//c:\01\02\03\04
                //  high first
                temp=0;
                if (size<5)
                {
                    delay_ms(W25QDelayTime);
                    size=Serial_GetRxFlag();
                    if (size<5)
                    {
                        Serial_SendString("E:c1");//too short
                        break;
                    }
                }
                size=5+i;
                for ( ;  size>2+i ; size--) {
                    temp=temp<<8;
                    temp+=data[size-1];
                }
                W25Q64_SectorErase(temp);
                Serial_Printf("cok");
            }else if (data[i]=='s' && data[i+1]==':')
            {
                //  high first
                temp=0;
                if (size<5)
                {
                    delay_ms(W25QDelayTime);
                    size=Serial_GetRxFlag();
                    if (size<5)
                    {
                        Serial_SendString("E:s1");//too short
                        break;
                    }
                }
                size=i+5;
                for ( ;  size>2+i ; size--) {
                    temp=temp<<8;
                    temp+=data[size-1];
                }
                writePosition=temp;
                Serial_Printf("sok");
            }else if(data[i]=='t' && data[i+1]==':')
            {
                data[0]=1;
                data[1]=2;
                data[2]=3;
                W25Q64_PageProgram(3,data,5);
                delay_ms(500);
                W25Q64_ReadData(3,data,5);
                Serial_Printf("%d %d %d",data[1],data[2],data[3]);
                Serial_SendString("start");
            }
        }
        Serial_Clear_Flag();
    }
}
int main(void)
{

    unsigned char data[5]={1,2,3,4,5};
    delay_init(72);
    Serial_Init();
    Lcd_Init();

    W25Q64_Init();
    Serial_SendString("start");
			//W25Q64_SectorErase(0x000000);
 //   W25Q64_PageProgram(0,data,3);
    while(1)
    {
            saveToW25();
			delay_ms(100);
        showString("こんにちは");
			
    }
}





/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

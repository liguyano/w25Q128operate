//////////////////////////////////////////////////////////////////////////////////	 
//???????????????¦Ä???????????????????????¦Ê????
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

unsigned char connect()
{
    Serial_SendString("AT+CIPSTART=\"UDP\",\"192.168.0.103\",7001,7001,0\r\n");
    Delay_ms(100);
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
        tempString[i] = '\0'; // 添加终止符，使 tempString 成为字符串
        return j;
    }else  {
        return 0;
    }

}

unsigned char udpSend(char *s ,int size)
{
    Serial_Printf("AT+CIPSEND=%d\r\n",size);
    Delay_ms(50);
    Serial_SendString(s);
    return findOK();
}
int main(void)
{
    char j=0;
   char tempString[50];
    char *a;
    unsigned char rowNow=1;
    unsigned int i=0;
    unsigned int size=0;
  SystemInit();	//System init.

  delay_init(72);//Delay init.
  Lcd_Init();
  Serial_Init();
    LCD_LED_SET;//Í¨¹ýIO¿ØÖÆ±³¹âÁÁ
    Lcd_Clear(GRAY0);
    Gui_DrawFont_GBK16(16,0,GREEN,GRAY0,"Test Start");
	//Serial_SendString("AT\r\n");
    Serial_SendString("AT+CIPCLOSE\r\n");
    Delay_ms(500);
 //    Delay_ms(50);
  //  Serial_SendString("AT+CIPSTART=\"UDP\",\"192.168.0.103\",7001,7001,0\r\n");
 //   Delay_ms(50);
 //   size=sprintf(fstring,"i:%d",i++);
  //  Serial_Printf("AT+CIPSEND=%d\r\n",size);
 //   Delay_ms(50);
   // Serial_SendString(fstring);
   // Delay_ms(50);

    i=connect();
    if (i==1)
    {
        Gui_DrawFont_GBK16(16,0,BLACK,GRAY0,"Connected");
        i=udpSend("first",5);
        if (i==1)
        {
            Gui_DrawFont_GBK16(16,0,BLACK,GRAY0,"Send ok");
        }else
        {   Gui_DrawFont_GBK16(16,0,BLACK,GRAY0,"Send not ok");
        }
    }else
    {
        Gui_DrawFont_GBK16(16,0,BLACK,GRAY0," not Connected");


    }

    while(1)
  {
      size=sprintf(tempString,"有難う%d",j++);
      udpSend(tempString,size);
      Delay_ms(50);
      if (recv(tempString)>0){
          Gui_DrawFont_GBK16(0,rowNow*16,RED,GRAY0,tempString);
          rowNow++;
          rowNow%=7;
        }
      Delay_s(1);
  }
}





/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

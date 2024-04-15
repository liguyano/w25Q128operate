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


int main(void)
{
    char rxDate;
    char tempString[50];
    unsigned int i=0;
    unsigned int j=0;
    unsigned long long timeCount=0;
  SystemInit();	//System init.

  delay_init(72);//Delay init.
  Lcd_Init();
  Serial_Init();
    LCD_LED_SET;//Í¨¹ýIO¿ØÖÆ±³¹âÁÁ
    Lcd_Clear(GRAY0);
    Gui_DrawFont_GBK16(16,0,GREEN,GRAY0,"AT\\r\\n");
	//Serial_SendString("AT\r\n");
    Serial_SendString("AT+CWJAP=\"TP-LINK_197B\",\"qwertyuiop\"\r\n");
    Delay_ms(10);
      Serial_SendString("AT+CIPSTART=\"UDP\",\"192.168.0.129\",7001,7001,0\r\n");  Delay_ms(10);
    Serial_SendString("AT+CIPSEND=10\r\n");  Delay_ms(10);
    Serial_SendString("hello world");

    while(1)
  {
        timeCount=0;
        while (Serial_GetRxFlag()!=1 && timeCount<100000){
              timeCount++;
              Delay_us(1);
        }
      if (timeCount>=100000)
      {
         // Gui_DrawFont_GBK16(0,0,GREEN,GRAY1,"timeout");
/*         j++;
          if (j>=1)*/
              if (i>0)
              {
                  tempString[i]=0;
                  Gui_DrawFont_GBK16(0,16,RED,GRAY0,tempString);
                  i=0;
              }
          continue;
      }
      rxDate=Serial_GetRxData();
      Serial_SendByte(rxDate);
      tempString[i++]=rxDate;
  }

}





/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

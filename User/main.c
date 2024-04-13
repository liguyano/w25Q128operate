#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

uint8_t RxData;
void AT_SendCmd(char * data){
    while (*data!=0)
    {
        Serial_SendByte(*data);
        data++;
        while (Serial_GetRxFlag()!=1);
    }
}
int main(void)
{int i = 0,size=0;
    char fstring[50];
	Serial_Init();

	while (1)
	{
        Serial_SendString("AT+CIPCLOSE\r\n");
        Delay_ms(50);
        Serial_SendString("AT+CIPSTART=\"UDP\",\"192.168.0.103\",7001,7001,0\r\n");
        Delay_ms(50);
        size=sprintf(fstring,"i:%d",i++);
        Serial_Printf("AT+CIPSEND=%d\r\n",size);
        Delay_ms(50);
        Serial_SendString(fstring);
        Delay_ms(50);
	}
}

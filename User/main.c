#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#include "Serial.h"
#include <stdio.h>
#include <stdarg.h>
/*
 * [18:27:10.934]AT+CWLAP\R\N
[18:27:10.938]AT+CWLA
[18:27:10.939]P ERROR
[18:27:15.694]AT+CWLAP\R\N
[18:27:15.697]AT+CWLAP
[18:27:18.25] OK
[18:27:36.63]AT+CWLAP\R\N
关闭串口
打开串口成功
[18:27:38.414]WIFI GOT IP
[18:27:39.782]AT+CWLAP\R\N
[18:27:39.786]AT+CWLAP
[18:27:41.388]+CWLAP:(4,"TP-LINK_197B",-35,"b8
[18:27:41.391]:f8:83:5e:19:7b",1,-19,0,4,4,7,0
[18:27:41.404]) +CWLAP:(4,"CU_C3TK",-45,"74:6
[18:27:41.406]9:4a:50:af:e8",3,-7,0,5,3,7,0)
[18:27:41.408]+CWLAP:(3,"DIRECT-8a-HP M232 Las
[18:27:41.411]erJet",-38,"aa:93:4a:7f:30:8a",6
[18:27:41.412],32767,0,4,4,7,1) +CWLAP:(4,"A4
[18:27:41.415]18",-89,"30:fc:68:1e:d0:14",1,-2
[18:27:41.417],0,4,4,7,0) +CWLAP:(4,"FOREA-88
[18:27:41.419]",-81,"88:40:3b:2c:a5:90",11,-29
[18:27:41.426],0,4,4,7,1) OK
[18:27:51.767]AT+CIPSEND=10\R\N
[18:27:51.777]AT+CIPSEND=10 link is
[18:27:51.782] not valid
[18:27:51.793] ERROR
[18:28:00.151]AT+CIPSTART="UDP","192.168.0.129",7001,7001,0\R\N
[18:28:00.166]AT+CIPSTART=
[18:28:00.172]"UDP","192.168.0.129
[18:28:00.177]",7001,7001,0 CONNECT OK
[18:28:04.158]AT+CIPSEND=10\R\N
[18:28:04.17]AT+CIPSEND=10
[18:28:04.175] OK >
[18:28:07.022]AT+CIPSEND=10\R\N
[18:28:07.036]=10 busy s... Re
[18:28:07.042]cv 10 byte
[18:28:07.048]s SEND OK
[18:28:13.686]AT+CIPSEND=10\R\N
[18:28:13.699]AT+CIPSEND=10
[18:28:13.705] OK >
[18:28:14.391]AT+CIPSEND=10\R\N
[18:28:14.404]=10 busy s...
[18:28:14.41]Recv 10 byte
[18:28:14.417]s SEND OK
[18:29:22.671]AT+CWLAP\R\N
[18:29:22.685]AT+CWLAP
[18:29:22.691]
[18:29:24.291]+CWLAP:(4,"TP-LINK_197B",-40,"b8
[18:29:24.305]:f8:83:5e:19:7b",1,-21,0,4,4,7,0
[18:29:24.319]) +CWLAP:(4,"A418",-89,"30:fc:6
[18:29:24.327]8:1e:d0:14",1,-4,0,4,4,7,0) +CW
[18:29:24.334]LAP:(4,"CU_C3TK",-57,"74:69:4a:5
[18:29:24.341]0:af:e8",4,-9,0,5,3,7,0) +CWLAP
[18:29:24.354]:(3,"DIRECT-8a-HP M232 LaserJet"
[18:29:24.361],-36,"aa:93:4a:7f:30:8a",6,32767
[18:29:24.369],0,4,4,7,1) OK
[18:29:31.942]AT+CIPCLOSE\R\N
[18:29:31.959]AT+CIPCLOSE CLO
[18:29:31.967]SED OK
[18:29:35.878]AT+CIPSTART="UDP","192.168.0.129",7001,7001,0\R\N
[18:29:35.896]AT+CIPSTART="UDP","1
[18:29:35.904]92.168.0.129
[18:29:35.917]",7001,7001,0 CONNECT OK
 +IPD,12:hiw are you
 * */
uint8_t RxData;
void AT_SendCmd(char * data){
    unsigned int i;
    while (*data!=0)
    {
        Serial_SendByte(*data);
        data++;
        i++;
    }
    for (int j = 0; j < i; ++j) {
        while (Serial_GetRxFlag()!=1);
    }
}
void AT_Sendf(char *format,...)
{
    unsigned int size;
    char String[100];
    va_list arg;
    va_start(arg, format);
    size= vsprintf(String, format, arg);
    va_end(arg);
    Serial_SendString(String);
    for (int j = 0; j < size; ++j) {
        while (Serial_GetRxFlag()!=1);
    }
}
unsigned char SendUdp(char *str,unsigned int size){
    char t='0';
    AT_Sendf("AT+CIPSEND=%d\r\n",size);
    AT_SendCmd(str);
   /* char expected_sequence[] = "SEND ";

    int i = 0;

    while (1) {
        while (!Serial_GetRxFlag()) {
            continue;
        }
        t = Serial_GetRxData();

        if (t == expected_sequence[i]) {
            i++;
            if (i == sizeof(expected_sequence) - 1) {
                // Reached the end of the expected sequence
                if (t == 'O') {
                    return 1;
                } else {
                    return -1;
                }
            }
        } else {
            // Mismatch; reset and start over
            i = 0;
            if (t == 'S') {
                // Keep processing if we've just matched 'S'
                i = 1;
            } else {
                continue;
            }
        }
    }*/

}
unsigned int recvUdp(char * s)
{
    char expected_sequence[] = "+IPD,",t;
    unsigned int size=0;
    int i = 0;
    while (1) {
        while (!Serial_GetRxFlag()) {
            continue;
        }
        t = Serial_GetRxData();

        if (t == expected_sequence[i]) {
            i++;
            if (i == sizeof(expected_sequence) - 1) {
                break;
            }
        } else {
            // Mismatch; reset and start over
            i = 0;
            if (t == '+') {
                // Keep processing if we've just matched 'S'
                i = 1;
            } else {
                continue;
            }
        }
    }
    t='0';
    //get size;
   do
    {
       size*=10;
       size+=t-'0';
        while (!Serial_GetRxFlag());
        t = Serial_GetRxData();
    } while (t!=':');
    for (i = 0; i< size; ++i) {
        while (!Serial_GetRxFlag());
        t = Serial_GetRxData();
        s[i]=t;
    }
    return size;
}
int main(void)
{unsigned int i = 0,size=0;
    char fstring[50];
	Serial_Init();
    AT_SendCmd("AT+CIPCLOSE\r\n");
    AT_SendCmd("AT+CIPSTART=\"UDP\",\"192.168.0.129\",7001,7001,0\r\n");
    SendUdp("hello",5);
	while (1)
	{
        size=recvUdp(fstring);
        SendUdp(fstring,size);
        Delay_s(3);
	}
}

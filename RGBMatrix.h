/*
Oscar Martinez
CS 281
Final Project
*/


#ifndef _RGBMATRIX_H_
#define _RGBMATRIX_H_

#include "Arduino.h"


void RGBMatrixInit();
void DispShowChar(char chr,unsigned char R,unsigned char G,unsigned char B,char bias);
void DispShowColor(unsigned char R,unsigned char G,unsigned char B);
void DispShowPic(unsigned char Index);



// ********************************** The base Function ************************
void _IO_Init();
void _LED_Init();
void _TC2_Init();

//************************ the timer2 operate functions zone *******************
ISR(TIMER2_OVF_vect);

// ****************** the LED Hardware operate functions zone *******************
void LED_SDA(unsigned char temp);
void LED_SCL(unsigned char temp);
void LED_RST(unsigned char temp);
void LED_LAT(unsigned char temp);
void LED_SLB(unsigned char temp);

// ******************* the LED datas operate functions zone ********************
void SetGamma();
void run(unsigned char k);
void open_line(unsigned char x);
void LED_Delay(unsigned char i);


#endif

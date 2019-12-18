/*
Oscar Martinez
CS 281
Final Project
*/


#ifndef _RGBMATRIX_H_
#define _RGBMATRIX_H_

#include "Arduino.h"

void RGBMatrixInit();

void drawChar(char chr,unsigned char R,unsigned char G,unsigned char B,char bias);
void drawMultChar(char chr,unsigned char R,unsigned char G,unsigned char B,char bias);

void drawPaddle1(int P1_y, int P1_x, unsigned char R,unsigned char G,unsigned char B,char bias);
void drawPaddle2(int P2_y, int P2_x, unsigned char R,unsigned char G,unsigned char B,char bias);
void drawBall(int B_y, int B_x, unsigned char R,unsigned char G,unsigned char B, char bias);

void gameStart(char bias);
void gameOver(char bias);

void dispShowColor(unsigned char R,unsigned char G,unsigned char B);
void dispShowPic(unsigned char Index);



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

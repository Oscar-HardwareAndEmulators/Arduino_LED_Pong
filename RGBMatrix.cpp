/*
Oscar Martinez
CS 281
Final Project
*/


#include "RGBMatrix.h"


//************************** define the operate commands ***********************
#define RST_BIT 0x04
#define LAT_BIT 0x02
#define SLB_BIT 0x01
#define SCL_BIT 0x40
#define SDA_BIT 0x80

//****************************** define the status *****************************
#define RST PORTC
#define LAT PORTC
#define SLB PORTC
#define SDA PORTD
#define SCL PORTD

//******************************** define the IO *******************************
#define open_line0	{PORTB=0x01;}
#define open_line1	{PORTB=0x02;}
#define open_line2	{PORTB=0x04;}
#define open_line3	{PORTB=0x08;}
#define open_line4	{PORTB=0x10;}
#define open_line5	{PORTB=0x20;}
#define open_line6	{PORTD=0x08;}
#define open_line7	{PORTD=0x10;}
#define close_all_line	{PORTD=0x00;PORTB=0x00;}

/*******************************************
define the data zone
*******************************************/
/*
//Test dots
unsigned char Tdots[8][8][3]=     {{{0,0,255},     {0,0,255},      {0,0,255},     {0,0,255},    {0,0,255},      {0,0,255},       {0,0,255},        {0,0,255}},
                                  {{0,165,255},   {0,165,255},    {0,165,255},   {0,165,255},  {0,165,255},    {0,165,255},     {0,165,255},      {0,165,255}},
                                  {{0,255,255},   {0,255,255},    {0,255,255},   {0,255,255},  {0,255,255},    {0,255,255},     {0,255,255},      {0,255,255}},
                                  {{0,255,0},     {0,255,0},      {0,255,0},     {0,255,0},    {0,255,0},      {0,255,0},       {0,255,0},        {0,255,0}},
                                  {{255,127,0},   {255,127,0},    {255,127,0},   {255,127,0},  {255,127,0},    {255,127,0},     {255,127,0},      {255,127,0}},
                                  {{255,0,0},     {255,0,0},      {255,0,0},     {255,0,0},    {255,0,0},      {255,0,0},       {255,0,0},        {255,0,0}},
                                  {{255,0,139},   {255,0,139},    {255,0,139},   {255,0,139},  {255,0,139},    {255,0,139},     {255,0,139},      {255,0,139}},
                                  {{255,255,255}, {255,255,255},  {255,255,255}, {255,255,255},{255,255,255},  {255,255,255},   {255,255,255},    {255,255,255}}
                                 };
*/

unsigned char dots[2][8][8][3] = {0};
//dots matrix
//[2]:Page:one for display, one for receive data
//[8]:Row:8 row in LED plane
//[8]:Column:8 column in one row
//[3]:Color:RGB data: 0 for Red; 1 for green, 2 for Blue
unsigned char Gamma_Value[3] = {10,63,63};
//Gamma correctly value, every LED plane is different.value range is 0~63
//[3]:RGB data, 0 for Red; 1 for green, 2 for Blue
unsigned char Page_Index = 0; // the index of buffer
unsigned char row = 0;//the value of row in LED plane, from 0~7
unsigned char column = 0;//the value of every row, from 0~7
unsigned char color = 0;//the value of every dots, 0 is Red, 1 is Green, 2 is Blue

unsigned char line = 0;

// *********************** define the extern data zone *************************
extern unsigned char font8_8[92][8];
extern unsigned char pic[4][8][8][3];


// =============================== IO Init Method ==============================
void _IO_Init()
// input output intion
{
  DDRD = 0xff; // set all pins direction of PortD
  DDRC = 0xff; // set all pins direction of PortC
  DDRB = 0xff; // set all pins direction of PortB

  PORTD = 0x00; // set all pins output is low of PortD
  PORTC = 0x00; // set all pins output is low of PortC
  PORTB = 0x00; // set all pins output is low of PortB
}


// =============================== LED Init Method =============================
void _LED_Init()
//
{
  LED_RST(1);
  LED_Delay(1);
  LED_RST(0);
  LED_Delay(1);
  LED_RST(1);
  LED_Delay(1);

  SetGamma();
  line = 0;
}


// ================================= TC2 Method ================================
void _TC2_Init()
// the timer2 operate functions zone
{
  TCCR2A |= (1 << WGM21) | (1 << WGM20);
  TCCR2B |= ((1<<CS22)|(1<<CS20));   // by clk/64
  TCCR2B &= ~((1<<CS21));   // by clk/64
  TCCR2A &= ~((1<<WGM21) | (1<<WGM20));   // Use normal mode
  ASSR |= (1<<AS2);       // Use internal clock - external clock not used in Arduino
  TIMSK2 |= (1<<TOIE2) | (0<<OCIE2B);   //Timer2 Overflow Interrupt Enable
  TCNT2 = 0xff;
  sei();
}


// ============================== RGBMatrix Method =============================
void RGBMatrixInit()
//Matrix init
{
	_IO_Init();           //Init IO
	_LED_Init();          //Init LED Hardware
	_TC2_Init();          //Init Timer/Count2
}


// ================================ ISR Method =================================
ISR(TIMER2_OVF_vect)
// the timer2 operate functions zone
{
  cli();
  TCNT2 = 0x64;      //flash a led matrix frequency is 100.3Hz,period is 9.97ms
  //TCNT2 = 0x63;      //flash a led matrix frequency is 99.66Hz,period is 10.034ms
    if(line > 7) line = 0;
    close_all_line;
    run(line);
    open_line(line);
    line++;
    sei();
}


// =============================== LED SDA Method ==============================
void LED_SDA(unsigned char temp)
// the LED Hardware operate functions zone
{
  if (temp)
    SDA|=SDA_BIT;
  else
    SDA&=~SDA_BIT;
}


// =============================== LED SCL Method ==============================
void LED_SCL(unsigned char temp)
//
{
  if (temp)
    SCL|=SCL_BIT;
  else
    SCL&=~SCL_BIT;
}


// =============================== LED RST Method ==============================
void LED_RST(unsigned char temp)
//
{
  if (temp)
    RST|=RST_BIT;
  else
    RST&=~RST_BIT;
}


// ================================ LED LAT Method =============================
void LED_LAT(unsigned char temp)
//
{
  if (temp)
    LAT|=LAT_BIT;
  else
    LAT&=~LAT_BIT;
}


// ================================ LED SLB Method =============================
void LED_SLB(unsigned char temp)
//
{
  if (temp)
    SLB|=SLB_BIT;
  else
    SLB&=~SLB_BIT;
}


// ============================== Set Gamma Method =============================
void SetGamma()
// the LED datas operate functions zone
{
  unsigned char i = 0;
  unsigned char j = 0;
  unsigned char k = 0;
  unsigned char temp = 0;
  LED_LAT(0);
  LED_SLB(0);
  for(k=0;k<8;k++)
    for(i = 3;i > 0 ;i--)
    {
      temp = Gamma_Value[i-1]<<2;
      for(j = 0;j<6;j++)
      {
        if(temp &0x80)
          LED_SDA(1);
        else
          LED_SDA(0);

        temp =temp << 1;
        LED_SCL(0);
        LED_SCL(1);
    }
  }
  LED_SLB(1);
}


// ================================ Run Method =================================
void run(unsigned char k)
//
{
  unsigned char i = 0;
  unsigned char j = 0;
  unsigned char p = 0;
  unsigned char temp = 0;
  LED_SLB(1);
  LED_LAT(0);
  for(i = 0;i<8;i++)
  {

    for(j=0;j<3;j++)
    {
      temp = dots[Page_Index][k][i][2-j];
      for(p=0;p<8;p++)
      {
         if(temp & 0x80)
           LED_SDA(1);
         else
           LED_SDA(0);

         temp = temp<<1;
         LED_SCL(0);
         LED_SCL(1);
       }
     }
  }
  LED_LAT(1);
  LED_LAT(0);
}


// ============================== Open Line Method =============================
void open_line(unsigned char x)
//
{
  switch (x)
  {
    case 0 :open_line0;
            break;
    case 1 :open_line1;
            break;
    case 2 :open_line2;
            break;
    case 3 :open_line3;
            break;
    case 4 :open_line4;
            break;
    case 5 :open_line5;
            break;
    case 6 :open_line6;
            break;
    case 7 :open_line7;
            break;
    default: close_all_line;
            break;
  }
}


// ============================== Draw Char Method =============================
void drawChar(char chr,unsigned char R,unsigned char G,unsigned char B,char bias)
// Parameter: chr :the latter want to show
//            R: the value of RED.   Range:RED 0~255
//            G: the value of GREEN. Range:RED 0~255
//            B: the value of BLUE.  Range:RED 0~255
//            bias: the bias of a letter in LED Matrix.Range -7~7
{
  unsigned char i,j,Page_Write,temp;
  unsigned char Char;
  unsigned char chrtemp[24] = {0};

  if ((bias > 8) || (bias < -8))
    return;


  Char = chr - 32;

  if(Page_Index == 0)
    Page_Write = 1;
  if(Page_Index == 1)
    Page_Write = 0;
  j = 8 - bias;
  for(i = 0;i< 8;i++)
  {
    chrtemp[j] = pgm_read_byte(&(font8_8[Char][i]));
    j++;
  }
  for(i = 0;i < 8;i++)
  {

    temp = chrtemp[i+8];
    for(j = 0;j < 8;j++)
    {
      if(temp & 0x80)
      {
        dots[Page_Write][j][i][0] = B;
        dots[Page_Write][j][i][1] = G;
        dots[Page_Write][j][i][2] = R;
      }
      else
      {
        dots[Page_Write][j][i][0] = 0;
        dots[Page_Write][j][i][1] = 0;
        dots[Page_Write][j][i][2] = 0;
      }
      temp = temp << 1;
    }
  }
  Page_Index = Page_Write;
}


// ============================== Draw Char Method =============================
void drawMultChar(char chr,unsigned char R,unsigned char G,unsigned char B,char bias)
// Parameter: chr :the latter want to show
//            R: the value of RED.   Range:RED 0~255
//            G: the value of GREEN. Range:RED 0~255
//            B: the value of BLUE.  Range:RED 0~255
//            bias: the bias of a letter in LED Matrix.Range -7~7
{

}


// ============================= Draw Paddle Method ============================
void drawPaddle1(int P1_y, int P1_x, unsigned char R,unsigned char G,unsigned char B,char bias)
// Parameter: chr :the latter want to show
//            R: the value of RED.   Range:RED 0~255
//            G: the value of GREEN. Range:RED 0~255
//            B: the value of BLUE.  Range:RED 0~255
//            bias: the bias of a letter in LED Matrix.Range -7~7
{
  unsigned char Page_Write;

  if ((bias > 8) || (bias < -8))
    return;

  if(Page_Index == 0)
    Page_Write = 1;
  if(Page_Index == 1)
    Page_Write = 0;

  dots[Page_Write][P1_y][P1_x][0] = B;
  dots[Page_Write][P1_y][P1_x][1] = G;
  dots[Page_Write][P1_y][P1_x][2] = R;

  Page_Index = Page_Write;

  //dots matrix
  // unsigned char dots[2][8][8][3] = {0};
  // [2]:Page:one for display, one for receive data
  // [8]:Row:8 row in LED plane
  // [8]:Column:8 column in one row
  // [3]:Color:RGB data: 0 for Red; 1 for green, 2 for Blue
}


// ============================= Draw Paddle Method ============================
void drawPaddle2(int P2_y, int P2_x, unsigned char R,unsigned char G,unsigned char B,char bias)
// Parameter: chr :the latter want to show
//            R: the value of RED.   Range:RED 0~255
//            G: the value of GREEN. Range:RED 0~255
//            B: the value of BLUE.  Range:RED 0~255
//            bias: the bias of a letter in LED Matrix.Range -7~7
{
  unsigned char Page_Write;

  if ((bias > 8) || (bias < -8))
    return;

  if(Page_Index == 0)
    Page_Write = 1;
  if(Page_Index == 1)
    Page_Write = 0;

  dots[Page_Write][P2_y][P2_x][0] = B;
  dots[Page_Write][P2_y][P2_x][1] = G;
  dots[Page_Write][P2_y][P2_x][2] = R;

  Page_Index = Page_Write;

  //dots matrix
  // unsigned char dots[2][8][8][3] = {0};
  // [2]:Page:one for display, one for receive data
  // [8]:Row:8 row in LED plane
  // [8]:Column:8 column in one row
  // [3]:Color:RGB data: 0 for Red; 1 for green, 2 for Blue
}


// ============================== Draw Ball Method =============================
void drawBall(int B_y, int B_x, unsigned char R,unsigned char G,unsigned char B,char bias)
// Parameter: chr :the latter want to show
//            R: the value of RED.   Range:RED 0~255
//            G: the value of GREEN. Range:RED 0~255
//            B: the value of BLUE.  Range:RED 0~255
//            bias: the bias of a letter in LED Matrix.Range -7~7
{
  unsigned char Page_Write;

  if ((bias > 8) || (bias < -8))
    return;

  dots[Page_Write][B_y][B_x][0] = B;
  dots[Page_Write][B_y][B_x][1] = G;
  dots[Page_Write][B_y][B_x][2] = R;
}


// ============================= Game Start Method =============================
void gameStart(char bias)
// Parameter: bias: the bias of a letter in LED Matrix.Range -7~7
{
  unsigned char i,j,Page_Write,temp;
  unsigned char Char;
  unsigned char chrtemp[24] = {0};
  unsigned char R = 0;
  unsigned char G = 255;
  unsigned char B = 0;

  if ((bias > 8) || (bias < -8))
    return;

  dispShowColor(R, G, B);
  LED_Delay(2);
  drawChar('3', R, G, B, bias);
  LED_Delay(1);
  drawChar('2', R, G, B, bias);
  LED_Delay(1);
  drawChar('1', R, G, B, bias);
}


// ============================== Game Over Method =============================
void gameOver(char bias)
// Parameter: bias: the bias of a letter in LED Matrix.Range -7~7
{
  unsigned char Page_Write;
  unsigned char R = 255;
  unsigned char G = 0;
  unsigned char B = 0;

  if ((bias > 8) || (bias < -8))
    return;

  dispShowColor(R, G, B);
  LED_Delay(2);
  drawChar('G', R, G, B, bias);
  LED_Delay(1);
  drawChar('G', 0, 255, 0, bias);  //Makes second G green to differentiate. GG = good game
}


// =========================== Disp Show Color Method ==========================
void dispShowColor(unsigned char R,unsigned char G,unsigned char B)
// Parameter:R: the value of RED.   Range:RED 0~255
//           G: the value of GREEN. Range:RED 0~255
//           B: the value of BLUE.  Range:RED 0~255
{
  unsigned char Page_Write,i,j;

  if(Page_Index == 0)
    Page_Write = 1;
  if(Page_Index == 1)
    Page_Write = 0;

  for (i = 0;i<8;i++)
    for(j = 0;j<8;j++)
    {
      dots[Page_Write][i][j][2] = R;
      dots[Page_Write][i][j][1] = G;
      dots[Page_Write][i][j][0] = B;
    }

  Page_Index = Page_Write;
}


// ============================ Disp Show Pic Method ===========================
void dispShowPic(unsigned char Index)
//Function:Fill a picture in LED matrix from FLASH
//Parameter:Index:the index of picture in Flash.
{
  unsigned char Page_Write,i,j;

  if(Page_Index == 0)
    Page_Write = 1;
  if(Page_Index == 1)
    Page_Write = 0;

  for (i = 0;i<8;i++)
  {

    for(j = 0;j<8;j++)
    {

      dots[Page_Write][i][j][0] = pgm_read_byte(&(pic[Index][i][j][2]));
      dots[Page_Write][i][j][1] = pgm_read_byte(&(pic[Index][i][j][1]));
      dots[Page_Write][i][j][2] = pgm_read_byte(&(pic[Index][i][j][0]));
    }
  }
  Page_Index = Page_Write;

}


// ============================== LED Delay Method =============================
void LED_Delay(unsigned char i)
//the other operate functions zone
{
  unsigned int y;
  y = i * 10;
  while(y--);
}

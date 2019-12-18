#include "LedControl.h"

#define POTPIN1 A0
#define POTPIN2 A1

byte A[5]={B01111111, B10001000, B10001000, B10001000, B01111111};
byte B[5]={B11111111, B01001001, B01001001, B00110110, B00000000};
byte C[5]={B01111110, B10000001, B10000001, B10000001, B01000010};
byte D[5]={B01111111, B01000001, B01000001, B00111110, B00000000};
byte E[5]={B11111110, B10010010, B10010010, B10010010, B00000000};
byte F[5]={B01111111, B01001000, B01001000, B01001000, B00000000};
byte G[5]={B01111110, B10000001, B10001001, B10001001, B01001110};
byte H[5]={B01111111, B00001000, B00001000, B00001000, B01111111};
byte I[5]={B00100001, B00100001, B00111111, B00100001, B00100001};
byte J[5]={B00000110, B00000001, B00000001, B00000001, B11111110};
byte K[5]={B11111111, B00011000, B00100100, B01000010, B10000001};
byte L[5]={B11111111, B00000001, B00000001, B00000001, B00000001};
byte M[5]={B11111100, B01000000, B00100000, B01000000, B11111100};
byte N[5]={B00111111, B00010000, B00001000, B00000010, B00111111};
byte O[5]={B01111110, B10000001, B10000001, B10000001, B01111110};
byte P[5]={B11111111, B10001000, B10001000, B10001000, B01110000};
byte Q[5]={B01111100, B10000010, B10000010, B10000011, B01111101};
byte R[5]={B11111111, B10001000, B10001100, B10001010, B01110001};
byte S[5]={B00110001, B01001001, B01001001, B01001001, B01000110};
byte T[5]={B00100000, B00100000, B00111111, B00100000, B00100000};
byte U[5]={B00111110, B00000001, B00000001, B00000001, B00111110};
byte V[5]={B00111100, B00000010, B00000001, B00000010, B00111100};
byte W[5]={B00111111, B00000010, B00000100, B00000010, B00111111};
byte X[5]={B00010001, B00001010, B00000100, B00001010, B00010001};
byte Y[5]={B11100000, B00010000, B00001111, B00010000, B11100000};
byte Z[5]={B00010001, B00010011, B00010101, B00011001, B00010001};
byte one[6]={B0001000, B00011000, B00001000, B00001000, B00001000, B00011100};
byte two[6]={B0011000, B0100100, B00001000, B00010000, B00100000, B00111100};
byte three[6]={B0011000, B0100100, B00001000, B00001000, B0100100, B0011000};
byte gg[6]={B01100110, B10001000, B10001000, B10111011, B10011001, B01100110};
byte intialPaddles[1]={B00011100};
byte erase[1]={B00000000};
  
LedControl lc = LedControl(10 ,9, 8, 1);
const int PADDLE_HEIGHT = 3;

const int ballSpeed = 300;

boolean gameRunning = true;

byte pad1;
byte pad2;

int ballX = 3;
int ballXDir = 1;

int ballY = random(1, 7);
int ballYDir = 1;

int newBallX;
int newBallY;

int p1X = 0;
int p1Y = 3;

int p2X = 7;
int p2Y = 3;

byte pad1Byte;
byte pad2Byte;


//=========================================================================================
//===================================== Set Up ============================================
//=========================================================================================
void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(POTPIN1, INPUT);
  pinMode(POTPIN2, INPUT);
  lc.shutdown(0,false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  gameStart();

  lc.setLed(0, ballY, ballX, HIGH);
  delay(500);
  lc.setColumn(0, p1X, intialPaddles[0]);
  delay(1000);
  lc.setColumn(0, p2X, intialPaddles[0]);
  delay(1000);
  lc.setColumn(0, p1X, erase[0]);
  lc.setColumn(0, p2X, erase[0]);
}


//=========================================================================================
//==================================== Main Loop ==========================================
//=========================================================================================
void loop()
{

  pad1 = map(analogRead(POTPIN1), 0, 1023, 8 - PADDLE_HEIGHT, 0);
  pad2 = map(analogRead(POTPIN2), 0, 1023, 8 - PADDLE_HEIGHT, 0);
  
  if(gameRunning)
  {
    if (millis() % ballSpeed < 2) 
    {
      paddleMovement();
      ballMovement();
      
    }
  }
  
  else
  {
    lc.clearDisplay(0);
    exit(0);
  }
}


//================================== Ball Movement ========================================
void ballMovement()
{
  newBallX = ballX + ballXDir;
  newBallY = ballY + ballYDir;
  
//  Serial.print(ballX);
//  Serial.print("  ");
//  Serial.println(p1X);

//  Serial.print(ballY);
//  Serial.print("  ");
//  Serial.println(p1Y);
//  
//  Serial.print(ballY);
//  Serial.print("  ");
//  Serial.println(p1Y+PADDLE_HEIGHT);
  
  if (hitPaddle(ballX, ballY, p1X, p1Y, PADDLE_HEIGHT)) 
  {
    ballXDir = -ballXDir;
    ballYDir = -ballYDir;
  }

  if (hitPaddle(ballX, ballY, p2X, p2Y, PADDLE_HEIGHT)) 
  {
    ballXDir = -ballXDir;
    ballYDir = -ballYDir;
  }

  else if (ballY >= 7 || ballY <= 0) 
  {
    ballYDir = -ballYDir;
  }  
  
  else if (ballX > 7 || ballX < 0) 
  {
    gameOver();
  }
  
  newBallX = ballX + ballXDir;
  newBallY = ballY + ballYDir;

  lc.setLed(0, ballY, ballX, LOW);
  lc.setLed(0, newBallY, newBallX, HIGH);
  
  ballX = newBallX;
  ballY = newBallY;
}


//================================= Paddle Movement =======================================
void paddleMovement()
{
 byte pad1Byte = byte(0xFF >> (8 - PADDLE_HEIGHT) << pad1);
 lc.setColumn(0, 0, pad1Byte);

 byte pad2Byte = byte(0xFF >> (8 - PADDLE_HEIGHT) << pad2);
 lc.setColumn(0, 7, pad2Byte);
}


//================================= Paddle Collision =======================================
boolean hitPaddle(int x, int y, int padX, int padY, int PADDLE_HEIGHT) 
{
  boolean result = false;
  if ((x == padX) && (y >= padY && y <= (padY + PADDLE_HEIGHT)))
  {
    result = true;
  }

  return result;  
}


//=============================== Game Start Method =======================================
void gameStart()
{
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      lc.setLed(0, i, j, HIGH);
      delay(10);
    }
  }
  
  lc.setRow(0, 1, three[0]);
  lc.setRow(0, 2, three[1]);
  lc.setRow(0, 3, three[2]);
  lc.setRow(0, 4, three[3]);
  lc.setRow(0, 5, three[4]);
  lc.setRow(0, 6, three[5]);
  delay(1000);
  lc.setRow(0, 1, two[0]);
  lc.setRow(0, 2, two[1]);
  lc.setRow(0, 3, two[2]);
  lc.setRow(0, 4, two[3]);
  lc.setRow(0, 5, two[4]);
  lc.setRow(0, 6, two[5]);
  delay(1000);
  lc.setRow(0, 1, one[0]);
  lc.setRow(0, 2, one[1]);
  lc.setRow(0, 3, one[2]);
  lc.setRow(0, 4, one[3]);
  lc.setRow(0, 5, one[4]);
  lc.setRow(0, 6, one[5]);
  delay(1000);
  lc.clearDisplay(0);
}


//================================ Game Over Method =======================================
void gameOver()
{
  lc.clearDisplay(0);
  lc.setRow(0, 1, gg[0]);
  lc.setRow(0, 2, gg[1]);
  lc.setRow(0, 3, gg[2]);
  lc.setRow(0, 4, gg[3]);
  lc.setRow(0, 5, gg[4]);
  lc.setRow(0, 6, gg[5]);
  delay(3000);
  gameRunning = false;
  lc.clearDisplay(0);
}

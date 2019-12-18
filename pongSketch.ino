#include "LedControl.h"


LedControl lc = LedControl(12,11,10,1);

const int PADDLE_HEIGHT = 2;

unsigned long ball_update;
unsigned long paddle_update;

boolean gameRunning = true;

int B_x = random(1, 7);
int B_xDir = 1;

int B_y = 4;
int B_yDir = 1;

int P1_x = 0;
int P1_y = 4;

int P2_x = 8;
int P2_y = 4;


//=========================================================================================
//===================================== Set Up ============================================
//=========================================================================================
void setup()
{
  pinMode(POTPIN, INPUT);
  lc.shutdown(0,false);
  gameStart(0);

  lc.setLed(0, B_y, B_x, HIGH);
  lc.setRow(0, B_y, byte(1 << (B_x)));
}


//=========================================================================================
//==================================== Main Loop ==========================================
//=========================================================================================
void loop()
{
  if(gameIsRunning)
  {
    ballMovement();
  }

  gameOver(0);
  exit(0);
}


//================================== Ball Movement ========================================
void ballMovement()
{
  int new_Bx = B_x + B_xDir;
  int new_By = B_y + B_yDir;

  if(new_Bx == 0 || new_Bx == 8) //check to see if hit side walls
  {
    gameOver();
  }

  if(new_By == 0 || new_By == 8) //check to see if hit top/bottom walls
  {
    B_yDir = -B_yDir;
    new_By += B_yDir + B_yDir;
  }

  if(new_Bx == P1_x && new_By >= P1_y && new_y <= P1_y + PADDLE_HEIGHT) // check to see if we hit paddle1     .
  {
    B_xDir = -B_xDir;
    new_Bx += B_xDir + B_xDir;
  }

  if(new_Bx == P2_x && new_By >= P2_y && new_y <= P2_y + PADDLE_HEIGHT) // check to see if we hit paddle2     .
  {
    B_xDir = -B_xDir;
    new_Bx += B_xDir + B_xDir;
  }

  //drawBall(new_By, new_Bx, R, G, B);
  B_x = new_Bx;
  B_y = new_By;
}


//================================= Paddle Movement =======================================
void paddleMovement()
{
  // drawPaddle1(P1_y, P1_x, R, G, B);
  // drawPaddle2(P2_y, P2_x, R, G, B);
}

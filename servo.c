#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "servo.h"
#include "Timer.h"

#define POSITION_FACTOR (2)

int servoState[2] = {0,0};
int servoPosition[2] = {0,0};


void moveServo(int servo, int position){
  
  //Keep the position in bounds (0 to 5)
  if(position < 0){
    position = 0;
  }
  if(position > 5){
    position = 5;
  }
  
  //Set position
  servoPosition[servo] = position;
  setDuty(servo, position * POSITION_FACTOR);
}

/*
* Moves the given servo right by the given amount
*@param servo, servo to move
*@param amount, amount to move right
*/
void moveServoRight(int servo, int amount){
  
  //Move the servo right, maxing out at position 5
  servoPosition[servo] += amount;
  if(servoPosition[servo] > 5){
    servoPosition[servo] = 5;
  }
  
  //Set servo to position
  setDuty(servo, servoPosition[servo] * POSITION_FACTOR);
}

/*
* Moves the given servo left by the given amount
*@param servo, servo to move
*@param amount, amount to move left
*/
void moveServoLeft(int servo, int amount){
  
  //Move the servo right, maxing out at position 5
  servoPosition[servo] -= amount;
  if(servoPosition[servo] < 0){
    servoPosition[servo] = 0;
  }
  
  //Set servo to position
  setDuty(servo, servoPosition[servo] * POSITION_FACTOR);
}


/*
* Handles the commands input from the user. First character controls Servo0,
* second character controls Servo1
*/
void handleCommand(char * command){
  
  //loop through twice, once for each servo
	for(int i = 0; i < 2; i++){
    
    //Parse the character into the separate commands
    //Pause Recipe Execution
    if(command[i] == 'P' || command[i] == 'p'){
      servoState[i] = 0;
    }
    //Continue Recipie Execution
    if(command[i] == 'C' || command[i] == 'c'){
      servoState[i] = 1;
    }
    //Begin or restart recipe
    if(command[i] == 'B' || command[i] == 'b'){
      servoState[i] = 1;
      //TODO
    }
    //Check if the servo is paused before allowing l or r commands
    if(!servoState[i]){
      //Move 1 position to the right
      if(command[i] == 'R' || command[i] == 'r'){
        moveServoRight(i, 1);
      }
      //Move 1 position to the left
      if(command[i] == 'L' || command[i] == 'l'){
        moveServoLeft(i, 1);
      }
    }
    //Otherwise, no-op
  }
}
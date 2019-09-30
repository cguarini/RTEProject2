#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "Timer.h"
#include "GPIOINP.h"
#include "POST.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char RxComByte = 0;
uint8_t buffer[BufferSize];
char str[100];
uint32_t results[10];
int buckets[100];
long lowerLimit = 1000;



//Handles the initial interface of the program, allowing the user
//to redefine the lowerLimit variable.
void runInterface(void){
  //Print introduction message
  putString("Welcome to the Timing Demo for Group 17!");
  char str[250];
  sprintf(str, "Default Lower Limit: %ld", lowerLimit);
  putString(str);
  putString("\n\rWould you like to set a new lower limit? (Y|N): ");

  
  //Get if user wants to change lower limit default
  char answer = 1;
  
  while((answer = getChar())){
    
    //Yes is selected, define a new lower limit
    if(answer == 'Y' || answer == 'y'){
      putChar(answer);

      while(1){//will loop until limit is correctly chosen
        putString("\r\nInput new lower limit (50-9950us): ");
        char * newLimit = getString();
        char * ptr;
        long i = strtol(newLimit, &ptr, 10);
        if(i >= 50 && i <= 9950){
          //limit correctly chosen, set limit and break loop
          lowerLimit = i;
          break;
        }
        else{
          //invalid limit, loop again
          putString("\r\nInvalid limit!");
        }
      }
      //break answer loop after correctly choosing response
      break;
    }
  
    
    //No is selected, proceed with default limit
     if(answer == 'N' || answer == 'n'){
       putChar(answer);
       putString("\n\r");
      break;
    }
    
  }
  

  sprintf(str, "Measuring with a lower limit of: %ld Upper limit of: %ld\n\r", lowerLimit, lowerLimit + 100);
  putString(str);
  
}



int main(void){

	System_Clock_Init(); // Switch System Clock = 80 MHz
	UART2_Init();
  initGPIOA();
  initTIM2(); //1MHz clock frequency
  
	//Main program loop
  while(1){
   
  }
}




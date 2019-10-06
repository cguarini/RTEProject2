#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "Timer.h"
#include "GPIOINP.h"
#include "servo.h"
#include "recipeProcessor.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char str[100];

void printWelcome(){
  putString("Welcome to the Project 2 Demo!\n\r");
  putString("List of Commands:\n\r");
  putString("\t(B | b) - Begin or Restart the Recipe\n\r");
  putString("\t(P | p) - Pause Recipe Execution\n\r");
  putString("\t(C | c) - Continue Recipe Execution\n\r");
  putString("\t(R | r) - Move 1 Position to the Right\n\r");
  putString("\t(L | l) - Move 1 Position to the Left\n\r");
  putString("\t(N | n) - No-op\n\r");
}

void printPrompt(){
  putString(">>");
}

int main(void){

	System_Clock_Init(); // Switch System Clock = 80 MHz
	UART2_Init();
  initGPIOA();
  initTIM2(); //1MHz clock frequency
  LED_Init();
  initTIM3();
	
	int strIndex = 0;
  
  printWelcome();
  printPrompt();
  
  int wait = getWait();
  
	//Main program loop, should always repeat
  while(1){
    uint32_t currCNT = getCNT();
		//Check for User Input While Looping
		if(hasCharacter(USART2)){
			char c = getChar();
			putChar(c);
			
			str[strIndex] = c;
			strIndex++;
			
			//Check for newline character
			if(c == 0x0D){
        putChar('\n');
        //null terminate string
				str[strIndex] = 0x00;
        
        //Parse and handle the command string
        handleCommand(str);
        
        //reset the string
        strIndex = 0;
        
        //Re-print the prompt
        printPrompt();
			}
			
		}
    
    if(getTIM3_TIF()){
      resetTIM3_TIF();
      
      wait--;
      if(wait <= 0){
        servo1_recipe_check();
        wait = getWait();
      }

      
    }
   
  }
}




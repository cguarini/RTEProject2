#include "Timer.h"
#include "UART.h"

#include <string.h>
#include <stdio.h>
char buf[100];


void initTIM2(){
    //TODO Enable TIM2 Clock
    RCC->APB1ENR1 |= (RCC_APB1ENR1_TIM2EN);
      
    //Disable Timer
    TIM2->CCER &= (~TIM_CCER_CC1E);

  
    //Load Prescaler
    TIM2->PSC = 79;
    //Set PWM Period
    TIM2->ARR = 20000;
    //Set duty cycle
    TIM2->CCR1 = 10000;
    TIM2->RCR = 0;
  
    TIM2->CR1 |= TIM_CR1_ARPE;
    //Generate event
    TIM2->EGR |= TIM_EGR_UG;

    
    //Enable output on CC1
    TIM2->CCMR1 &= (~TIM_CCMR1_CC1S);
    TIM2->CCMR1 &= (~TIM_CCMR1_OC1M);
    TIM2->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
    
    //Select edge of active transition
    TIM2->CCER &= (~TIM_CCER_CC1NP);
    TIM2->CCER &= (~TIM_CCER_CC1P);
    
    //Clear input prescaler
    //TIM2->CCMR1 &= (~TIM_CCMR1_IC1PSC);
  

    
    TIM2->CCMR1 |= TIM_CCMR1_OC1PE;
   
    
    //enable timer
    TIM2->CCER |= (TIM_CCER_CC1E);
    
    TIM2->CR1 |= TIM_CR1_CEN;

    
    
    
}

uint32_t getCCR1(){
    return TIM2->CCR1;
}

uint32_t getCNT(){
    TIM2->SR &= (~TIM_SR_CC1IF);
    return TIM2->CNT;
}



void TIM2_IRQHandler(void){
    sprintf(buf, "%d\n\r", getCNT());
    putString(buf);
}
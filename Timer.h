#include "stm32l476xx.h"

void initTIM2();
unsigned int getCNT();
unsigned int getCCR1();
void setDuty(int channel, float duty);

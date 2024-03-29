#ifndef RECIPENEW_H
#define RECIPENEW_H
#include "stm32l476xx.h"


// Defining Macros for the total servos and Recipe maximum length
#define SERVO_TOTAL 2
#define RECIPE_LIMIT 100

//Declaring the opcode values (bits 6 to 8)
#define MOV 0x20
#define DM 0xE0
//#define SWEEP 0x50
#define WAIT 0x40
#define DELAY 0x60
#define LOOP 0x80
#define END_LOOP 0xA0
#define RECIPE_END 0x00

//Setting the Opcode and Parameter masks to obtain the range of it operation
#define OPCODE_MASK 0xE0
#define PARAMETER_MASK 0x1f

//The status of the recipe
typedef enum 
{
	Running,
	Paused,
	Command_error,
	Nested_loop_error 
} recipe_status;




void executeRecipes(void);
int getWait(int servo);
int addToWaitCounter(int servo, int amount);
void restartRecipe(int servo);
void sweepServo(int servo);

#endif /* RECIPENEW_H */


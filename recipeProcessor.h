#ifndef RECIPENEW_H
#define RECIPENEW_H
#include "stm32l476xx.h"


// Defining Macros for the total servos and Recipe maximum length
#define SERVO_TOTAL 2
#define RECIPE_LIMIT 100

//Declaring the opcode values (bits 6 to 8)
#define MOV 0x20
#define WAIT 0x40
#define LOOP 0x80
#define END_LOOP 0xA0
#define RECIPE_END 0x00

//Setting the Opcode and Parameter masks to obtain the range of it operation
#define OPCODE_MASK 0xE0
#define PARAMETER_MASK 0x1f



// The position of the servos at any given time
//typedef enum
//{
//SERVO_POSITION0, 
//SERVO_POSITION1,
//SERVO_POSITION2, 
//SERVO_POSITION3, 
//SERVO_POSITION4,
//SERVO_POSITION5
//}servo_position;

//The status of the recipe
typedef enum 
{
	Running,
	Paused,
	Command_error,
	Nested_loop_error 
} recipe_status;




void servo1_recipe_check(void);
void waitServo(int servo, int wait_arg);
void loop_Servo(int servo, int loop_arg, int Recipe_index);
void End_loop_func(int servo,int Recipe_index );
int getWait();


#endif /* RECIPENEW_H */


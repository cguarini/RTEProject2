#include "recipeProcessor.h"
#include "servo.h"

//Initilaizing the values
int waitArg[] = {0,0}; //Stores the wait value for each servo recipe
int waitCounter[] = {0,0};//How many more "cycles" the instruction must wait before execution
int programCounter[] = {0,0};//Stores the instruction index each servo is on
int loopAddress[] = {0,0};//Stores the address of the loop instruction
int loopCounter[] = {0,0};//Stores the loop count of each servo
int endFlag[] = {0,0};//Marks the end of recipe execution
//Marks that the program encountered an error
//0 - no error
//1 - command error
//2 - nested loop error
int errorFlag[] = {0,0};


uint32_t recipe1[RECIPE_LIMIT] = {
	
WAIT+31,
MOV+0,
MOV+5,
MOV+0,
MOV+3,
LOOP+0,
MOV+1 ,
MOV+4,
END_LOOP,
MOV+0,
MOV+2,
WAIT+0,
MOV+3,
WAIT+0,
MOV+2,
MOV+3,
WAIT+31,
WAIT+31,
WAIT+31,
MOV+4, 
RECIPE_END 
};


uint32_t recipe2[RECIPE_LIMIT] = {
	
WAIT+31,
MOV+0,
MOV+5,
MOV+0,
MOV+3,
LOOP+0,
MOV+1 ,
MOV+4,
END_LOOP,
MOV+0,
MOV+2,
WAIT+0,
MOV+3,
WAIT+0,
MOV+2,
MOV+3,
WAIT+31,
WAIT+31,
WAIT+31,
MOV+4, 
RECIPE_END 
};

uint32_t * recipePrograms[] = { recipe1, recipe2};

int getWait(int servo){
	
	//Default to 0 if out of bounds
	if(servo > 1 || servo < 0){
		servo = 0;
	}
  
  return waitArg[servo];
}

void executeLoop(int servo, int count, int address){
	//Set the loop address to branch back to
	loopAddress[servo] = address;
	//Set the loop counter
	loopCounter[servo] = count;
}

// To check the recipe command at each iteration 
void executeRecipes()
{
	//Loop through both recipes, reading the instruction at the PC
	for(int i = 0; i < 1; i++){
		
		//Check if the recipe is paused or not
		//AND check if the instruction has waited long enough before execution
		//AND check that the program is not in an error state
		if(getServoState(i) && !waitCounter[i] && !errorFlag[i]){
			//No errors, execute the program
			
			//Fetch the instruction
			int PC = programCounter[i];
			uint32_t * recipe = recipePrograms[i];
			uint32_t instruction = recipe[PC];
			
			//Decode and execute the instruction
			uint32_t parameter = instruction & PARAMETER_MASK;
			switch(instruction  & OPCODE_MASK){
				
				//Move the servo to the desired position
				case MOV:											
					moveServo(i, parameter);	
					break;
						
				//Set time between instructions, parameter * 100ms
				case WAIT: 
					waitArg[i] = parameter; // for getting the count in wait command
					break;
					
				//Loop through the following instructions until END_LOOP the given amount
				case LOOP:
					//Check for nested loops
					if(loopCounter[i]){
						errorFlag[i] = 2;
					}
					executeLoop(i, parameter, PC);
					break;	

				//Marks the end of a loop function, either return to LOOP or exit loop
				case END_LOOP:
					if(loopCounter[i]){
						programCounter[i] = loopAddress[i];
						loopCounter[i]--;
					}
					break;

				//End of recipe, do nothing
				case RECIPE_END:
					endFlag[i] = 1;
					break;
			}
			
			//If the recipe has not ended, increment the PC
			if(endFlag[i]){
				programCounter[i]++;
			}
		}
		
		//Decrement or reset the wait cycles for this servo
		waitCounter[i]--;
		if(waitCounter[i] <= 0){
			waitCounter[i] = getWait(i);
		}
	}
}




	

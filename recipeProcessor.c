#include "recipeProcessor.h"
#include "servo.h"
#include "LED.h"

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
//int sweepstart;

uint32_t recipe1[RECIPE_LIMIT] = {
MOV+0,
WAIT+10,
DM+2,
WAIT+31,
MOV+1,
MOV+5,
MOV+1,
MOV+3,
LOOP+1,
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
MOV+1, 
RECIPE_END 
};


uint32_t recipe2[RECIPE_LIMIT] = {
MOV+3,
MOV+5,
MOV+2,
MOV+3,
LOOP+3,
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
MOV+5, 
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

//Reinitialize the given servo
void restartRecipe(int servo){
  programCounter[servo] = 0;
  endFlag[servo] = 0;
  errorFlag[servo] = 0;
  waitCounter[servo] = 0;
  waitArg[servo] = 0;
  loopCounter[servo] = 0;
  loopAddress[servo] = 0;
}

void setLEDs(){
  if(errorFlag[0] == 1){
    Red_LED_On();
    Green_LED_Off();
  }
  else if(errorFlag[0] == 2){
    Red_LED_On();
    Green_LED_On();
  }
  else if(getServoState(0)){
    Red_LED_Off();
    Green_LED_On();
  }
  else{
    Red_LED_Off();
    Green_LED_Off();
  }
}

int addToWaitCounter(int servo, int amount){
  waitCounter[servo] += amount;
  waitCounter[servo] += getWait(servo);
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
	for(int i = 0; i < 2; i++){
		
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
          if(parameter > 5){
            errorFlag[i] = 1;
          }
          else{
            moveServo(i, parameter);	
          }
					break;
          
         //DMOV moves the servo position twice

          case DM:		
          if(parameter > 3){
            errorFlag[i] = 1;
          }
          else{
            moveServo(i, parameter * 2);
          }
					break;
          
          //function to sweep the servo
          
//          case SWEEP:
//            sweepServo(i);
						
				//Set time between instructions, parameter * 100ms
				case DELAY: 
					waitArg[i] = parameter; // for getting the count in wait command
					break;
        
        //Wait the given amount of time before the next instruction
        case WAIT:
          waitCounter[i] += parameter;
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
          loopCounter[i]--;
					if(loopCounter[i] > 0){
						programCounter[i] = loopAddress[i];
					}
					break;

				//End of recipe, do nothing
				case RECIPE_END:
					endFlag[i] = 1;
					break;
			}
			
			//If the recipe has not ended, increment the PC
			if(!endFlag[i]){
				programCounter[i]++;
			}
		}
		
		//Decrement or reset the wait cycles for this servo
		waitCounter[i]--;
		if(waitCounter[i] < 0){
			waitCounter[i] = getWait(i);
		}
	}
  setLEDs();
}




	

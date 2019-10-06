#include "recipeProcessor.h"
#include "servo.h"



//Initilaizing the values
uint32_t Recipe_index =0;
uint8_t loop_arg =0;
uint8_t start_loop_ind =0;
uint8_t stop_loop_ind =0;
uint8_t loop_iter =0;
uint8_t servo_no =0;
uint8_t diff = 0;
uint8_t end_flag =0;
int wait_arg =0;
int i,j,k,l;

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

int getWait(){
  
  return wait_arg;
}

// To check the recipe command at each iteration 
void servo1_recipe_check()
{
//	  for(i=0; i<SERVO_TOTAL; i++)
//	  {
//			for(j=0; j<20; j++)
//			{
		if(end_flag == 1); 
			 // ->-> // have to end the recipe
				   
		 
		else {
		uint32_t position;
					switch(recipe1[Recipe_index]  & OPCODE_MASK)
						{
								case MOV:											
													position = (recipe1[Recipe_index] & PARAMETER_MASK); //for retrieving the parameter values
													moveServo(i =1, position);	
													Recipe_index = Recipe_index +1;
													break;
										
							 
								case WAIT: 
													wait_arg = recipe1[Recipe_index ] & PARAMETER_MASK; // for getting the count in wait command
													waitServo(i =1,wait_arg);
								          Recipe_index = Recipe_index +1;
													break;
											
								case LOOP:
													 loop_arg = recipe1[Recipe_index ] & PARAMETER_MASK; 
													 loop_Servo(i =1,loop_arg, Recipe_index);
								           Recipe_index = Recipe_index +1;
													 break;
													
			 
								case END_LOOP:
													 
													 End_loop_func(i=1,Recipe_index );
								          
								           Recipe_index = Recipe_index +1;
													 break;
			 
								case RECIPE_END:
														
														end_flag =1;
													  //Recipe_end_func(i=1);
													  break;
								
		
						}
						
		
				}
			
			
		}

	
// wait function
	
	void waitServo(int servo, int wait_arg)
  
	{
      //Keep the position in bounds (0 to 31)
      if(wait_arg < 0)
				{
					wait_arg = 0;
        }
				
			if(wait_arg > 31)
				{
					wait_arg = 31;
        }
//  delay_arg = (wait_arg*1000)+100;
//				if(delay_arg > 0){
//		 
//		while(--delay_arg); 
// 		
	}
	
void loop_Servo(int servo, int loop_arg, int Recipe_index)
  
	{
    servo_no = servo;
		loop_iter = loop_arg;
		start_loop_ind = Recipe_index;
		
	}
	
void End_loop_func(int servo,int Recipe_index )
	{
	
		servo_no = servo;
		stop_loop_ind = Recipe_index;
		
		if(&loop_iter >0)
		{
			diff = (stop_loop_ind - start_loop_ind)-1;
		 uint32_t index = (start_loop_ind) +1;
			
			for(l =0; l<loop_iter-1; l++) //no of times it needs to execute
			
				{
			for (k=0; k<diff-1; k++)  // runs the commands between start loop and end loop 
			{
          int position;
			
				      switch(recipe1[index])
							{
					      case MOV :		
									
													position = recipe1[index]*PARAMETER_MASK; //for retrieving the parameter values
													moveServo(i =1, position);	
													index = index +1;
													break;
										
							 
								case WAIT : 
													wait_arg = recipe1[index]*PARAMETER_MASK; // for getting the count in wait command
													waitServo(i =1,wait_arg);
								          index = index +1;
													break;
								
							}
					
					
				}
			
		}
		
	}
	
}
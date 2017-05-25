// PID4.3
// Author: Tariq Saiyad & Dan Dowden
// Code for AVC Q1 to Q3
//25/05/17

#include <stdio.h>
#include <time.h>
# include "E101.h"
#include <math.h>
#include <ctype.h>
//Assign variables here

 
// Testing statements
	bool camera = false;
	bool speed = false;
	bool PID_signal = false;
	bool direction =true;

// Quadrant change
	int quadrant =1;

//num
	float  numWhite=0;
	float numWhiteLeft=0;
	float numWhiteRight=0;
	float numWhiteTop=0;

//PID constants
	float current_error;
	float kp = 0.28;
	float ki =0.001;
	float kd = 0.13;
	float scale =50;

//Motor speeds
	float final_signal;
	float baseSpeed = 29;
	float min = 22;
	float max = 45;
	float left = 0;
	float right = 0;
  
//Method to control motors.
void move(int left, int right){
    set_motor(1,(int)right);   
    set_motor(2,(int)left);
    sleep1(0,500);
} 

void stop(){
    set_motor(1,0);
    set_motor(2,0);
}
 


//Networking Code
/*
void openGate(){
	char server[15] = {'1','3','0','.','1','9','5','.','6','.','1','9','6'};
	connect_to_server(server, 1024);
	char message[6] = {'P','l','e','a','s','e'};
	send_to_server(message);
	char password[6];
	receive_from_server(password);
	send_to_server(password);

}  /**/
int cameraRead(){
	
	//Camera control
	int whiteThreshold = 110;	
	int p = 0; // p is the actual number of how white the pixel is, 
	int s = 0; //s is the converted number of p to  1(white) or 0(black)
	
	//Error control
	float  error;
    	current_error =0;
	float previous_error =0;
	numWhite=0;
	numWhiteLeft=0;
	numWhiteRight=0;
	numWhiteTop=0;

	//PID signals
	float  proportional_signal;
	float derivative_signal;
	 final_signal;
	
	 take_picture();
	 
	// Left Vertical line			
	for(int i=0; i<240; i++){				    
		p = get_pixel(i,20,3);						
		if(p>whiteThreshold){   				
			numWhiteLeft++;																	
		}
	}					
	// Right vertical Line					
	for(int i=0; i<240; i++){					
		p = get_pixel(i,300,3);							
		if(p>whiteThreshold){   	
			numWhiteRight++;
		}	
	}	
	// Top Horizontal line			
	for(int i=0; i<320; i++){				    
		p = get_pixel(220,i,3);						
		if(p>whiteThreshold){   				
			numWhiteTop++;																	
		}
	}
	 	 
	// Middle Horizontal Line							
	for(int i=0; i<320; i++){					 					
		p = get_pixel(120,i,3);			
		if(p>whiteThreshold){   
			s = 1;
			numWhite++;												
		}else{
			s = 0;												
		}
		error = (i-160)*s; 
		current_error = (current_error + error);		
	}

	current_error = current_error/numWhite;
	if(camera){
		printf("Current error %.2f \n",current_error);
		printf("White left %.2f \n",numWhiteLeft);
		printf("White right %.2f \n", numWhiteRight);
		printf("White top %.2f \n", numWhiteTop);
		printf("White num: %.2f \n",numWhite);
		printf("\n\n");
	}

		sleep1(0,10000);
		//Proportional
		proportional_signal = current_error*kp;  
		//Derivative
		derivative_signal = (current_error-previous_error/0.01)*kd; // check
		previous_error = current_error;
		//final signal
		final_signal = (((proportional_signal+derivative_signal))/160)*scale;

		//set speeds
		left = baseSpeed - (final_signal);
		right = baseSpeed +(final_signal);
		
		// Maximum speed of motors		
		if (left>max)		{left = max;}
		if (left < -max)	{left = -max;}
		if (right>max)		{right = max;}
		if (right < -max)   {right = -max;}
		
		// Minimum speed of motors
		if (left>= 0 && left < min)		{left = min;}
		if (left <0 && left > -min)		{left = -min;}
		if (right>= 0 && right < min)	{right = min;}
		if (right <0 && right > -min)	{right = -min;}
		
	if(PID_signal){
		printf("Proportional signal: %.2f\n", proportional_signal );	
		printf("Derivative signal: %.2f\n",derivative_signal );		
		printf("final signal: %.2f \n", final_signal);
		printf("\n\n");
	}	
	if(speed){
		printf("left: %.2f \n",left);
		printf("right: %.2f \n", right);
		printf("\n\n");
	}	

						
return 0;  
}

void align(){
	cameraRead();
	while(current_error > 10 || current_error < -10){
		cameraRead();
		left = -30;
		right= 30;
		move(left,right);

	}
		sleep1(0,500000);
}

//Turn left
void turnLeft(){ //change
set_motor(1,26);
set_motor(2,-26);
sleep1(0,80000); 
} 

//Turn right
void turnRight(){ //change
set_motor(1,-26);
set_motor(2,26);
sleep1(0,80000);
} 


void setSpeed(int left, int right){
	float Spleft = left- final_signal;
	float Spright = right+ final_signal;
	move(Spleft,Spright);
} 

int main(){
	init();
	while(true){
		cameraRead();	
	//openGate();
	if(quadrant ==1){
		
		
			if(numWhite >0){
				if(direction){
					printf("Straight\n");
				}
				move(left,right);
				
				if(numWhiteLeft >33 && numWhiteRight >33 && numWhiteTop >310){
					quadrant = 3;
					printf("----------------Quadrant 3--------------\n");
					sleep1(0,400000);				
				}
			}
            else{
				if(direction){
					printf("Reverse\n");
				}
				left = -28;
				right =-28;
				move(left,right);
				sleep1(0,500);
			}

	}	
	if (quadrant ==3){
				
		// Turn left if left track
		if (numWhiteLeft == 0 && numWhiteRight >33 && numWhiteTop == 0 && numWhite <75){
			setSpeed(27,27);
			sleep1(0,400000); 
			if(direction){
				printf("Left\n");
			}
			turnLeft();

		//	align();
		}
		// Turn right if right track
		if (numWhiteLeft > 33 && numWhiteRight== 0 && numWhiteTop ==0&& numWhite <75){
			setSpeed(27,27);
			sleep1(0,100000);
			if(direction){
				printf("Right\n");
			}
			turnRight();

		//	align();
		}
		// Go straight if junction with track ahead
		if (numWhiteLeft > 33 && numWhiteRight > 33 && numWhiteTop < 80  && numWhite > 200){
			if(direction){
				printf("Onwards and forwards\n");
			}
			setSpeed(30,30);
			sleep1(0,50000);
		}
		// Turn left if junction with no track ahead
		if (numWhiteLeft >33  && numWhiteRight > 33 && numWhite >150 && numWhiteTop ==0){
			setSpeed(27,27);
			sleep1(0,400000);
			if(direction){
				printf("Left with junction\n");
			}
			turnLeft();

		//	align();	
		}
		// Keep moving if signal > 0
		if(numWhite >0){
			if(direction){
				printf("Forward\n");
			}
			setSpeed(30,30);
		}
		//Reverse if signal == 0 		
		if(numWhite==0){
			if(direction){
				printf("Back it up\n");
			}
			setSpeed(-28,-28);
			//sleep1(0,500);	 remove if necessary
		}
	}
	
	}			
									
return 0;
}

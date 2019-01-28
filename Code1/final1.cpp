
// Final code 1.0
// Authors: Tariq Saiyad & Dan Dowden
// Code for AVC Q1 to Q4
//31/05/17

#include <stdio.h>
#include <time.h>
#include "E101.h"
#include <math.h>
#include <ctype.h>

//Assign variables here

// Testing statements
	bool camera = false;
	bool speed = false;
	bool PID_signal = false;
	bool direction =true;
	bool sensor = false;
	bool errorCalc = false;

// Quadrant change
	int quadrant =1;

//num
	int r = 0;
	int g = 0;
	int b = 0;
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
	
//SENSOR
	
// Sensor variables
	int near_wall = 500;
	int sensorThresh = 650; 
	float leftSide = 680;
	int sensorMinThresh = 200;
	int front_sensor;
	int left_sensor;
	int right_sensor;
	float sensorScale = 1;
	
	float sensor_kp = 0.015;
	float sensor_ki =0.000;
	float sensor_kd = 0.0015;
	float sensor_scale =50;
	
	float sensor_error;
	float total_error;
	float previous_sensor_error;
	float sensor_derivative_signal;
	float sensor_integral_signal;
	float sensor_proportional_signal;
	float sensor_final_signal;
	
	float sensor_baseSpeed = 25;
  
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
void openGate(){   
 connect_to_server("130.195.6.196",1024);
   send_to_server("Please ");
   char message[24];
   receive_from_server(message);
   send_to_server(message);
}
int cameraRead(){
	//Camera control
	int whiteThreshold = 110;	
	int p = 0; // p is the actual number of how white the pixel is, 
	int s = 0; //s is the converted number of p to  1(white) or 0(black)
	
	r = 0;
	g = 0;
	b = 0;
	
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
	 
	 // RED line 
	for(int i=0; i<320; i++){				    
		r = get_pixel(120,i,0);
		g = get_pixel(120,i,1);	
		b = get_pixel(120,i,2);	
		
		if( r >120 && g < 30 && b < 30){
			printf(" ####################### Quadrant 4 ######################");
			quadrant ==4;
			sleep1(1,0);
		}						
		
	} 
	 
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
		derivative_signal = (current_error-previous_error/0.1)*kd;
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
	}	
	if(speed){
		printf("left: %.2f \n",left);
		printf("right: %.2f \n", right);
	}							
return 0;  
}

int sensorRead(){
	for(int i =0; i< 5;i++){
		front_sensor = front_sensor + read_analog(7);
		left_sensor = left_sensor + read_analog(6);
		right_sensor = right_sensor + read_analog(5);
	}
    // Calculates the average of 5 values 
		front_sensor = front_sensor/5;
		left_sensor = left_sensor/5;
		right_sensor = right_sensor/5; 
    
    //Error control for the sensor. 0 if it is in the center.
	    sensor_error = (float)left_sensor - (float)right_sensor+25;//+54;
		sleep1(0,10000);
		total_error = total_error + sensor_error;
		sensor_integral_signal = total_error * sensor_ki;
		sensor_proportional_signal= sensor_error* sensor_kp;
  		sensor_derivative_signal = (sensor_error-previous_sensor_error/0.1)*sensor_kd;
  		previous_sensor_error = sensor_error;
  	
		//final signal
		sensor_final_signal = ((sensor_proportional_signal+sensor_derivative_signal+ sensor_integral_signal));

		//set speeds
		left = sensor_baseSpeed + (sensor_final_signal);
      	right = sensor_baseSpeed -(sensor_final_signal)+7;
		
	if(sensor){    
		printf("Sensor error %.2f \n",sensor_error);
		printf("left %.2f \n",(float)left_sensor);
		printf("right %.2f \n", (float)right_sensor);
		printf("front %.2f \n\n", (float)front_sensor);
	}

	if(speed){
		printf("left speed: %.2f \n", (float)left);
        printf("right speed:  %.2f \n\n", (float)right);
	}

	if(errorCalc){
		printf("Error: %.2f \n", sensor_error);
		printf("Prop: %.2f \n", sensor_proportional_signal);
        printf("Deriv:  %.2f \n", sensor_derivative_signal);
		printf("Final : %.2f \n\n", sensor_final_signal);        
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
	set_motor(1,35);
	set_motor(2,-35);
	sleep1(0,80000);
} 


//Turn right
void turnRight(){ //change
	set_motor(1,-35);
	set_motor(2,35);
	sleep1(0,80000);
} 
void setSpeed(){
	left = 27 - final_signal;
	right = 27+ final_signal;

} 
//Turn left
void sensor_turnLeft(){ //change
	set_motor(1,43);
	set_motor(2,-43);
	sleep1(0,232000);
	stop(); 

}
//Turn right
void sensor_turnRight(){ //change
	set_motor(1,-43);
	set_motor(2,43);
	sleep1(0,210000);
	stop();

} 

int main(){
	init();
    openGate();
    sleep1(0,500000);
	while(true){
	cameraRead();
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
	kp =0.1;				
	// Turn left if left track
	if (numWhiteLeft == 0 && numWhiteRight >33 && numWhiteTop == 0 && numWhite <75){
		sleep1(0,130000);
		if(direction){
			printf("Left\n");
		}
		turnLeft();

	//	align();
	}
	// Turn right if right track
	if (numWhiteLeft > 33 && numWhiteRight== 0 && numWhiteTop ==0&& numWhite <75){
		sleep1(0,130000);
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
		left= 30;
		right =30;
		setSpeed();
		move(left,right);
		sleep1(0,50000);
	}
	// Turn left if junction with no track ahead
	if (numWhiteLeft >20  && numWhiteRight > 20 && numWhite >150 && numWhiteTop ==0){
		sleep1(0,130000);
		if(direction){
			printf("Left with junction\n");
		}
		turnLeft();

	//	align();
	
	}
	// Keep moving if signal > 0
	if(numWhite >0){
		if(direction){
			printf("Onwards and forwards\n");
		}
		setSpeed();
		move(left,right);
	}
	//Reverse if signal == 0 		
	else{
		cameraRead();
		if(direction){
			printf("Back it up\n");
		}
		left= -23;
		right =-28;
		move(left,right);
		sleep1(0,550);	
	}
}
	if(quadrant ==4){
	sensorRead();
	// RED line 
	for(int i=0; i<320; i++){				    
		r = get_pixel(120,i,0);
		g = get_pixel(120,i,1);	
		b = get_pixel(120,i,2);	
		
		if( r >120 && g < 30 && b < 30){
			openGate();
			sleep1(1,0);
		}							
	}

   //robot turns 90 degrees clockwise if there are walls ahead and left, with space right
  if ((front_sensor >= near_wall)  && (right_sensor <= sensorMinThresh)){
    	if(direction){
			printf("GO right\n");
		}
		stop();
		sleep1(0,5000);
		sensor_turnRight();
		move(28,30);
		sleep1(0,53000);  
    
  }
  
  //robot turns 90 degrees anticlockwise if there are walls ahead and right, with space left
  if ((front_sensor >= near_wall)&& (left_sensor <= sensorMinThresh)){
		if(direction){
			printf("GO left\n");
		}
		stop();
		sleep1(0,5000);
		sensor_turnLeft();
		move(28,30);  
		sleep1(0,50000);
  }
	
  if ((right_sensor >= sensorThresh) && (left_sensor <= sensorMinThresh) && front_sensor <= sensorMinThresh){
		if(direction){
			printf("Straight for now -----------------\n");
        }
		move(30,32);
		sleep1(0,5000000);  
  }	
  else{
		if(direction){
			printf("Keeeeep moving forward\n");
        }
		move(left,right);
  }
 
}
	
}												
return 0;
}

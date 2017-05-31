#include <stdio.h>
#include <time.h>
#include "E101.h"


//testing
bool speed = true;
bool sensor = false;
bool errorCalc = false;
bool direction = false;
// Sensor variables
int near_wall = 500;
int sensorThresh = 650; 
float leftSide = 680;
int sensorMinThresh = 200;
int front_sensor;
int left_sensor;
int right_sensor;
float sensorScale = 1;

//Sensor 
	
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

//Motor speeds
	
	float sensor_baseSpeed = 25;
	float sensor_min = 22;
	float sensor_max = 4;
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


//Turn left
void turnLeft(){ //change
  set_motor(1,43);
  set_motor(2,-43);
  sleep1(0,232000);
	stop(); 

} 


//Turn right
void turnRight(){ //change
  set_motor(1,-43);
  set_motor(2,43);
  sleep1(0,210000);
	stop();

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

//	if(left_sensor < 560){
  //              sensor_error = 300;
    //    }
      //  if(right_sensor < 560){
    //            sensor_error = -300;
    //    }   	
	//	sensor_error = (float)left_sensor - leftSide ;
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

int main(){

init();

while (true){
  sensorRead();

//if (((front_sensor < near_wall)) || ((sensor_error < 0) && (front_sensor < near_wall))){
  //robot moves forward if there are walls left and right, with space ahead OR wall on right with space ahead and left

 //move(left, right);

//}

   //robot turns 90 degrees clockwise if there are walls ahead and left, with space right
  if ((front_sensor >= near_wall)  && (right_sensor <= sensorMinThresh)){
    	if(direction){
	printf("GO right\n");
	}
	stop();
	sleep1(0,5000);
	turnRight();
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
	 turnLeft();
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
return 0;
}








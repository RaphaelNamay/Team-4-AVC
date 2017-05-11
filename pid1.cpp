#include <stdio.h>
#include <time.h>
# include "E101.h"

//Assign variables here

	//Motor speeds
	int left = 0;
	int right = 0;
	
	
	

//Method to control motors.
void move(int left, int right){
    set_motor(1, left);   
    set_motor(2, right);
    sleep1(5,0);
    stop(1);
    stop(2);
}  

//Networking Code
void openGate(){
		//insert IP
    connect_to_server(" ",1024);
    //insert password
    send_to_server(" ");
    char message[24];
    receive_from_server(message);
    send_to_server(message);
}

int cameraRead(){
	
	//Camera control
	
	int whiteThreshold = 120;	
	int p = 0; // p is the actual number of how white the pixel is, 
	int s = 0; //s is the converted number of p to  1(white) or 0(black)
	
	//Error control
	int error;
	int current_error = 0;
	int previous_error = 0;
	int numwhite = 0;
	
	//PID constants
	double kp = 0.5;
	double ki =0.001;
	double kd = 5.0;
	
	//PID signals
	int proportional_signal;
	//int derivative_signal;
	//int integral_signal;
	int final_signal;
	


	take_picture();
	 display_picture(0,5000000);
				for(int i=0; i<320; i++){
					
					p = get_pixel(120,i,3);
					
					
					if(p>whiteThreshold){   
					s = 1;
					numwhite++;	
																
					}
					
					else{
					s = 0;
							
					}
					printf("%d\n", s); 
					
					error = (i-160)*s; // if white is on left, error < 0, if on right, error > 0, if in center, error = 0
					
					
					current_error = (current_error + error);
					
					
				}
				current_error = current_error/numwhite;
				
				
				sleep1(0,100000);
				
				//Integral				
				//integral_signal = total_error*ki;
				//printf("Integral signal is: %d", integral_signal );
				//printf("current error is: %d", current_error);
				
				//Proportional
				proportional_signal = current_error*kp; // should it be current error? 
				printf("Proportional signal is: %d", proportional_signal ); 
				//which motor to turn?
				//left_v = (proportional_signal/(160*1*kp))*255;
				
				
				//Derivative
				//derivative_signal = (current_error-previous_error/0.1)*kd;
				//previous_error = current_error;
				//printf("Derivative signal is: %d", derivative_signal );
				
				
				
				//final signal
				//final_signal = (((proportional_signal/(160*1*kp)) + (derivative_signal/((320/0.1)*1*kd)))/2)*255;
				final_signal = (proportional_signal/(160*1*kp))*255;
				printf("final signal is: %d", final_signal);
				
				left = 80 + (final_signal/2);
				right = 80 - (final_signal/2);
				
				
				//current_error = 160-whiteMid;// how far away the center of the white line is from the center of the camera
				
				
				
return 0;  

}
int main(){
		
	init();
	
	
	cameraRead();
	printf("leftis: %d", left);
				move(left,right);
				
				
	
	return 0;
}


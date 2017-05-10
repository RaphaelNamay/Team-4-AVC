#include <stdio.h>
#include <time.h>

//Assign variables here

	//Motor speeds
	int left = 0;
	int right = 0;

//Method to control motors.
void move(int left, int right){
    set_motor(1, (int)left);
    set_motor(2, (int)right);
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
	int numWhite = 0;
	int numDark = 0; 
	int whiteThreshold = 120;
	int white1 = 320; 
	int whiteMid = 0; 
	int p = 0; // p is the actual number of how white the pixel is, 
	int s = 0; //s is the converted number of p to  1(white) or 0(black)
	
	//Error control
	int sum = 0;
	int value;
	int error;
	int current_error = 0;
	int previous_error = 0;
	int total_error = 0;s
	
	//PID constants
	int kp = 0.5;
	int ki = 0.001;
	int kd = 5.0;
	
	//PID signals
	int proportional_signal;
	int derivative_signal;
	int integral_signal;
	int final_signal;
	


	take_picture();
				for(int i=0; i<320; i++){
					p = get_pixel(i,120,3);
					
					
					if(p>whiteThreshold){   
					s = 1;
					numwhite++
						if(i<white1){
							white1 = i; //gives what pixel the first one to be white
						}
					}
					
					else{
					s = 0;
					numdark++;		
					}
					printf("%d\n", s); 
					
					error = (i-160)*s; // if white is on left, error < 0, if on right, error > 0, if in center, error = 0
					current_error = current_error + error;
				}
				Sleep(0,100000);
				
	                        total_error = total_error+current_error;
	
				//Integral				
				integral_signal = total_error*ki;
				printf("Integral signal is: %d", integral_signal );
				
				//Proportional
				proportional_signal = error*kp; // should it be current error? 
				printf("Proportional signal is: %d", proportional_signal ); 
				//which motor to turn?
				//left_v = (proportional_signal/(160*1*kp))*255;
				
				
				//Derivative
				derivative_signal = (current_error-previous_error/0.1)*kd;
				previous_error = current_error;
				printf("Derivative signal is: %d", derivative_signal );
				
				//final signal
				final_signal = proportional_signal + derivative_signal + integral_signal;
				
				
				
				/**if(current_error > 0 ) {
					//robot needs to turn right
					set_motor( int motor , int speed )
				}
				
				if(current_error < 0 ) {
					//robot needs to turn left
				}
				
				if(current_error = 0 ) {
					//robot needs to carry on forward
				}
				//chage these values from 0 to a larger number as the current error does not have to be completely perfect.
*/
	
				
return 0;  

}

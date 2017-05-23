
#include <stdio.h>
#include <time.h>
# include "E101.h"

//Assign variables here

	//Motor speeds
	float left = 0;
	float right = 0;
	float leftSt =0;
	float rightSt = 0;
	
//Algorithm change
	int quadrant = 1 ;	
	int intersection = 0;
	

//Method to control motors.
void move(int left, int right){
    set_motor(1,(int)right);   
    set_motor(2,(int)left);
    sleep1(0,1000);
} 

void moveSt(int leftSt, int rightSt){
	set_motor(1,(int)right);   
    set_motor(2,(int)left);
    sleep1(1,0);
}
//Turn left
void turnLeft(){ //change
	set_motor(1, 40);   
    set_motor(2, -40);
    sleep1(0,900000);
    set_motor(1, 0);   
    set_motor(2, 0);
} 

//Turn right
void turnRight(){ //change
	set_motor(1, -40);   
    set_motor(2, 40);
    sleep1(0,900000);
    set_motor(1,0);   
    set_motor(2,0);
} 

//Networking Code
//void openGate(){
		//insert IP
//    connect_to_server(" ",1024);
    //insert password
//    send_to_server(" ");
//    char message[24];
//    receive_from_server(message);
//   send_to_server(message);
//}

int cameraRead(){
	
	//Camera control
	
	int whiteThreshold = 100;	
	int p = 0; // p is the actual number of how white the pixel is, 
	int s = 0; //s is the converted number of p to  1(white) or 0(black)
	
	//Error control
	float  error;
        float current_error =0;
	float previous_error =0;
	float  numWhite=0;
	//int numDark = 0;
 	float  whiteProportion = 0; //proportion of white pixels in image
	numWhiteLeft = 0;
	numWhiteRight =0;
	
	//PID constants
	float kp = 0.02;
	float ki =0.001;
	float kd = 0.28;
	
	//PID signals
	float  proportional_signal;
	float derivative_signal;
	//int integral_signal;
	float final_signal;
	
	

	take_picture();
	 //display_picture(0,5000000);
				
				for(int i=0; i<240; i++){				     //for left	
					p = get_pixel(i,80,3);						
					if(p>whiteThreshold){   
						s = 1;
						numWhiteLeft++;																	
					}
				}					
							
				for(int i=0; i<240; i++){					//for Right
					p = get_pixel(i,240,3);							
					if(p>whiteThreshold){   
						s = 1;
						numWhiteRight++;
					}	
				}	 	 
							
				for(int i=0; i<320; i++){					 					
					p = get_pixel(120,i,3);			
					if(p>whiteThreshold){   
						s = 1;
						numWhite++;												
					}else{
						s = 0;
						//numDark++;
							
					}
					//printf("%d\n", s); 
					error = (i-160)*s; // if white is on left, error < 0, if on right, error > 0, if in center, error = 0
					current_error = (current_error + error);		
				}
			
				current_error = current_error/numWhite;
				printf("number of white %.2f \n",numWhite);
                printf("current error %.2f \n",current_error);
				whiteProportion = (numWhite/320.0)*100;
				if(whiteProportion >=80 && numWhiteLeft >=30 && numWhiteRight >=30){ // Subject to change
					quadrant = 3;
					//intersection ++;
				}
				sleep1(0,10000);				
				//Integral				
				//integral_signal = total_error*ki;
				//printf("Integral signal is: %d", integral_signal );
				//printf("current error is: %d", current_error);
				
				//Proportional
				proportional_signal = current_error*kp;  
//				printf("Proportional signal is: %d \n", proportional_signal ); 
				
				//Derivative
				derivative_signal = (current_error-previous_error/0.01)*kd;
				previous_error = current_error;
				printf("Derivative signal is: %.2f", derivative_signal );
				
				//final signal
				//final_signal = (((proportional_signal/(160*1*kp)) + (derivative_signal/((320/0.1)*1*kd)))/2)*255;
				//	final_signal = (proportional_signal/160)*60;
				final_signal = ((proportional_signal+derivative_signal)/160)*45;
				printf("final signal is: %.2f \n", final_signal);
				if(numWhite>0){
					left = 30 - (final_signal);
					right = 30 +(final_signal);
                }else{
					printf("---------------------------------------------------------------------");
					left=-25;
					right=-25;
                    sleep1(0,500);
                }
				printf("left is: %.2f \n",left);
				printf("right is: %.2f \n", right);
				//	printf("num whit is:  %d \n", numWhite);
				//	printf("Proportion:  %d \n", whiteProportion);
				//current_error = 160-whiteMid;// how far away the center of the white line is from the center of the camera
							
return 0;  
}


int main(){	
	init();
			//openGate();
			while(quadrant == 1){
						cameraRead();			
						move(left,right);									
			}	
			
			while(quadrant == 3){
						
						if (numWhiteLeft >=30 && numWhiteRight>=30){           //detects T intersection
							
							if(intersection == 0){ //go straight on
								cameraRead();			
						        moveSt(30,30);
						        sleep1(1,0);
						        intersection =1;	
							}
							if(intersection == 1){ // go left
								sleep1(0,200000);
								turnLeft();
								cameraRead();
								sleep1(1,0);
								
							}		
						
						}
											
						if (numWhiteLeft >=30 && numWhiteright<30){           //detects left turn
							
							if(leftTurn == 0){ //go straight on
								cameraRead();			
						        move(left,right);
						        leftTurn = 1;									
							}
							if(leftTurn == 1){  //go left	
								sleep1(0,100000);
								turnLeft();	
								sleep1(0,100000);						
							}
						}
											
						if (numWhiteLeft <30 && numWhiteRight>=30){           // detects right turn						
							if(rightTurn ==	0){ //turn right every time
								sleep1(0,100000);
								turnright();
								sleep1(0,100000);
							}
							
						if(numWhiteLeft <30 && numWhiteRight <30){            // straight line
						cameraRead();			
						move(left,right);	
						}
						
							
						}						
								
			}
			
			while(quadrant == 4){
				
			}
			
			
return 0;
}



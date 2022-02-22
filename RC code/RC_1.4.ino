/*
3pi+ radio control implementation notes:
1. Forward, reverse and spin functionalities of the robot are implemented separately to allow for changes after the prototype stage development.
2. All driving sensitivity changes should be made in the RC SENSITIVITY SETTINGS block

*/

#include <ServoInput.h>
#include <Pololu3piPlus32U4.h>
#include <stdlib.h>;


using namespace std;
using namespace Pololu3piPlus32U4;


Encoders encoders;
Motors motors;
//arduino pins for the two channels for the RC transmitter
const int throttlePin = 2;
const int steeringPin = 3;
ServoInputPin<throttlePin> throttle;
ServoInputPin<steeringPin> steering;

/*RC SENSITIVITY SETTINGS GUIDELINES:
 1. maxThrottleSpeed (values: 0 to 400): 
    Maximum speed of the motors from throttle input. Higher values = faster motors
    
 2. maxSteeringSpeed (recommended values: 10 to 50):
    Higher values will result in sharper turns for a given rotation of the steering wheel (i.e., higher values = smaller turning radius). 
 
 3. spinSpeed (values: multiplier for maxSteeringSpeed when the robot is spinning):
    Higher values will spin the the robot faster. A value of 1 will spin the robot at maxSteeringSpeed
 
 4. throttleDeadzone (recommended values: .1 to .15):
    Percentage from the center (neutral position) of the throttle that will be zeroed. This prevents the transmitter from repeatedly sending a mix of "forward" and "reverse" signals when the throttle is near the center.

 5. steeringDeadzone (recommended values: .1 to .15):
    Percentage from the center (neutral position) of the steering wheel that will be zeroed. This prevents the transmitter from repeatedly sending a mix of "left" and "right" signals when the steering wheel is near the center.


 *****ONLY USE ONE OF THE FOLLOWING TWO SETTINGS*****
  
 6. motorComp (recommended values: -3 to 3 depending on the robot motor)
    This setting compensates for the difference in motor speeds in some of the robots.
    Negative values will slow down the left motor by a constant value. 
    Positive values will speed up the left motor by a constant value.
    Example: Setting the value to 2 means that at a speed of 100 the 
    SET THIS TO ZERO IF NOT BEING USED

 7. motorCompFactor (recommended values: varies between robots)
    This settings compensates for the difference in motor speeds in some of the robots. 
    Values greater than 1 will speed up the right motor by that given factor. 
    Values less than 1 will slow down the right motor by that given factor.
    Example: Setting the value to 2 will run the right motor 2x faster than the left motor
    SET THIS TO 1 IF NOT BEING USED
 */

//RC SENSITIVITY SETTINGS:
const int maxThrottleSpeed = 300; //ZOOM ZOOM
const int maxSteeringSpeed = 30;   
const int spinSpeed = 10; //motors maximum speed when spinning will be 300 (maxSteeringSpeed * spinSpeed = 30 * 10 = 300)
const float throttleDeadzone = 0.10;  // 10% of middle is zeroed for the throttle
const float steeringDeadzone = 0.10; // 10% of middle is zeroed for steering
const int motorComp = 0; //no compensation for the left wheel
const int motorCompFactor = 1; //no compensation for the right wheel




void setup() {
  //initializes serial monitor
  Serial.begin(115200);
}


//forward function
void forward(int throttleSpeed, int steeringSpeed, int motorComp) {
  
    //stops both motors 
    if((throttleSpeed == 0) && (steeringSpeed == 0)){
        motors.setSpeeds(0,0);
    }
        
    //forward and steering left
    if ((throttleSpeed > 0) && (steeringSpeed < 0)){
        motors.setSpeeds(throttleSpeed + motorComp, (throttleSpeed + abs(steeringSpeed))*motorCompFactor);        
    }
    
    //forward and steering right
    if((throttleSpeed > 0) && (steeringSpeed > 0)){
        motors.setSpeeds(throttleSpeed + motorComp + abs(steeringSpeed), throttleSpeed*motorCompFactor);        
    }
    
    //forward only
    if((throttleSpeed > 0) && (steeringSpeed == 0)){
        motors.setSpeeds(throttleSpeed + motorComp, throttleSpeed*motorCompFactor);  
    }
    
    //serial monitor printouts for sensitivity testing
    Serial.println("Throttle:");
    Serial.println(throttleSpeed);
    Serial.println("Steering:");
    Serial.println(steeringSpeed);
    Serial.println("forward");
}


//reverse function
void reverse(int throttleSpeed,int steeringSpeed, int motorComp) {
  
    //stops both motors; redundant code since implemented in forward function; included here in case changes need to be made after prototype stage
    if((throttleSpeed == 0) && (steeringSpeed == 0)){
        motors.setSpeeds(0,0);
    }

    //steering left
    if ((throttleSpeed < 0) && (steeringSpeed < 0)){
         motors.setSpeeds((throttleSpeed - motorComp), (throttleSpeed - abs(steeringSpeed))*motorCompFactor);                
    }

    //steering right
    if ((throttleSpeed < 0) && (steeringSpeed > 0)){
         motors.setSpeeds((throttleSpeed - abs(steeringSpeed) - motorComp), throttleSpeed*motorCompFactor);         
    }
    
    //reverse only
    if((throttleSpeed < 0) && (steeringSpeed == 0)){
        motors.setSpeeds(throttleSpeed - motorComp, throttleSpeed*motorCompFactor);
    }
        
    //serial monitor printouts for sensitivity testing
    Serial.println("Throttle:");
    Serial.println(throttleSpeed);
    Serial.println("Steering:");
    Serial.println(steeringSpeed);
    Serial.println("reverse");    
}

//done
//spin function
void spin(int throttleSpeed, int steeringSpeed){
  
    //spin counterclockwise
    if ((steeringSpeed < 0)){
        motors.setSpeeds(steeringSpeed*spinSpeed,  (abs(steeringSpeed))*spinSpeed);                
    }

    //spin clockwise
    if ((steeringSpeed > 0)){
        motors.setSpeeds(steeringSpeed*spinSpeed, -steeringSpeed*spinSpeed);          
    }
    
    //serial monitor printouts for sensitivity testing
    Serial.println("Throttle:");
    Serial.println(throttleSpeed);
    Serial.println("Steering:");
    Serial.println(steeringSpeed);
    Serial.println("spin");
}



//Potential implementation for making the robot drift
/*
void reverseWithDrifting(int throttleSpeed,int steeringSpeed, int motorComp) {

    if((throttleSpeed == 0) && (steeringSpeed == 0)){//implement drifting here??(see immediately below reverse block)
        motors.setSpeeds(0,0);
    }
    
    if((throttleSpeed < 0) && (!(throttleSpeed == (-maxThrottleSpeed)))){
            //steering left
            if ((throttleSpeed < 0) && (steeringSpeed < 0)){
                motors.setSpeeds((throttleSpeed - motorComp), (throttleSpeed - abs(steeringSpeed)));
                
            }

            //steering right
            if ((throttleSpeed < 0) && (steeringSpeed > 0)){
                motors.setSpeeds((throttleSpeed - abs(steeringSpeed) - motorComp), (throttleSpeed));
                
            }
            //reverse only
            if((throttleSpeed < 0) && (steeringSpeed == 0)){
                motors.setSpeeds((throttleSpeed), (throttleSpeed));
                
                

            }
        }
    if(throttleSpeed == -maxThrottleSpeed){//implement drifting here see immediately below reverse block)
        if(steeringSpeed == 0){  
          motors.setSpeeds(0,0);
        }
      
        //spin left
        if ((steeringSpeed < 0)){
            motors.setSpeeds(-(abs(steeringSpeed)*spinPower),  abs(steeringSpeed)*spinPower);

        }

        //spin right
        if ((steeringSpeed > 0)){
            motors.setSpeeds(abs(steeringSpeed)*spinPower, -abs(steeringSpeed)*spinPower);
        }
    }
  
    Serial.println("Throttle:");
    Serial.println(throttleSpeed);
    Serial.println("Steering:");
    Serial.println(steeringSpeed);
    Serial.println("reverseWithDrifting");
  
}

*/


void loop() {
    //maps throttle and steering inputs to motor speed values
    //see above for deadzone settings
    int throttleSpeed = throttle.mapDeadzone(maxThrottleSpeed, -maxThrottleSpeed, throttleDeadzone);
    int steeringSpeed = steering.mapDeadzone(-maxSteeringSpeed, maxSteeringSpeed, steeringDeadzone);


    //function calls
    forward(throttleSpeed, steeringSpeed, motorComp);
    reverse(throttleSpeed, steeringSpeed, motorComp);
    spin(throttleSpeed, steeringSpeed);
    //reverseWithDrifting(throttleSpeed, steeringSpeed, motorComp);  
}

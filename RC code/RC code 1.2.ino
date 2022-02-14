#include <ServoInput.h>
#include <Pololu3piPlus32U4.h>
#include <stdlib.h>;

using namespace std;
using namespace Pololu3piPlus32U4;


Encoders encoders;
Motors motors;

const int throttlePin = 2;
const int steeringPin =3;
const int maxThrottleSpeed = 400; //maximum speed of the motors from throttle input
const int maxSteeringSpeed = 50; //maximum speed of motors from steering input
const float throttleDeadzone = 0.15;  // 15% of middle is zeroed for the throttle
const float steeringDeadzone = .15; // 10% of middle is zeroed for steering
const int motorComp = 2; //compensation for the slower motor

ServoInputPin<throttlePin> throttle;
ServoInputPin<steeringPin> steering;

void setup() {
  Serial.begin(115200);
}

//forward
//backward
//steering
//


void forward(int throttleSpeed, int steeringSpeed, int motorComp) {

    if((throttleSpeed == 0) && (steeringSpeed == 0)){//implement drifting here??(see immediately below reverse block)
        motors.setSpeeds(0,0);
    }
    
    
    // steering left
    if ((throttleSpeed > 0) && (steeringSpeed < 0)){
        motors.setSpeeds(throttleSpeed + motorComp, throttleSpeed + abs(steeringSpeed));
        
    }
    //steering right
    if((throttleSpeed > 0) && (steeringSpeed > 0)){
        motors.setSpeeds(throttleSpeed + motorComp + abs(steeringSpeed), throttleSpeed);
        
    }
    //forward only
    if((throttleSpeed > 0) && (steeringSpeed == 0)){
        motors.setSpeeds((throttleSpeed + motorComp), (throttleSpeed));
        
    }

    Serial.println(throttleSpeed);
    Serial.println(steeringSpeed);
}



void reverse(int throttleSpeed,int steeringSpeed, int motorComp) {

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
    if(throttleSpeed == -maxThrottleSpeed){//implement drifting here??(see immediately below reverse block)
        motors.setSpeeds(0,0);
    }

    Serial.println(throttleSpeed);
    Serial.println(steeringSpeed);
    
}

void spin(int throttleSpeed, int steeringSpeed){
  

    //steering left
    if ((steeringSpeed < 0)){
        motors.setSpeeds(-(abs(steeringSpeed)*2),  abs(steeringSpeed)*2);
                
    }

    //steering right
    if ((steeringSpeed > 0)){
        motors.setSpeeds(abs(steeringSpeed)*2, -abs(steeringSpeed)*2);
                
    }

    

    
}
/*
else if(throttleSpeed == -400){//implement drifting here??
        motors.setSpeeds(0,0);
        delay(250);
            // steering left
        if (steeringSpeed < 0){
            motors.setSpeeds(throttleSpeed + motorComp, throttleSpeed + abs(steeringSpeed));
        }
        //steering right
        else if(steeringSpeed > 0){
            motors.setSpeeds(throttleSpeed + motorComp + abs(steeringSpeed), throttleSpeed);
        }
    }
*/


void loop() {

    int throttleSpeed = throttle.mapDeadzone(maxThrottleSpeed, -maxThrottleSpeed, throttleDeadzone);
    int steeringSpeed = steering.mapDeadzone(-maxSteeringSpeed, maxSteeringSpeed, steeringDeadzone);

    
    forward(throttleSpeed, steeringSpeed, motorComp);
    reverse(throttleSpeed, steeringSpeed, motorComp);
    spin(throttleSpeed, steeringSpeed);

  




  //Serial.println(throttleSpeed);
  //Serial.println(steeringSpeed);
  Serial.print("\n");
 
  
}

//Test the following:
//1. braking
//2. spin speed
//3. steering sensitivity
//4. throttle sensitivity

//5. drifting!!!!

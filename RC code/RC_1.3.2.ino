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
const float throttleDeadzone = 0.10;  // 10% of middle is zeroed for the throttle
const float steeringDeadzone = 0.10; // 10% of middle is zeroed for steering
const int motorComp = 0; //compensation for the slower motor

ServoInputPin<throttlePin> throttle;
ServoInputPin<steeringPin> steering;

void setup() {
  Serial.begin(115200);
}



void forward(int throttleSpeed, int steeringSpeed, int motorComp) {

    if((throttleSpeed == 0) && (steeringSpeed == 0)){
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

    Serial.println("Throttle:");
    Serial.println(throttleSpeed);
    Serial.println("Steering:");
    Serial.println(steeringSpeed);
    Serial.println("forward");
}



void reverse(int throttleSpeed,int steeringSpeed, int motorComp) {

    if((throttleSpeed == 0) && (steeringSpeed == 0)){
        motors.setSpeeds(0,0);
    }
    
    if((throttleSpeed < 0)){
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
   
    Serial.println("Throttle:");
    Serial.println(throttleSpeed);
    Serial.println("Steering:");
    Serial.println(steeringSpeed);
    Serial.println("reverse");
    
}


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
            motors.setSpeeds(-(abs(steeringSpeed)*2),  abs(steeringSpeed)*2);

        }

        //spin right
        if ((steeringSpeed > 0)){
            motors.setSpeeds(abs(steeringSpeed)*2, -abs(steeringSpeed)*2);
        }
    }
  
    Serial.println(Throttle:);
    Serial.println(throttleSpeed);
    Serial.println(Steering:);
    Serial.println(steeringSpeed);
    Serial.println("reverseWithDrifting");
  
}
*/
   


void spin(int throttleSpeed, int steeringSpeed){
  
    //spin left
    if ((steeringSpeed < 0)){
        motors.setSpeeds(-(abs(steeringSpeed)*2),  abs(steeringSpeed)*2);
                
    }

    //spin right
    if ((steeringSpeed > 0)){
        motors.setSpeeds(abs(steeringSpeed)*2, -abs(steeringSpeed)*2);          
    }
  
    Serial.println("Throttle:");
    Serial.println(throttleSpeed);
    Serial.println("Steering:");
    Serial.println(steeringSpeed);
    Serial.println("spin");

}
  



void loop() {

    int throttleSpeed = throttle.mapDeadzone(maxThrottleSpeed, -maxThrottleSpeed, throttleDeadzone);
    int steeringSpeed = steering.mapDeadzone(-maxSteeringSpeed, maxSteeringSpeed, steeringDeadzone);

    
    forward(throttleSpeed, steeringSpeed, motorComp);
    reverse(throttleSpeed, steeringSpeed, motorComp);
    spin(throttleSpeed, steeringSpeed);
    //reverseWithDrifting(throttleSpeed, steeringSpeed, motorComp);
  

  




  //Serial.println(throttleSpeed);
  //Serial.println(steeringSpeed);
  
 
  
}

//Test the following:
//1. braking
//2. spin speed
//3. steering sensitivity
//4. throttle sensitivity

//5. drifting!!!!

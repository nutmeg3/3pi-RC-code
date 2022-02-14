#include <ServoInput.h>
#include <Pololu3piPlus32U4.h>
#include <stdlib.h>;

using namespace std;
using namespace Pololu3piPlus32U4;


Encoders encoders;
Motors motors;

const int throttlePin = 2;
const int steeringPin =3;
const int maxThrottleSpeed = 300; //maximum speed of the motors from throttle input
const int maxSteeringSpeed = 50; //maximum speed of motors from steering input
const float throttleDeadzone = 0.15;  // 15% of middle is zeroed for the throttle
const float steeringDeadzone = .10; // 10% of middle is zeroed for steering
const int motorComp = 0; //compensation for the slower motor

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
    // steering left
    if ((throttleSpeed > 0) && (steeringSpeed < 0)){
        motors.setSpeeds(throttleSpeed + motorComp, throttleSpeed + abs(steeringSpeed));
    }
    //steering right
    else if((throttleSpeed > 0) && (steeringSpeed > 0)){
        motors.setSpeeds(throttleSpeed + motorComp + abs(steeringSpeed), throttleSpeed);
    }
    //forward only
    else if((throttleSpeed > 0) && (steeringSpeed == 0)){
        motors.setSpeeds(throttleSpeed + motorComp, throttleSpeed);
    }
}


void reverse(int throttleSpeed,int steeringSpeed, int motorComp) {

    //steering left
    if ((throttleSpeed < 0) && (steeringSpeed < 0)){
        motors.setSpeeds(-(throttleSpeed + motorComp), -(throttleSpeed + abs(steeringSpeed)));
    }

    //steering right
    else if ((throttleSpeed < 0) && (steeringSpeed > 0)){
        motors.setSpeeds(-(throttleSpeed + motorComp), -(throttleSpeed + abs(steeringSpeed)));
    }
    //reverse only
    else if((throttleSpeed < 0) && (steeringSpeed > 0)){
        motors.setSpeeds(-(throttleSpeed + motorComp), -(throttleSpeed));
    }
}



void loop() {

    int throttleSpeed = throttle.mapDeadzone(-maxThrottleSpeed, maxThrottleSpeed, throttleDeadzone);
    int steeringSpeed = steering.mapDeadzone(-maxSteeringSpeed, maxSteeringSpeed, steeringDeadzone);

    
    forward(throttleSpeed, steeringSpeed, motorComp);
    reverse(throttleSpeed, steeringSpeed, motorComp);
    



  Serial.println(throttleSpeed);
  Serial.println(steeringSpeed);
  Serial.print("\n");
 
  
}







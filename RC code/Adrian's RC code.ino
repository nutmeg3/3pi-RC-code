#include <Wire.h>
#include <Pololu3piPlus32U4.h>
#include <PololuMenu.h> //May be needed
#include <ServoInput.h>

using namespace Pololu3piPlus32U4;
ServoInputPin<2> throttle;
ServoInputPin<3> steering;
Encoders encoders;
Buzzer buzzer;
Motors motors;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  float v = throttle.getAngle();  // get angle of servo (0 - 180)
  float angle = steering.getAngle();
  Serial.print(v);
  Serial.print("\n");
  if((3*angle -270) < 3 && (3*angle -270) > -3){
    motors.setSpeeds(-(3*v -270)*2.03,-(3*v - 270)*2);
  }
  else if((3*angle -270) < -3 && (3*v - 270) < -3){
    motors.setSpeeds(-(3*v -270)*1.015, -(3*angle - 270)*2);
  }
    else if((3*angle -270) > 3 && (3*v - 270) < -3){
    motors.setSpeeds(-(3*angle -270)*2.03, -(3*v - 270));
  }
  else if((3*angle -270) > 3 && (3*v - 270) < -3){
    motors.setSpeeds(-(3*angle -270)*2.03, -(3*v - 270));
  }
  else if((3*angle -270) > 3 && (3*v - 270) > 3){
    motors.setSpeeds(-(3*angle -270)*2.03, -(3*v - 270)*1);
  }
  else
  motors.setSpeeds((3*angle - 270)*1.015,-(3*angle - 270)*1);
   
  
}
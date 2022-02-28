#include <Pololu3piPlus32U4.h>
#include <stdlib.h>;


using namespace std;
using namespace Pololu3piPlus32U4;
 
Encoders encoders;
Buzzer buzzer;
Motors motors;
 
 
// global timer for the robot
 
unsigned long currentMillis;
unsigned long previousMillis;
 
//how often to check the encoders in milliseconds
const unsigned long PERIOD = 20;
 
//# of encoder counts per rotation of the motor shaft
const int ENCODER_COUNT_PER_ROTATION = 12;
//# of motor shaft rotations per rotation of the wheel
const float GEAR_RATIO = 29.86;
 
//wheel diameter in centimeters
const float WHEEL_DIAMETER = 3.2;
// wheel circumeference in centimeters
const float WHEEL_CIRCUMFERENCE = 10.0530;
 
//current encoder counts 
long encoderCountsLeft = 0;
long encoderCountsRight = 0;
 
//previous encoder counts
long previousEncoderCountsLeft = 0;
long previousEncoderCountsRight = 0;
 
 
//displacement of left wheel
float displacementLeftWheel = 0.0;
 
//displacement of right wheel
float displacementRightWheel = 0.0;
 
//discrepancy between the two wheels' displacement in centimeters in order to activate the closed loop
float displacementDiscrepancy = .02;

//if the displacment between the two motors has a discrepancy that is larger than the displacementDiscrepancy then this is the speed that is used to catch up the wheel with less displacement
//int motorAdjustment = displacementDiscrepancy*1000; 


/*
Things to test:
1. changing displacmentDiscrepancy
2. changing motorAdjustment
3. changing PERIOD
4. increasing precision of WHEEL_CIRCUMFERENCE
*/


// LOOOOOOOK HERE
void checkEncoders() {
  currentMillis = millis();
  
    if(currentMillis > previousMillis + PERIOD){
        encoderCountsLeft += encoders.getCountsAndResetLeft();
        encoderCountsRight += encoders.getCountsAndResetRight();
    
        //converts the encoder counts since the last time the encoder was checked into a displacement value in centimeters
        //displacementLeftWheel and displacementRightWheel are running total of displacements
        displacementLeftWheel += ((encoderCountsLeft - previousEncoderCountsLeft) / (ENCODER_COUNT_PER_ROTATION * GEAR_RATIO) * WHEEL_CIRCUMFERENCE);
        displacementRightWheel += ((encoderCountsRight - previousEncoderCountsRight) / (ENCODER_COUNT_PER_ROTATION * GEAR_RATIO) * WHEEL_CIRCUMFERENCE);


        if ((abs(displacementLeftWheel - displacementRightWheel)) > displacementDiscrepancy){
          float discrepancy = (abs(displacementLeftWheel - displacementRightWheel));
          int discrepancyMultiplier = 100;
            
            
            if (displacementLeftWheel < displacementRightWheel) {
                motors.setSpeeds(discrepancy * discrepancyMultiplier, 0);
            }

            if(displacementLeftWheel > displacementRightWheel){
                motors.setSpeeds(0, discrepancy * discrepancyMultiplier);
            }

        }

        


        if ((abs(displacementLeftWheel - displacementRightWheel)) < displacementDiscrepancy){
    
                motors.setSpeeds(0,0);
           
        }

        
        //Serial.println("outside");
        
            
            
        
  

    

        previousEncoderCountsLeft = encoderCountsLeft;
        previousEncoderCountsRight = encoderCountsRight;
        previousMillis = currentMillis;
  }


        //SERIAL MONITOR WHEEL DISPLACEMENT TRACKER
        Serial.println("Left Displacement:");
        Serial.println(displacementLeftWheel);
        Serial.println("Right Displacement:");
        Serial.println(displacementRightWheel);
        //Serial.println("inside checkEncoders");
        //Serial.println("discrepancy:");
        Serial.println((abs(displacementLeftWheel - displacementRightWheel)));
        //Serial.println("inside checkEncoders");
        //Serial.println(millis());
        
}

 
 
 
 

 
void setup() {
  Serial.begin(9600);
  delay(500);
  buzzer.play("c32");
}
 
void loop() {

  while((millis())< 10000){
    motors.setSpeeds(20,20);
    checkEncoders();
    Serial.println(millis());
  }

  
}
 


 
 

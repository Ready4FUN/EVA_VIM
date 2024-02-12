#include <AccelStepper.h>

// The X Stepper pins
#define FOREARM_DIR_PIN 27
#define FOREARM_STEP_PIN 26

#define RAIL_DIR_PIN 29
#define RAIL_STEP_PIN 28

#define ROTOR_DIR_PIN 25
#define ROTOR_STEP_PIN 24

#define SHOULDER_DIR_PIN 23
#define SHOULDER_STEP_PIN 22

#define GRABBER_DIR_PIN 31
#define GRABBER_STEP_PIN 30

#define SHOULDER_SWITCH 19
#define RAIL_SWITCH 20
#define FOREARM_SWITCH 21
 
// Define some steppers and the pins the will use
AccelStepper stepper1(AccelStepper::DRIVER, FOREARM_STEP_PIN, FOREARM_DIR_PIN);
AccelStepper rail(AccelStepper::DRIVER, RAIL_STEP_PIN, RAIL_DIR_PIN);
AccelStepper rotor(AccelStepper::DRIVER, ROTOR_STEP_PIN, ROTOR_DIR_PIN);
AccelStepper shoulder(AccelStepper::DRIVER, SHOULDER_STEP_PIN, SHOULDER_DIR_PIN);

AccelStepper grabber(AccelStepper::DRIVER, GRABBER_STEP_PIN, GRABBER_DIR_PIN);

char incomingByte;
bool rotorMove = false;
int rotorToGo = 0;
bool stepper1Move = false;
int stepper1ToGo = 0;
bool shoulderMove = false;
int shoulderToGo = 0;
bool grabberMove = false;
int grabberToGo = 0;
bool railMove = false;
int railToGo = 0;


void setup()
{  

    stepper1.setMinPulseWidth(30);
    stepper1.setMaxSpeed(3000.0);
    stepper1.setAcceleration(6000.0);
   // stepper1.moveTo(4000);

    rail.setMinPulseWidth(30);
    rail.setMaxSpeed(500.0);
    rail.setAcceleration(1000.0);
    //rail.moveTo(-5000);

    rotor.setMinPulseWidth(30);
    rotor.setMaxSpeed(500.0);
    rotor.setAcceleration(1000.0);
   // rotor.moveTo(5000);

    shoulder.setMinPulseWidth(30);
    shoulder.setMaxSpeed(500.0);
    shoulder.setAcceleration(1000.0);
    //shoulder.moveTo(-3000);

    grabber.setMinPulseWidth(30);
    grabber.setMaxSpeed(500.0);
    grabber.setAcceleration(1000.0);
    //grabber.moveTo(4000);
    Serial.begin(9600);

       
    
    //delay(5000);
}

void loop(){
while(Serial.available() > 0){
incomingByte = Serial.read();
if (incomingByte == '3'){
          stepper1ToGo = 0;
          stepper1ToGo -= 90000;
          stepper1.moveTo(stepper1ToGo);
          stepper1Move = true;
         }
        
  if (incomingByte == '4'){
          stepper1ToGo = 0;
          stepper1ToGo += 90000;
          stepper1.moveTo(stepper1ToGo);
          stepper1Move = true;
         }
     
  if (incomingByte == '1'){
          rotorToGo = 0;
          rotorToGo -= 90000;
          Serial.println(rotorToGo);
          rotor.moveTo(rotorToGo);
          rotorMove = true;
         }
        
  if (incomingByte == '2'){
          rotorToGo = 0;
          rotorToGo += 90000;
          Serial.println(rotorToGo);
          rotor.moveTo(rotorToGo);
          rotorMove = true;
         }
     

  if (incomingByte == '6'){
          shoulderToGo = 0;
          shoulderToGo += 90000;
          shoulder.moveTo(shoulderToGo);
          shoulderMove = true;
         }
       
  if (incomingByte == '5'){
          shoulderToGo = 0;
          shoulderToGo -= 90000;
          shoulder.moveTo(shoulderToGo);
          shoulderMove = true;
         }
         
  if (incomingByte == '7'){
          railToGo = 0;
          railToGo -= 90000;
          rail.moveTo(railToGo);
          railMove = true;
         }
    
  if (incomingByte == '8'){
          railToGo = 0;
          railToGo += 90000;
          rail.moveTo(railToGo);
          railMove = true;
         }

   if (incomingByte == 'A'){
          analogWrite(3,255);
          grabberToGo = 0;
          grabberToGo += 90000;
          grabber.moveTo(grabberToGo);
          grabberMove = true;
         }
  if (incomingByte == 'B'){
         analogWrite(3,255);
         grabberToGo = 0;
         grabberToGo -= 90000;
         grabber.moveTo(grabberToGo);
         grabberMove = true;
         }
   if (incomingByte == '9') {
      rotorMove = false;
      shoulderMove = false;
      railMove = false;
      grabberMove = false;
      stepper1Move = false;
      analogWrite(3,0);
   }

}
if(rotorMove){
    rotor.run();
    //Serial.print("run");
  } else {
    rotor.stop();
    //Serial.print("stop");
  }  
  if(shoulderMove){
    shoulder.run();
    //Serial.print("run");
  } else {
    shoulder.stop();
    //Serial.print("stop");
  } 
  if(railMove){
    rail.run();
    //Serial.print("run");
  } else {
    rail.stop();
    //Serial.print("stop");
  } 
  if(stepper1Move){
    stepper1.run();
    //Serial.print("run");
  } else {
    stepper1.stop();
    //Serial.print("stop");
  }
    if(grabberMove){
    grabber.run();
  } else {
    grabber.stop();
  }  
      
}

#include <AccelStepper.h>

// The X Stepper pins
#define FOREARM_DIR_PIN 23
#define FOREARM_STEP_PIN 22

#define RAIL_DIR_PIN 29
#define RAIL_STEP_PIN 28

#define ROTOR_DIR_PIN 25
#define ROTOR_STEP_PIN 24

#define SHOULDER_DIR_PIN 27
#define SHOULDER_STEP_PIN 26

#define GRABBER_DIR_PIN 31
#define GRABBER_STEP_PIN 30
 
// Define some steppers and the pins the will use
AccelStepper forearm(AccelStepper::DRIVER, FOREARM_STEP_PIN, FOREARM_DIR_PIN);
AccelStepper rail(AccelStepper::DRIVER, RAIL_STEP_PIN, RAIL_DIR_PIN);
AccelStepper rotor(AccelStepper::DRIVER, ROTOR_STEP_PIN, ROTOR_DIR_PIN);
AccelStepper shoulder(AccelStepper::DRIVER, SHOULDER_STEP_PIN, SHOULDER_DIR_PIN);

AccelStepper grabber(AccelStepper::DRIVER, GRABBER_STEP_PIN, GRABBER_DIR_PIN);


void setup()
{  

    forearm.setMinPulseWidth(30);
    forearm.setMaxSpeed(1000.0);
    forearm.setAcceleration(1000.0);
    forearm.moveTo(0);

    rail.setMinPulseWidth(30);
    rail.setMaxSpeed(1000.0);
    rail.setAcceleration(1000.0);
    rail.moveTo(0);

    rotor.setMinPulseWidth(30);
    rotor.setMaxSpeed(1000.0);
    rotor.setAcceleration(1000.0);
    rotor.moveTo(0);

    shoulder.setMinPulseWidth(30);
    shoulder.setMaxSpeed(1000.0);
    shoulder.setAcceleration(1000.0);
    shoulder.moveTo(0);

    grabber.setMinPulseWidth(30);
    grabber.setMaxSpeed(1000.0);
    grabber.setAcceleration(1000.0);
    grabber.moveTo(4000);

    //analogWrite(3,255);
    
    //delay(5000);
}

void loop()
{
    // Change direction at the limits
    if (rail.distanceToGo() == 0){
        rail.moveTo(-rail.currentPosition());
    }
    
    rail.run();
    

    if (forearm.distanceToGo() == 0){
        forearm.moveTo(-forearm.currentPosition());
    }
    
   forearm.run();
    
    if (rotor.distanceToGo() == 0){
        rotor.moveTo(-rotor.currentPosition());
    }
    
    rotor.run();

    if (shoulder.distanceToGo() == 0){
        shoulder.moveTo(-shoulder.currentPosition());
    }
    
    shoulder.run();

    if (grabber.distanceToGo() == 0){
        grabber.moveTo(-grabber.currentPosition());
    }
    
    grabber.run();
    
}

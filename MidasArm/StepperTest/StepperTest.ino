#include <AccelStepper.h>

// The X Stepper pins
#define STEPPER1_DIR_PIN 12
#define STEPPER1_STEP_PIN 13

#define RAIL_DIR_PIN 10
#define RAIL_STEP_PIN 11

#define ROTOR_DIR_PIN 8
#define ROTOR_STEP_PIN 9

#define SHOULDER_DIR_PIN 6
#define SHOULDER_STEP_PIN 7

#define GRABBER_DIR_PIN 4
#define GRABBER_STEP_PIN 5
 
// Define some steppers and the pins the will use
AccelStepper stepper1(AccelStepper::DRIVER, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN);
AccelStepper rail(AccelStepper::DRIVER, RAIL_STEP_PIN, RAIL_DIR_PIN);
AccelStepper rotor(AccelStepper::DRIVER, ROTOR_STEP_PIN, ROTOR_DIR_PIN);
AccelStepper shoulder(AccelStepper::DRIVER, SHOULDER_STEP_PIN, SHOULDER_DIR_PIN);

AccelStepper grabber(AccelStepper::DRIVER, GRABBER_STEP_PIN, GRABBER_DIR_PIN);


void setup()
{  

    stepper1.setMinPulseWidth(30);
    stepper1.setMaxSpeed(10000.0);
    stepper1.setAcceleration(5000.0);
    stepper1.moveTo(4000);

    rail.setMinPulseWidth(30);
    rail.setMaxSpeed(10000.0);
    rail.setAcceleration(5000.0);
    rail.moveTo(-5000);

    rotor.setMinPulseWidth(30);
    rotor.setMaxSpeed(10000.0);
    rotor.setAcceleration(3000.0);
    rotor.moveTo(5000);

    shoulder.setMinPulseWidth(30);
    shoulder.setMaxSpeed(10000.0);
    shoulder.setAcceleration(3000.0);
    shoulder.moveTo(-3000);

    grabber.setMinPulseWidth(30);
    grabber.setMaxSpeed(10000.0);
    grabber.setAcceleration(5000.0);
    grabber.moveTo(4000);
    
    //delay(5000);
}

void loop()
{
    // Change direction at the limits
    
    if (stepper1.distanceToGo() == 0){
        stepper1.moveTo(-stepper1.currentPosition());
    }
    
    stepper1.run();

    if (rail.distanceToGo() == 0){
        rail.moveTo(-rail.currentPosition());
    }
    
    rail.run();

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

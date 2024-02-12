#include <AccelStepper.h>

// The X Stepper pins
// The X Stepper pins
#define FOREARM_DIR_PIN 27
#define FOREARM_STEP_PIN 26

#define RAIL_DIR_PIN 23
#define RAIL_STEP_PIN 22

#define ROTOR_DIR_PIN 25
#define ROTOR_STEP_PIN 24

#define SHOULDER_DIR_PIN 29
#define SHOULDER_STEP_PIN 28

#define GRABBER_DIR_PIN 31
#define GRABBER_STEP_PIN 30

#define SHOULDER_SWITCH 19
#define RAIL_SWITCH 20
#define FOREARM_SWITCH 21

byte    zeroSwitch[3] = {SHOULDER_SWITCH, FOREARM_SWITCH, RAIL_SWITCH};
// Define some steppers and the pins the will use
AccelStepper    forearm(AccelStepper::DRIVER, FOREARM_STEP_PIN, FOREARM_DIR_PIN);
AccelStepper    rail(AccelStepper::DRIVER, RAIL_STEP_PIN, RAIL_DIR_PIN);
AccelStepper    rotor(AccelStepper::DRIVER, ROTOR_STEP_PIN, ROTOR_DIR_PIN);
AccelStepper    shoulder(AccelStepper::DRIVER, SHOULDER_STEP_PIN, SHOULDER_DIR_PIN);
AccelStepper    grabber(AccelStepper::DRIVER, GRABBER_STEP_PIN, GRABBER_DIR_PIN);
AccelStepper    Motors[5] = {shoulder, forearm, rail, grabber, rotor};

 
// Define some steppers and the pins the will use
AccelStepper stepper1(AccelStepper::DRIVER, FOREARM_STEP_PIN, FOREARM_DIR_PIN);


int mapLength = 4; // длина массива
int k = 0;  // текущее положение в массиве

//motion map 
long wayRotor[8] =       {0,        0,   2500,     0};
long wayShoulder[8] =    {15000,    0,      0,     0};
long wayForearm[8] =     {-9500,    0,      0,     0};

void setup()
{  

    Motors[0].setPinsInverted(true);

    for (int i = 0; i < 5; i++) {
        Motors[i].setMinPulseWidth(30);
        Motors[i].setMaxSpeed(500);
        Motors[i].setAcceleration(1000);
        Motors[i].moveTo(-1000000);
    } 
        
    Motors[3].setMaxSpeed(3000);
    Motors[3].setAcceleration(6000);

    Motors[4].setMaxSpeed(3000);
    Motors[4].setAcceleration(6000);
}

void loop(){
  if(!Motors[4].distanceToGo() && !Motors[0].distanceToGo() && !Motors[1].distanceToGo()) {
    if (k >= mapLength) {
      k = 0;
      return;
    }
    Motors[4].moveTo(wayRotor[k]);
    Motors[0].moveTo(wayShoulder[k]);
    Motors[1].moveTo(wayForearm[k]);

    k++;
  }

  Motors[4].run();
  Motors[0].run();
  Motors[1].run();
}

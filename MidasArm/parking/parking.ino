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

void setup()
{  

    Serial.begin(9600);

    pinMode(RAIL_SWITCH,INPUT_PULLUP);
    pinMode(FOREARM_SWITCH,INPUT_PULLUP);
    pinMode(SHOULDER_SWITCH,INPUT_PULLUP);

    
    /*
    forearm.setMinPulseWidth(30);
    forearm.setMaxSpeed(1000.0);
    forearm.setAcceleration(1000.0);
    forearm.moveTo(0);

    Motors[3].setMinPulseWidth(30);
    Motors[3].setMaxSpeed(1000.0);
    Motors[3].setAcceleration(1000.0);
    Motors[3].moveTo(-5000);

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
    grabber.moveTo(0);
    */
    //analogWrite(3,255);
    
    //delay(5000);

    parking();
}

void loop()
{
    // Change direction at the limits
    /*
    rail.run();
    forearm.run();
    rotor.run();
    shoulder.run();
    grabber.run();
    */
    //Motors[3].run();

}

void parking() 
{
    Serial.println("parking");

    Motors[2].setPinsInverted(true);

    for (int i = 0; i < 5; i++) {
        Motors[i].setMinPulseWidth (30);
        Motors[i].setMaxSpeed (2000);
        Motors[i].setAcceleration (3000);
        Motors[i].moveTo(-40000);
    }  

    Serial.println(Motors[0].currentPosition());
    Serial.println(Motors[0].targetPosition());
    Serial.println(Motors[0].distanceToGo());
    
    while(Motors[0].distanceToGo() || Motors[1].distanceToGo() || Motors[2].distanceToGo()) {
        for (int i = 0; i < 3; i++) {
            if(digitalRead(zeroSwitch[i]) != 0){
                Motors[i].run();
            } else {
                Motors[i].moveTo(0);
                Motors[i].setCurrentPosition(0);
            }
        }
    } 

    for (int i = 3; i < 5; i++){
        Motors[i].moveTo(0);
        Motors[i].setCurrentPosition(0);
    }
        

    Serial.println("parking done");
}



/*
class Scan {
    private:
    int N = 2; // микрошаг
    int k = 0;  // текущая целевая точка

    long wayRotor[] =       {0,     50};
    long wayShoulder[] =    {50,    0};
    long wayForearm[] =     {50,    0};

    public:
    void scan(){
        for(int i = 0; i <= 2, i++){
            
            if(!Motors[1].distanceToGo() && !Motors[2].distanceToGo() && !Motors[4].distanceToGo()) {
                k++;
                if (k >= N) {
                    k = 0;
                    Serial.println("new Snake begin");
                }
                Motors[0].moveTo(wayRotor[k]*100);
                Motors[1].moveTo(wayShoulder[k]*100);
                Motors[2].moveTo(wayForearm[k]*100);
                }
            
            Motors[i].run();
        }
    }
};
*/
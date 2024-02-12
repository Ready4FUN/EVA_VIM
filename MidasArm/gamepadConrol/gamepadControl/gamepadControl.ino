#include <AccelStepper.h>

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

// Define some steppers and the pins the will use
AccelStepper    forearm(AccelStepper::DRIVER, FOREARM_STEP_PIN, FOREARM_DIR_PIN);
AccelStepper    rail(AccelStepper::DRIVER, RAIL_STEP_PIN, RAIL_DIR_PIN);
AccelStepper    rotor(AccelStepper::DRIVER, ROTOR_STEP_PIN, ROTOR_DIR_PIN);
AccelStepper    shoulder(AccelStepper::DRIVER, SHOULDER_STEP_PIN, SHOULDER_DIR_PIN);
AccelStepper    grabber(AccelStepper::DRIVER, GRABBER_STEP_PIN, GRABBER_DIR_PIN);
AccelStepper    Motors[5] = {shoulder, forearm, rail, grabber, rotor};

String inputString = "";         // строка, в которую будут записываться входящие данные
boolean stringComplete = false;  // заполнилась ли строка или нет



enum correct {left, right, up, down, stop, stopGor, stopVer, bite, rotater}; //список действий для подстройки

enum correct correctAction = stop; 

class ControlArm{
    private:


    public:

    void correct(enum correct correctDo){

        Motors[0].run();
        Motors[4].run();

        switch(correctDo) {
            case up:
                Motors[0].moveTo(Motors[0].currentPosition() + 500);
                Serial.println("up");
                while(Motors[0].distanceToGo()) {
                    Motors[0].run();
                }
                correctAction = stop;
                break;
            case down:
                Motors[0].moveTo(Motors[0].currentPosition() - 500);
                Serial.println("down");
                while(Motors[0].distanceToGo()) {
                    Motors[0].run();
                }
                correctAction = stop;
                break;
            case left:
                Motors[4].moveTo(Motors[4].currentPosition() + 500);
                Serial.println("left");
                while(Motors[4].distanceToGo()) {
                    Motors[4].run();
                }
                //Serial.print("Current position = ");
                //Serial.print(Motors[4].currentPosition());

                correctAction = stop;
                break;
            case right:
                Motors[4].moveTo(Motors[4].currentPosition() - 500);
                Serial.println("right");
                while(Motors[4].distanceToGo()) {
                    Motors[4].run();
                }
                correctAction = stop;
                break;
            case stopGor:
                Motors[4].moveTo(Motors[4].currentPosition());
                Motors[4].stop();
                Serial.println("stopGor");
                break;
            case stopVer:
                Motors[0].moveTo(Motors[0].currentPosition());
                Motors[0].stop();
                Serial.println("stopVer");
                break;
            case stop:
                Motors[4].moveTo(Motors[4].currentPosition());
                Motors[4].stop();
                Motors[0].moveTo(Motors[0].currentPosition());
                Motors[0].stop();
                break;
            case bite:
                Serial.println("bite");
                Motors[0].moveTo(Motors[0].currentPosition() + 700);
                Motors[4].moveTo(Motors[4].currentPosition() + 300);
                while(Motors[0].distanceToGo() || Motors[4].distanceToGo()) {
                    Motors[0].run();
                    Motors[4].run();
                }
                //Движение вперёд
                Motors[2].moveTo(11000);
                
                while(Motors[2].distanceToGo()) {
                    Motors[2].run();
                }
        
                //Закрытие захвата
                analogWrite(3,255);
                Motors[3].moveTo(-15000);
                Motors[2].moveTo(0);
                while(Motors[3].distanceToGo()) {
                    Motors[3].run(); 
                }
                analogWrite(3,0);
                
                //движение назад
                Motors[2].moveTo(0);
                while(Motors[2].distanceToGo()) {
                    Motors[2].run();
                }

                //открытие захвата
                analogWrite(3,255);
                Motors[3].moveTo(0);
                while(Motors[3].distanceToGo()) {
                    Motors[3].run(); 
                }
                analogWrite(3,0);

                correctAction = stop;
                break;
        }

        
        
    }
    
};

ControlArm eva;

void setup() {
    Serial.begin(115200);
    inputString.reserve(200);

    //limit switch initialization 
    pinMode(RAIL_SWITCH,INPUT_PULLUP);
    pinMode(FOREARM_SWITCH,INPUT_PULLUP);
    pinMode(SHOULDER_SWITCH,INPUT_PULLUP);

    //Motors[2].setPinsInverted(true);
    Motors[0].setPinsInverted(true);

    for (int i = 0; i < 5; i++) {
        Motors[i].setMinPulseWidth(30);
        Motors[i].setMaxSpeed(500);
        Motors[i].setAcceleration(1000);
        //Motors[i].moveTo(-1000000);
    } 
        
    Motors[3].setMaxSpeed(3000);
    Motors[3].setAcceleration(6000);

    Motors[4].setMaxSpeed(3000);
    Motors[4].setAcceleration(6000);
}

void loop() {
    if (stringComplete) {
        if(inputString == "left\n") correctAction = left;
        if(inputString == "right\n") correctAction = right;
        if(inputString == "up\n")  correctAction = up;
        if(inputString == "down\n")  correctAction = down;
        if(inputString == "stopGor\n")  correctAction = stopGor;
        if(inputString == "stopVer\n")  correctAction = stopVer;
        if(inputString == "stop\n")  correctAction = stop;
        if(inputString == "bite\n")  correctAction = bite;

        inputString = "";
        stringComplete = false;
    }

    eva.correct(correctAction);
    //correctAction = stop;
}


void serialEvent() {
    while (Serial.available()) {
        char inChar = (char)Serial.read();
        inputString += inChar;

        if (inChar == '\n') {
            stringComplete = true;
        }
    }
}

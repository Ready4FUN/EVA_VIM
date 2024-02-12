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

byte    zeroSwitch[3] = {SHOULDER_SWITCH, FOREARM_SWITCH, RAIL_SWITCH};
// Define some steppers and the pins the will use
AccelStepper    forearm(AccelStepper::DRIVER, FOREARM_STEP_PIN, FOREARM_DIR_PIN);
AccelStepper    rail(AccelStepper::DRIVER, RAIL_STEP_PIN, RAIL_DIR_PIN);
AccelStepper    rotor(AccelStepper::DRIVER, ROTOR_STEP_PIN, ROTOR_DIR_PIN);
AccelStepper    shoulder(AccelStepper::DRIVER, SHOULDER_STEP_PIN, SHOULDER_DIR_PIN);
AccelStepper    grabber(AccelStepper::DRIVER, GRABBER_STEP_PIN, GRABBER_DIR_PIN);
AccelStepper    Motors[5] = {shoulder, forearm, rail, grabber, rotor};

String inputString = "";         // строка, в которую будут записываться входящие данные
boolean stringComplete = false;  // заполнилась ли строка или нет

enum event {Init, Scan, MoveToApple, Bite, Wait}; //Список действий

enum state {Waiting, Parking, Scaning, Moving, Bites};  //Список состойяний

enum stateMachine {goWait, goInit, goScan, goMoveToApple, goBite}; //Список состояний для постояного вызова

enum correct {left, right, up, down, stop, stopGor, stopVer, bite, rotate}; //список действий для подстройки

enum stateMachine evaAction = goWait;

enum correct correctAction = stop; 

enum state evaState = Waiting; //текущее состояние

class ControlArm{
    private:
    int mapLength = 4; // длина массива
    int k = 0;  // текущее положение в массиве

    //motion map 
    long wayRotor[8] =       {0,       25,   25,       0};
    long wayShoulder[8] =    {190,    190,    0,        0};
    long wayForearm[8] =     {95,      95,    0,        0};

    //Save current position to return 
    long shoulderPosition = 0;
    long forearmPosition = 0;

    public:
    void onWaiting(){
        //k = 0;
        Serial.println("Waiting");
    }

    void correct(enum correct correctDo){

        Motors[0].run();
        Motors[4].run();

        switch(correctDo) {
            case up:
                Motors[0].moveTo(Motors[0].currentPosition() + 100);
                //Serial.println("up");
                //Motors[0].run();
                break;
            case down:
                Motors[0].moveTo(Motors[0].currentPosition() - 100);
                //Serial.println("down");
                //Motors[0].run();
                break;
             case left:
                Motors[4].moveTo(Motors[4].currentPosition() + 100);
                //Serial.println("left");
                //Motors[4].run();
                break;
            case right:
                Motors[4].moveTo(Motors[4].currentPosition() - 100);
                //Serial.println("right");
                //Motors[4].run();
                break;
            case stopGor:
                //Motors[4].moveTo(Motors[4].currentPosition());
                Motors[4].stop();
                //Serial.println("stopGor");
                break;
            case stopVer:
                //Motors[0].moveTo(Motors[0].currentPosition());
                Motors[0].stop();
                //Serial.println("stopVer");
                break;
            case rotate:
                Motors[4].moveTo(Motors[4].currentPosition() + 500);
            case bite:
                onBite();
        }

        
        
    }

    void onParking(){
        Serial.println("Parking");

        Motors[2].setPinsInverted(true);
        Motors[0].setPinsInverted(true);

        for (int i = 0; i < 5; i++) {
            Motors[i].setMinPulseWidth(30);
            Motors[i].setMaxSpeed(500);
            Motors[i].setAcceleration(7000);
            Motors[i].moveTo(-1000000);
        } 
        
        Motors[1].setMaxSpeed(1500);
        Motors[1].setAcceleration(3000);

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

    void onScaning(){
        //Serial.println("Scaning");

        //Motors[1].setMaxSpeed(1000);
        //Motors[1].setAcceleration(2000);

        if(!Motors[4].distanceToGo() && !Motors[0].distanceToGo() && !Motors[1].distanceToGo()) {
            if (k >= mapLength) {
                //scanState = false;
                Serial.println("Scan end");\
                evaState = Waiting;
                k = 0;
                return;
            }
            Motors[4].moveTo(wayRotor[k]*100);
            Motors[0].moveTo(wayShoulder[k]*100);
            Motors[1].moveTo(wayForearm[k]*100);

            k++;
        }

        Motors[4].run();
        Motors[0].run();
        Motors[1].run();

    }

    void onMovingToApple(){
        static byte movingMapLength = 5;
        static byte thisStep = 0;

        static bool firstEntry = true;

        if(firstEntry){
            Serial.println("Moving start");

            Motors[0].stop();
            Motors[1].stop();
            Motors[2].stop();
            Motors[3].stop();
            Motors[4].stop();


            firstEntry = false;

            shoulderPosition = Motors[0].currentPosition();
            forearmPosition = Motors[1].currentPosition();

            Motors[0].moveTo(35000);
        
            if(shoulderPosition < 9000){
                Motors[1].moveTo(13500);
                Serial.println("Lower");
            }else if(shoulderPosition > 9000){
                Motors[1].moveTo(20000);
                Serial.println("upper");
            }
        }
        


        while(Motors[0].distanceToGo() || Motors[1].distanceToGo()) {
            Motors[0].run();
            Motors[1].run(); 
        }

        firstEntry = true;
        Serial.println("moving done");
    }

    void onBite(){
        //Один большой костыль ибо нет времени
        Serial.println("Bite");
        
        //Motors[0].moveTo(shoulderPosition);
       // Motors[1].moveTo(forearmPosition);
        Motors[0].moveTo(Motors[0].currentPosition() + 600);

        while(Motors[0].distanceToGo()) {
            Motors[0].run();
        }
        //Движение вперёд
        Motors[2].moveTo(7000);
        
        while(Motors[2].distanceToGo()) {
            Motors[2].run();
        }

        //Закрытие захвата
        analogWrite(3,255);
        Motors[3].moveTo(-15000);

        while(Motors[3].distanceToGo()) {
            Motors[3].run(); 
        }

        analogWrite(3,0);

        //движение к  корзине
        Motors[4].moveTo(12500);
        Motors[0].moveTo(-4000);
        Motors[2].moveTo(1000);
        while(Motors[4].distanceToGo() || Motors[0].distanceToGo() || Motors[2].distanceToGo() ) {
            Motors[4].run(); 
            Motors[0].run(); 
            Motors[2].run();
        }

        analogWrite(3,255);
        Motors[3].moveTo(0);
        Motors[2].moveTo(2000);
        while(Motors[3].distanceToGo() || Motors[2].distanceToGo()) {
            Motors[3].run();
            Motors[2].run(); 
        }

        analogWrite(3,0);
        
        Motors[4].moveTo(0);
        Motors[0].moveTo(0);
        Motors[2].moveTo(0);
        while(Motors[4].distanceToGo() || Motors[0].distanceToGo() || Motors[2].distanceToGo() ) {
            Motors[4].run(); 
            Motors[0].run(); 
            Motors[2].run();
        }

        Serial.println("wait");

        //while(true)
        /*
        //Движение назад с открытием захвата
        Motors[2].moveTo(0);
        Motors[3].moveTo(0);

        while(Motors[2].distanceToGo() || Motors[3].distanceToGo()) {
            Motors[2].run();
            Motors[3].run(); 
        }


        
        analogWrite(3,0);
        //Возврат в изначальное положение
        while(Motors[0].distanceToGo() || Motors[1].distanceToGo()) {
            Motors[0].run();
            Motors[1].run(); 
        }
        
        if(k > 0){
            k--;
        }
        Serial.println("bite done");
        */
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
        Motors[i].moveTo(-1000000);
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
        if(inputString == "rotate\n")  correctAction = rotate;
        if(inputString == "bite\n")  correctAction = bite;

        inputString = "";
        stringComplete = false;
    }

    eva.correct(correctAction); 
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
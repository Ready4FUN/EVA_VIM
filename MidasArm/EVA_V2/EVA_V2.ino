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

enum stateMachine evaAction = goWait;

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

    void onParking(){
        Serial.println("Parking");

        //Motors[2].setPinsInverted(true);
        Motors[0].setPinsInverted(true);

        for (int i = 0; i < 5; i++) {
            Motors[i].setMinPulseWidth(30);
            Motors[i].setMaxSpeed(5000);
            Motors[i].setAcceleration(7000);
            Motors[i].moveTo(-1000000);
        } 
        
        Motors[1].setMaxSpeed(1500);
        Motors[1].setAcceleration(3000);

        /*
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
        */
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
        Motors[0].moveTo(shoulderPosition);
        Motors[1].moveTo(forearmPosition);

        //Движение вперёд
        Motors[2].moveTo(8000);
        
        while(Motors[2].distanceToGo()) {
            Motors[2].run();
        }

        //Закрытие захвата
        analogWrite(3,255);
        Motors[3].moveTo(-15000);

        while(Motors[3].distanceToGo()) {
            Motors[3].run(); 
        }

        //временный(вечный) костыль
       
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
        */
        if(k > 0){
            k--;
        }
        Serial.println("bite done");
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
}
     
void loop() {
    if (stringComplete) {
        if(inputString == "init\n") evaAction = goInit;
        if(inputString == "scan\n") evaAction = goScan;
        if(inputString == "moveToApple\n") evaAction = goMoveToApple;
        if(inputString == "bite\n") evaAction = goBite;
        if(inputString == "wait\n") evaAction = goWait;

        inputString = "";
        stringComplete = false;
    }

    switch (evaAction) {
        case goWait:
            doEvent(Wait);
            break;
        case goInit:
            doEvent(Init);
            break;
        case goScan:
            doEvent(Scan);
            break;
        case goMoveToApple:
            doEvent(MoveToApple);
            break;
        case goBite:
            doEvent(Bite);
            break;
    }

    Motors[4].run();
    Motors[0].run();
    Motors[1].run();
}

void doEvent(enum event e){
    switch (e) {
        case Init:
            switch (evaState) {
                case Waiting:
                    evaState = Parking;
                    eva.onParking();
                    break;
            }
            break;
        case Scan:
            switch (evaState) {
                case Parking:
                    evaState = Scaning;
                    eva.onScaning();
                    break;
                case Bites:
                    evaState = Scaning;
                    eva.onScaning();
                    break;
                case Scaning:
                    evaState = Scaning;
                    eva.onScaning();
                    break;
            }
            break;
        case MoveToApple:
            switch (evaState) {
                case Scaning:
                    evaState = Moving;
                    eva.onMovingToApple();
                    break;
            }
            break;
        case Bite:
            switch (evaState) {
                case Moving:
                    evaState = Bites;
                    eva.onBite();
                    break;
            }
            break;
        case Wait:
            evaState = Waiting;
            eva.onWaiting();
            break;
    }
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
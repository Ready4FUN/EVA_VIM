
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


class ControlArm {
    private:
    int mapLength = 8; // длина массива
    int k = 0;  // текущее положение в массиве

    //motion map 
    long wayRotor[8] =       {0,       25,   25,    50,     50,     75,   75,   0};
    long wayShoulder[8] =    {190,    190,    0,     0,    190,    190,    0,   0};
    long wayForearm[8] =     {95,      95,    0,     0,     95,     96,    0,   0};


    //save current path to continue
    long rotorToGo = 0;
    long shoulderToGo = 0;
    long forearmToGo = 0;

    //Save current position to return
    long shoulderPosition = 0;
    long forearmPosition = 0;

    //Continue from the saved path after collecting the apple
    bool continueScan = false;
    //run scan or bite
    bool scanState = true;

    public:
    void scanning(){
        Serial.println("Scan");

        if(!scanState){ 
            return;
        }

        if(continueScan){
            continueScan = false;
            
            k--;
            
            Motors[0].moveTo(wayShoulder[k]*100);
            Motors[1].moveTo(wayForearm[k]*100);
            Motors[4].moveTo(wayRotor[k]*100);
        }

        if(!Motors[4].distanceToGo() && !Motors[0].distanceToGo() && !Motors[1].distanceToGo()) {
            if (k >= mapLength) {
                scanState = false;
                Serial.println("Scan end");
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

    void bite() {
        static byte biteMapLength = 5;
        static byte thisStep = 0;

        static bool firstEntry = true;

        if(firstEntry){
            Serial.println("bite");

            Motors[0].stop();
            Motors[1].stop();
            Motors[2].stop();
            Motors[3].stop();
            Motors[4].stop();


            firstEntry = false;

            shoulderPosition = Motors[0].currentPosition();
            forearmPosition = Motors[1].currentPosition();

            rotorToGo = Motors[4].distanceToGo();
            shoulderToGo = Motors[0].distanceToGo();
            forearmToGo = Motors[1].distanceToGo();

            Serial.print("Shoulder position - ");
            Serial.println(shoulderPosition);
            Serial.print("Forearm position - ");
            Serial.println(forearmPosition);

             Serial.print("rotorToGo - ");
            Serial.println(rotorToGo);
             Serial.print("shoulderToGo - ");
            Serial.println(shoulderToGo);
             Serial.print("forearmToGo - ");
            Serial.println(forearmToGo);
        }
        

        static long biteRail[5] =        {0,        10000,    10000,        0,                 0};
        static long biteShoulder[5] =    {35000,    35000,    35000,    35000,   shoulderPosition};
        static long biteForearm[5] =     {17000,    17000,    17000,    17000,   forearmPosition};
        static long biteGrabber[5] =     {0,            0,   -10000,        0,                 0};


        if(!Motors[0].distanceToGo() && !Motors[1].distanceToGo() && !Motors[2].distanceToGo() && !Motors[3].distanceToGo()) {
            if (thisStep >= biteMapLength) {
                thisStep = 0;
                scanState = true;
                continueScan = true;
                firstEntry = true;

                analogWrite(3,0);

                Serial.println("Bite end");

                return;
            }
            Motors[0].moveTo(biteShoulder[thisStep]);
            Motors[1].moveTo(biteForearm[thisStep]);
            Motors[2].moveTo(biteRail[thisStep]);
            Motors[3].moveTo(biteGrabber[thisStep]);

            thisStep++;
        }
        if(thisStep >= 1 && thisStep <= 4){
            analogWrite(3,255);
        } else {
            analogWrite(3,0);
        }

        Motors[0].run();
        Motors[1].run();
        Motors[2].run();
        Motors[3].run();
    
    }

    void controlMove(bool change) {
        if(change){
            scanState = false;
        }

        if(scanState){
            scanning();
        } else {
            bite();
        }
    }

    void parking() {
        Serial.println("parking");

        Motors[2].setPinsInverted(true);
        Motors[0].setPinsInverted(true);

        for (int i = 0; i < 5; i++) {
            Motors[i].setMinPulseWidth(30);
            Motors[i].setMaxSpeed(5000);
            Motors[i].setAcceleration(3000);
            Motors[i].moveTo(-1000000);
        } 
       
        Motors[1].setMaxSpeed(2000);
        Motors[1].setAcceleration(2000);

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
};

String inputString = "";        // String for read serial port
boolean stringComplete = false;  //Data is ending

ControlArm eva;//greated scan object

void setup()
{  

    Serial.begin(115200);

    inputString.reserve(200);   //Reserv 200 byte to input string

    pinMode(RAIL_SWITCH,INPUT_PULLUP);
    pinMode(FOREARM_SWITCH,INPUT_PULLUP);
    pinMode(SHOULDER_SWITCH,INPUT_PULLUP);

    eva.parking();

}

void loop()
{
    //eva.bite();
    eva.controlMove(false);
}


void serialEvent() {
    while(Serial.available()){
        char inChar = (char)Serial.read();

        inputString += inChar;

        if(inChar == '\n'){
            stringComplete = true;
        }

        if(stringComplete){
            Serial.println(inputString);
            if(inputString == "bite\n"){
                eva.controlMove(true);
            }
            
        }
    }
}

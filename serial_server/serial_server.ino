#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);

const int ledPin = 13;
int lineBuffer;
int motorSpeed = 50;
int time;
int last = 0;
int second = 1600;
void setup(){
   Serial.begin(9600);
   lineBuffer = 0;
   pinMode(ledPin, OUTPUT);
   AFMS.begin(); 
   leftMotor->setSpeed(motorSpeed);
   rightMotor->setSpeed(motorSpeed);  
}

int response(int arg) {
   switch (arg) {
        case 0 :
             //null argument
             return 0;
        case 1 :
             Serial.println("Forward!");
             leftMotor->run(FORWARD);
             rightMotor->run(FORWARD);
             return second;
        case 2 :
             Serial.println("Backward!");
             leftMotor->run(BACKWARD);
             rightMotor->run(BACKWARD);
             return second;
        case 3 :
             Serial.println("Left?");
             time = readData();
             leftMotor->run(FORWARD);
             rightMotor->run(BACKWARD);
             return second * time;
        case 4 :
             Serial.println("Right?");
             time = readData();
             leftMotor->run(BACKWARD);
             rightMotor->run(FORWARD);
             return second * time;
        case 5:
             Serial.println("Distance");
             return 0;
        case 6:
             Serial.println("Stop!");
             leftMotor->run(RELEASE);
             rightMotor->run(RELEASE);       
             return 0;
        default:
             Serial.println("Unknown");
             return 0;
    };
};

int readData(){
   Serial.flush();
   while(1) {
      if (Serial.available() > 0) {
         return Serial.parseInt();  
      };
   };
}

void loop(){
      if (Serial.available() > 0 and last == 0) {
         digitalWrite(ledPin, HIGH);
         lineBuffer = Serial.parseInt();
         last = response(lineBuffer);
         digitalWrite(ledPin, LOW); 
         };
      if (last > 0) {
        last = last -1;
      };   
};



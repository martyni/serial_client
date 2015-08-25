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
void setup(){
   Serial.begin(9600);
   lineBuffer = 0;
   pinMode(ledPin, OUTPUT);
   AFMS.begin(); 
   leftMotor->setSpeed(motorSpeed);
   rightMotor->setSpeed(motorSpeed);  
}

void response(int arg) {
   switch (arg) {
        case 0 :
             //null argument
             break;
        case 1 :
             Serial.println("Forward!");
             leftMotor->run(FORWARD);
             rightMotor->run(FORWARD);
             break;
        case 2 :
             Serial.println("Backward!");
             leftMotor->run(BACKWARD);
             rightMotor->run(BACKWARD);
             break;
        case 3 :
             Serial.println("Left?");
             time = readData();
             leftMotor->run(FORWARD);
             rightMotor->run(BACKWARD);
             delay(time);
             leftMotor->run(RELEASE);
             rightMotor->run(RELEASE); 
             break;
        case 4 :
             Serial.println("Right?");
             time = readData();
             leftMotor->run(BACKWARD);
             rightMotor->run(FORWARD);
             delay(time);
             leftMotor->run(RELEASE);
             rightMotor->run(RELEASE);              ;
             break;
        case 5:
             Serial.println("Distance");
             break;
        case 6:
             Serial.println("Stop!");
             leftMotor->run(RELEASE);
             rightMotor->run(RELEASE);       
             break;
        default:
             Serial.println("Unknown");
             break;
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
      if (Serial.available() > 0) {
         digitalWrite(ledPin, HIGH);
         lineBuffer = Serial.parseInt();
         response(lineBuffer);
         digitalWrite(ledPin, LOW); 
         };
};



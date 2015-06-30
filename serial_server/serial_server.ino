#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(4);

const int ledPin = 13;
int lineBuffer;
int motorSpeed = 100;

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
             leftMotor->run(FORWARD);
             rightMotor->run(BACKWARD);
             break;
        case 4 :
             Serial.println("Right?");
             leftMotor->run(BACKWARD);
             rightMotor->run(FORWARD);
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
             Serial.println("unknown");
             break;
    };
};



void loop(){
      if (Serial.available() > 0) {
         digitalWrite(ledPin, HIGH);
         lineBuffer = Serial.parseInt();
         response(lineBuffer);
         digitalWrite(ledPin, LOW); 
         };
};



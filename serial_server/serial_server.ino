#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <NewPing.h>
#include "utility/Adafruit_PWMServoDriver.h"

#define TRIGGER_PIN  12
#define ECHO_PIN     13
#define MAX_DISTANCE 200
#define SECOND 32000
#define ledPin 13
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
 
int lineBuffer;
int seconds;
int motorSpeed = 100;
int Time;
int commandDecay = 0;
int distanceAway;
boolean autodrive = true;

void setup(){
   Serial.begin(9600);
   lineBuffer = 0;
   AFMS.begin(); 
   leftMotor->setSpeed(motorSpeed);
   rightMotor->setSpeed(motorSpeed);  
}

void loop(){
      //Main loop
      if (Serial.available() > 0) {
         //If there is a command waiting in the serial buffer read it and trigger the command
         lineBuffer = Serial.parseInt();
         commandDecay = serial_commands(lineBuffer);
         };
  
      if (commandDecay > 0) {
        //if the commandDecay value has not reached zero yet reduce it by one
        commandDecay -= 1;
      }else if (10 > distance() > 0) {
         //else if the distance read in through the distance censor is between 0-10 cm move backwards
         commandDecay = backward();  
      }else if (commandDecay < 1) {
        //if command decay reaches below 1 
        if (autodrive){
          //if autodrive is enabled pick a random command and move for either 1 or 2 "amounts"
          commandDecay = autodrive_function(random(5), random(1,3));
        } else { 
          //if autodrive isn't enabled just stop
        commandDecay = stopMotors();
        };  
      };  
};

int serial_commands(int arg) {
  //If the commandDecay isn't below zero reduce it by one
   switch (arg) {
        case 0 :
             //null argument
             return 0;
        case 1 :
             Serial.println("Forward!");
             return forward(1);
        case 2 :
             Serial.println("Backward!");
             return backward();
        case 3 :
             Serial.println("Left?");
             seconds = readData();
             return left(seconds);
        case 4 :
             Serial.println("Right?");
             seconds = readData();
             return right(seconds);
        case 5:
             distanceAway = distance();
             Serial.print("Distance: ");
             Serial.print(String(distanceAway));
             Serial.println("cm");
             //don't want to change the value of commandDecay so just return its previous value
             return commandDecay;
        case 6:
             autodrive_switch();
             Serial.print("Autodrive: ");
             Serial.println(String(autodrive)); 
             return 0;   
        case 7:
             Serial.println("Stop!");   
             return 0;     
        default:
             Serial.println("Unknown");
             //default argument to make sure client knows when its read all the commands
             return 0;
    };
};

int readData(){
   //Function for reading 
   Serial.flush();
   while(1) {
      if (Serial.available() > 0) {
         return Serial.parseInt();  
      };
   };
}

long forward(int seconds) {
   leftMotor->run(FORWARD);
   rightMotor->run(FORWARD);
   return seconds * SECOND;
}

long backward() {
   leftMotor->run(BACKWARD);
   rightMotor->run(BACKWARD);
   return  SECOND;
}

long left(int seconds) {
   leftMotor->run(FORWARD);
   rightMotor->run(BACKWARD);
   return seconds * SECOND;
};

long right(int seconds) {
   leftMotor->run(BACKWARD);
   rightMotor->run(FORWARD);
   return seconds * SECOND;
};

int stopMotors() {
   leftMotor->run(RELEASE);
   rightMotor->run(RELEASE);
   return -1;
};

int distance() {
   return sonar.ping_cm();
};

void autodrive_switch() {
   if (autodrive) {
      autodrive = false;
   } else {
      autodrive = true;
   };
};

int autodrive_function(int function,int amount){
   switch (function){
      case 0:
         return forward(amount);
      case 3:
         return backward();
      case 2:
         return left(amount);
      case 1:
         return right(amount);   
   };
};

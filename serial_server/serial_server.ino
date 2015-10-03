#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <NewPing.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include "utility/Adafruit_PWMServoDriver.h"

#define TFT_SCLK 13 
#define TFT_MOSI 11 
#define TFT_CS     10
#define TFT_RST    9  
#define TFT_DC     8
#define TRIGGER_PIN  7
#define ECHO_PIN     6
#define MAX_DISTANCE 40
#define SECOND 30


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
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
int width = tft.width();
int height = tft.height();
int pupilSize = 8;
boolean autodrive = true;

void setup() {
   Serial.begin(9600);
   lineBuffer = 0;
   AFMS.begin(); 
   leftMotor->setSpeed(motorSpeed);
   rightMotor->setSpeed(motorSpeed); 
   tft.initR(INITR_BLACKTAB);   
   uint16_t time = millis();
   tft.fillScreen(ST7735_BLACK);
   initDisplay(); 
}

void loop(){
      distanceAway = distance();
      if (Serial.available() > 0) {
         //If there is a command waiting in the serial buffer read it and trigger the command
         lineBuffer = Serial.parseInt();
         commandDecay = serial_commands(lineBuffer);
         };
         

      if (commandDecay > 0) {
        //if the commandDecay value has not reached zero yet reduce it by one
        commandDecay -= 1; 
      } else if (commandDecay < 1) {
        //if command decay reaches below 1 
        if (autodrive) {
          //if autodrive is enabled pick a random command and move for either 1 or 2 "amounts"
          if (10 > distanceAway  && distanceAway > 1) {
            commandDecay = backward();
          } else {
            commandDecay = autodrive_function(random(5), random(1,4) );
          };
        } else { 
          //if autodrive isn't enabled just stop
          commandDecay = stopMotors();
        };  
      }
    draw_pupils(distanceAway);  
}

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
             Serial.print("Decay size: ");
             Serial.println(commandDecay);
             return commandDecay;
        case 8:
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
   drawData('F', seconds, 0);
   leftMotor->run(FORWARD);
   rightMotor->run(FORWARD);
   return seconds * SECOND;
}

long backward() {
   drawData('B', 1, 0);
   leftMotor->run(BACKWARD);
   rightMotor->run(BACKWARD);
   return  SECOND;
}

long left(int seconds) {
   drawData('L', seconds, 0);
   leftMotor->run(FORWARD);
   rightMotor->run(BACKWARD);
   return seconds * SECOND;
};

long right(int seconds) {
   drawData('R', seconds, 0);
   leftMotor->run(BACKWARD);
   rightMotor->run(FORWARD);
   return seconds * SECOND;
};

int stopMotors() {
   drawData('S', 0, 0);
   leftMotor->run(RELEASE);
   rightMotor->run(RELEASE);
   return -1;
};

int distance() {
   int dist = sonar.ping_cm();
   return dist;
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

void initDisplay() {
  tft.fillScreen(ST7735_BLACK);
  tft.fillRoundRect(0, 0, MAX_DISTANCE, MAX_DISTANCE, 8, ST7735_WHITE);
  tft.fillRoundRect(width - MAX_DISTANCE, 0, MAX_DISTANCE, MAX_DISTANCE, 8, ST7735_WHITE);
  tft.fillRoundRect(MAX_DISTANCE - pupilSize, MAX_DISTANCE/2, pupilSize, pupilSize, 1, ST7735_RED);
  tft.fillRoundRect(width - MAX_DISTANCE, MAX_DISTANCE/2, pupilSize, pupilSize, 1, ST7735_RED);
};

void draw_pupils(int pupilPosition) {
  tft.fillRoundRect(width - MAX_DISTANCE, 0, MAX_DISTANCE, MAX_DISTANCE, 8, ST7735_WHITE);
  tft.fillRoundRect(0, 0, MAX_DISTANCE, MAX_DISTANCE, 8, ST7735_WHITE);
  tft.fillRoundRect(MAX_DISTANCE - pupilSize - pupilPosition, MAX_DISTANCE/2, pupilSize, pupilSize, 1, ST7735_GREEN);
  tft.fillRoundRect(width - MAX_DISTANCE + pupilPosition, MAX_DISTANCE/2, pupilSize, pupilSize, 1, ST7735_GREEN);
};

void drawData (char botDirection, int time , int line) {
  tft.fillRect(width/2 -10, MAX_DISTANCE + line, width, 20, ST7735_BLACK);  
  tft.setCursor(width/2 -10, MAX_DISTANCE + line * 10);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(2);
  tft.setTextWrap(true);
  tft.print(botDirection);
  tft.print(time);
  
};




const int ledPin = 13;
int lineBuffer;
char* toClient;

void setup(){
   Serial.begin(9600);
   lineBuffer = 0;
}

char* response(int arg) {
   switch (arg) {
        case 0 :
             return "Null";
        case 1 :
             return "Forward";
        case 2 :
             return "Backward";
        case 3 :
             return "Left";
        case 4 :
             return "Right";
        case 5:
             return "Distance";
        default:
             return "Unknown arg";
    };
};

void loop(){
      if (Serial.available() > 0) {
         lineBuffer = Serial.parseInt();
         toClient = response(lineBuffer);
         Serial.println(toClient);
      };
};



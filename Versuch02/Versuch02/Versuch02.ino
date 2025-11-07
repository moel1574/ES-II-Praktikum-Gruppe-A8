#include <Servo.h> 


#define DEBUG 

#ifdef DEBUG 

  #define DEBUG_PRINT(x)    Serial.print(x) 

  #define DEBUG_PRINTLN(x)  Serial.println(x) 

#else 

  #define DEBUG_PRINT(x) 

  #define DEBUG_PRINTLN(x) 

#endif 


const int potPin = A0;   
const int servoPin = 4;  

Servo myServo; 

void setup() { 

  #ifdef DEBUG 

    Serial.begin(9600); 

    while (!Serial);  

    DEBUG_PRINTLN("Servo-Test gestartet"); 

  #endif 

  myServo.attach(servoPin); 

} 

 

void loop() { 

  int potValue = analogRead(potPin);     
  int angle = map(potValue, 0, 1023, 5, 175); 

  myServo.write(angle);                  

  DEBUG_PRINT("Pot: "); 

  DEBUG_PRINT(potValue); 

  DEBUG_PRINT("  → Servo: "); 

  DEBUG_PRINTLN(angle); 

  delay(50);  

} 
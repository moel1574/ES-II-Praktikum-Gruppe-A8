#define DEBUG 

 

#ifdef DEBUG 

  #define DEBUG_PRINT(x)    Serial.print(x) 

  #define DEBUG_PRINTLN(x)  Serial.println(x) 

#else 

  #define DEBUG_PRINT(x) 

  #define DEBUG_PRINTLN(x) 

#endif 

 

const int buttonPin = 4;          

unsigned long previousMillis = 0; 

unsigned long interval = 3000;     

bool ledState = false; 

bool fastMode = false; 

unsigned long fastModeStart = 0; 

 

void setup() { 

  pinMode(LED_BUILTIN, OUTPUT); 

  pinMode(buttonPin, INPUT_PULLUP); 
  Serial.begin(9600); 

  while (!Serial); 

  DEBUG_PRINTLN("Programm gestartet."); 

} 

 

void loop() { 

  unsigned long currentMillis = millis(); 

  if (digitalRead(buttonPin) == HIGH) {   

    fastMode = true; 

    fastModeStart = currentMillis; 

    interval = 500;   

    DEBUG_PRINTLN("Taster gedrückt → Schnellmodus aktiv"); 
    delay(200);

  } 

 
  if (fastMode && (currentMillis - fastModeStart >= 60000)) { 

    fastMode = false; 

    interval = 3000;  

    DEBUG_PRINTLN("Schnellmodus beendet"); 

  } 

 


  if (currentMillis - previousMillis >= interval) { 

    previousMillis = currentMillis; 

    ledState = !ledState; 

    digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW); 

    DEBUG_PRINT("Millis: "); 

    DEBUG_PRINTLN(currentMillis); 

  } 

} 
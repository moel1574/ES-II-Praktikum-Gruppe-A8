// --- Bibliotheken ---
#include <Wire.h>      
#include <rgb_lcd.h>   
#include <Servo.h>     
#include <math.h>      

// --- DEBUG-Makros ---
#define DEBUG
#ifdef DEBUG
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
  #define START_SERIAL() Serial.begin(9600); DEBUG_PRINTLN("--- Debug-Modus aktiv ---")
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define START_SERIAL()
#endif

// --- Hardware-Abstraktion (Aufgabe g) ---
#if defined(ARDUINO_ARCH_SAM) // Due
  #define ADC_RESOLUTION 10
  #define MAX_VOLTAGE 3300
  #define WireMaster Wire1 // zweiten I2C-Bus für Due
  #define SETRES(x) analogReadResolution(x)
#elif defined(ARDUINO_ARCH_AVR) // Uno
  #define ADC_RESOLUTION 10
  #define MAX_VOLTAGE 5000
  #define WireMaster Wire
  #define SETRES(x) while(false){}
#else
  #error "Wrong Hardware"
#endif

const unsigned long ADCMax = (1 << ADC_RESOLUTION) - 1;

// --- Pin-Definitionen ---
const int LED_PIN = LED_BUILTIN; 
const int TASTER_PIN = 2;        
const int POTI_PIN = A0;         
const int SERVO_PIN = 8;         
const int pinTempSensor = A1;    

// --- Globale Variablen ---
unsigned long letzterBlinkZeit = 0;
unsigned long aktuellesIntervall = 3000; 
bool LEDState = LOW;

// Fast Mode
volatile bool fastModeSollStarten = false; 
volatile unsigned long fastModeEndeZeit = 0; 
volatile bool fastModeAktiv = false;

// Servo und LCD
Servo meinServo;
rgb_lcd lcd;

// Thermistor
const unsigned int B = 4275; 
float currentTemperature = 0.0;

// --- Setup ---
void setup() {
  pinMode(LED_PIN, OUTPUT); 
  pinMode(TASTER_PIN, INPUT_PULLUP);
  START_SERIAL();

  attachInterrupt(digitalPinToInterrupt(TASTER_PIN), isr_tasterGedrueckt, FALLING);

  meinServo.attach(SERVO_PIN);
  meinServo.write(90); 

  // --- LCD initialisieren ---
  WireMaster.begin(); 
  lcd.begin(16, 2, 0x27, WireMaster); 
  lcd.setRGB(0, 255, 0); 
  lcd.print("System Start");

  SETRES(ADC_RESOLUTION); // ADC-Auflösung setzen (nur Due relevant)
}

// --- Loop ---
void loop() {
  unsigned long jetzigeZeit = millis();

  // Fast Mode starten / beenden
  if (fastModeSollStarten) {
    DEBUG_PRINTLN("Interrupt -> Fast Mode 60s aktivieren");
    aktuellesIntervall = 500;        
    fastModeAktiv = true;
    fastModeEndeZeit = jetzigeZeit + 60000; 
    fastModeSollStarten = false;
  }

  if (fastModeAktiv && (jetzigeZeit >= fastModeEndeZeit)) {
    DEBUG_PRINTLN("Fast Mode abgelaufen -> 3s Intervall");
    aktuellesIntervall = 3000; 
    fastModeAktiv = false;
  }

  // LED Blink
  if (jetzigeZeit - letzterBlinkZeit >= aktuellesIntervall) {
    letzterBlinkZeit = jetzigeZeit;
    LEDState = !LEDState;
    digitalWrite(LED_PIN, LEDState);

    DEBUG_PRINT("Zeit seit Start: ");
    DEBUG_PRINTLN(jetzigeZeit);

    // Temperatur messen
    int value = analogRead(pinTempSensor); 
    float R = (float)ADCMax / (float)value - 1.0;
    currentTemperature = 1.0 / (log(R) / B + 1.0 / 298.15) - 273.15;

    DEBUG_PRINT("ADC: "); DEBUG_PRINT(value);
    DEBUG_PRINT(" -> Temp: "); DEBUG_PRINTLN(currentTemperature);

    // LCD-Ausgabe
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temp: ");
    lcd.print(currentTemperature, 1); 
    lcd.write((uint8_t)0xDF); 
    lcd.print("C");
  }

  // Potentiometer → Servo
  int potiWert = analogRead(POTI_PIN);
  int servoWinkel = map(potiWert, 0, ADCMax, 5, 175); 
  meinServo.write(servoWinkel);

  DEBUG_PRINT("Poti: "); DEBUG_PRINT(potiWert);
  DEBUG_PRINT(" -> Servo: "); DEBUG_PRINTLN(servoWinkel);
}

// --- Interrupt ---
void isr_tasterGedrueckt() {
  if (!fastModeAktiv) fastModeSollStarten = true;
}
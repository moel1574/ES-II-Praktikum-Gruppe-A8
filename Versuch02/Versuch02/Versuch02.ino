#include <Wire.h> 

#include "rgb_lcd.h" 

#include <math.h> 

#define DEBUG 

 

// --- Board-spezifische Definitionen --- 

#if defined(ARDUINO_ARCH_SAM)   // Arduino Due 

  #define MAX_VOLTAGE 3300 

  #define WireMaster Wire1 

  #define SETRES(x) analogReadResolution(x) 

  #define ADC_RESOLUTION 12 

#elif defined(ARDUINO_ARCH_AVR) // Seeeduino / ATmega328P 

  #define ADC_RESOLUTION 10 

  #define MAX_VOLTAGE 5000 

  #define WireMaster Wire 

  #define SETRES(x) while(false){} 

#else 

  #error "Wrong Hardware" 

#endif 

const unsigned long ADCMax = (1 << ADC_RESOLUTION) - 1; 


const int pinTempSensor = A0; 

const unsigned int B = 4275;   // B-Wert des Thermistors 

const float R0 = 100000.0;     // Referenzwiderstand 100k 

const float T0 = 298.15;       // 25 °C in Kelvin 

const int r = 255;
const int g = 100;
const int b = 0;

rgb_lcd lcd; 

void setup() { 

  #ifdef DEBUG 

    Serial.begin(9600); 

    while(!Serial); 

    Serial.println("LCD + Temperatur gestartet"); 

  #endif 

 
  SETRES(ADC_RESOLUTION); 

  WireMaster.begin(); 

  lcd.begin(16, 2); 

  lcd.setRGB(r, g, b);  

  lcd.print("Temperatur:"); 

} 

 

void loop() { 


  int value = analogRead(pinTempSensor); 


  float R = R0 * ((float)ADCMax / value - 1.0); 


  float tempK = 1.0 / (log(R / R0) / B + 1.0 / T0); 

  float tempC = tempK - 273.15; 

 

  #ifdef DEBUG 

    Serial.print("ADC: "); Serial.print(value); 

    Serial.print("  Temp: "); Serial.print(tempC, 1); Serial.println(" °C"); 

  #endif 


  lcd.setCursor(0, 1); 

  lcd.print("                "); 

  lcd.setCursor(0, 1); 

  lcd.print(tempC, 1); 

  lcd.print(" "); 

  lcd.write((unsigned char)223); 

  lcd.print("C"); 

  delay(1000); 

} 
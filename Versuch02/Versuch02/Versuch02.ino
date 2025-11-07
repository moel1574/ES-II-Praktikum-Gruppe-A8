#include <math.h> 


#define DEBUG 

#ifdef DEBUG 

  #define DEBUG_PRINT(x)    Serial.print(x) 

  #define DEBUG_PRINTLN(x)  Serial.println(x) 

#else 

  #define DEBUG_PRINT(x) 

  #define DEBUG_PRINTLN(x) 

#endif 

 

#define ARDUINO_DUE 

#if defined(ARDUINO_DUE) 

  const float ADCMax = 4095.0; 

#else 

  const float ADCMax = 1023.0; 

#endif 



const int pinTempSensor = A0;       // Analog-Pin 

const unsigned int B = 4275;        // B-Wert des Thermistors (abhängig vom Sensor) 

const float R0 = 100000.0;          // Referenzwiderstand (z. B. 100kΩ beim Grove-Modul) 

const float T0 = 298.15;            // 25 °C in Kelvin 

int value = 0;

float R = R0 * ((ADCMax / value) - 1.0); 
 

void setup() { 

  #ifdef DEBUG 

    Serial.begin(9600); 

    while (!Serial); 

    DEBUG_PRINTLN("Temperatursensor gestartet"); 

  #endif 

} 

 

void loop() { 

  value = analogRead(pinTempSensor);     

  float R = R0 * ((1023.0 / value) - 1.0);  

  float tempK = 1.0 / (log(R / R0) / B + (1.0 / T0));   

  float tempC = tempK - 273.15;                        



  DEBUG_PRINT("ADC: "); 

  DEBUG_PRINT(value); 

  DEBUG_PRINT("  R_NTC: "); 

  DEBUG_PRINT(R); 

  DEBUG_PRINT(" Ohm  → Temp: "); 

  DEBUG_PRINTLN(tempC); 

  delay(1000); 

} 
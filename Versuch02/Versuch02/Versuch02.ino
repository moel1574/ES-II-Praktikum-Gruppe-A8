#include <Wire.h> 

#include "rgb_lcd.h" 

#include <math.h> 

 

// --- Konstanten für den Temperatursensor --- 

const int pinTempSensor = A0;        // Analogeingang 

const unsigned int B = 4275;         // B-Wert des Thermistors (lt. Datenblatt) 

const float R0 = 100000.0;           // Referenzwiderstand 100 kΩ 

const float T0 = 298.15;             // 25 °C in Kelvin (25 + 273.15) 

const float ADCMax = 1023.0;         // 10-Bit-ADC (0…1023) 

 

rgb_lcd lcd;                         // LCD-Objekt 

 

void setup() { 

  Serial.begin(9600); 

  lcd.begin(16, 2);                  

  lcd.setRGB(0, 128, 255);           

  lcd.print("Temperatur:"); 

} 

 

void loop() { 

  int value = analogRead(pinTempSensor); 

  float R = R0 * (ADCMax / value - 1.0); 

  float tempK = 1.0 / (log(R / R0) / B + 1.0 / T0); 

  float tempC = tempK - 273.15;  

  Serial.print("ADC: "); Serial.print(value); 

  Serial.print("  Temp: "); Serial.print(tempC, 2); Serial.println(" °C"); 


  lcd.setCursor(0, 1);               

  lcd.print("                ");     

  lcd.setCursor(0, 1); 

  lcd.print(tempC, 1);                

  lcd.print(" "); 

  lcd.write((unsigned char)223);      

  lcd.print("C"); 

  delay(1000);                       

} 
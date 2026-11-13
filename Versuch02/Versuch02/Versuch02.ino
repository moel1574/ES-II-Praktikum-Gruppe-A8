
#include <Arduino.h>

// ==========================================
// Hardware-Abstraktion
// ==========================================

// Green Board: Arduino Due
#if defined(ARDUINO_ARCH_SAM)

#define ADC_RESOLUTION 12
#define MAX_VOLTAGE 3300
#define SETRES(x) analogReadResolution(x)
#define WireMaster Wire1

// Red Board: Seeeduino V4 / Arduino Uno
#elif defined(ARDUINO_ARCH_AVR)

#define ADC_RESOLUTION 10
#define MAX_VOLTAGE 5000
#define SETRES(x) do {} while(false)
#define WireMaster Wire

#else
#error "Wrong Hardware"
#endif

#include <Wire.h>
#include <Servo.h>
#include <math.h>
#include <rgb_lcd.h>

// ==========================================
// Debug
// ==========================================

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

// ==========================================
// ADC
// ==========================================

const unsigned long ADCMax =
  (1UL << ADC_RESOLUTION) - 1;

// ==========================================
// Pins
// ==========================================

const int ledPin = LED_BUILTIN;
const int buttonPin = 2;
const int potPin = A1;
const int pinTempSensor = A0;
const int servoPin = 3;

const unsigned int B = 4275;

// ==========================================
// Objekte
// ==========================================

Servo myServo;
rgb_lcd lcd;

// ==========================================
// Interrupt
// ==========================================

volatile unsigned long buttonTriggeredAt = 0;
volatile bool isFastMode = false;

void onButtonPress() {
  buttonTriggeredAt = millis();
  isFastMode = true;
}

// ==========================================
// Setup
// ==========================================

void setup() {

  Serial.begin(9600);

  SETRES(ADC_RESOLUTION);

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  myServo.attach(servoPin);

  // LCD initialisieren
  lcd.begin(16, 2, LCD_5x8DOTS, WireMaster);

  lcd.setRGB(0, 128, 255);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Versuch 2");

  lcd.setCursor(0, 1);
  lcd.print("System startet");

  delay(2000);
  lcd.clear();

  attachInterrupt(
    digitalPinToInterrupt(buttonPin),
    onButtonPress,
    RISING
  );

  DEBUG_PRINTLN("Versuch 2 gestartet");

#if defined(ARDUINO_ARCH_SAM)
  DEBUG_PRINTLN("Arduino Due - 12 Bit ADC");
#else
  DEBUG_PRINTLN("Seeeduino / Uno - 10 Bit ADC");
#endif

}

// ==========================================
// Loop
// ==========================================

void loop() {

  // a) und b) Laufzeit
  DEBUG_PRINT("Laufzeit: ");
  DEBUG_PRINT(millis());
  DEBUG_PRINTLN(" ms");

  // d) Potentiometer und Servo
  int potValue = analogRead(potPin);

  int servoAngle = map(
    potValue,
    0,
    ADCMax,
    5,
    175
  );

  servoAngle = constrain(servoAngle, 5, 175);

  myServo.write(servoAngle);

  // e) Temperatursensor
  int rawTempValue = analogRead(pinTempSensor);

  float curr_temp = NAN;

  if (rawTempValue > 0 && rawTempValue < ADCMax) {

    float resistanceRatio =
      (float)ADCMax / rawTempValue - 1.0;

    curr_temp =
      1.0 / (
        log(resistanceRatio) / B
        + 1.0 / 298.15
      ) - 273.15;

  }

  // Serielle Ausgabe
  DEBUG_PRINT("Poti: ");
  DEBUG_PRINT(potValue);

  DEBUG_PRINT(" | Servo: ");
  DEBUG_PRINT(servoAngle);

  DEBUG_PRINT(" Grad | Temperatur: ");
  DEBUG_PRINT(curr_temp);

  DEBUG_PRINTLN(" C");

  // f) und g) LCD-Ausgabe

  // Erste Zeile: Temperatur
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");

  if (!isnan(curr_temp)) {
    lcd.print(curr_temp, 1);
  } else {
    lcd.print("--.-");
  }

  lcd.write((uint8_t)223);
  lcd.print("C    ");

  // Zweite Zeile: Servo
  lcd.setCursor(0, 1);
  lcd.print("Servo: ");
  lcd.print(servoAngle);
  lcd.write((uint8_t)223);
  lcd.print("     ");

  // c) Schneller Modus
  unsigned long currentInterval = 3000;

  if (isFastMode) {

    unsigned long triggerTime = buttonTriggeredAt;

    if (millis() - triggerTime < 60000UL) {

      currentInterval = 500;
      DEBUG_PRINTLN("Schneller Modus aktiv");

    } else {

      isFastMode = false;
      DEBUG_PRINTLN("Normalmodus aktiv");

    }
  }

  // a) LED blinken
  digitalWrite(ledPin, HIGH);
  delay(100);

  digitalWrite(ledPin, LOW);

  // Wartezeit
  if (currentInterval > 100) {
    delay(currentInterval - 100);
  }

}

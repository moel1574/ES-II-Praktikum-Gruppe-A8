int led = LED_BUILTIN;
int taster = 3;
int ledZustand = 0;

void setup() {
  
  pinMode(led, OUTPUT);
  pinMode(taster, INPUT_PULLUP);

  Serial.begin(9600);

}

void loop() {
  if (digitalRead(taster)==1 && ledZustand == 0) {
    digitalWrite(led, HIGH);
    ledZustand = 1;
    Serial.println("LED eingeschaltet");
    delay(100);
  } else if (digitalRead(taster)==1 && ledZustand == 1) {
    digitalWrite(led, LOW);
    ledZustand = 0;
    Serial.println("LED ausgeschaltet");
    delay(100);
  }
  delay(100);
}
//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
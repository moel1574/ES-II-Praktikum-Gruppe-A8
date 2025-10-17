int led = 13;
int taster = 3;
int ledZustand = 0;

void setup() {
  
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  pinMode(taster, INPUT_PULLUP);

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(13, HIGH);
  //delay(1000);
  //digitalWrite(13, LOW);
  //delay(1000);

  if (digitalRead(taster)==1 && ledZustand == 0) {
    digitalWrite(led, HIGH);
    ledZustand = 1;
    Serial.println("LED eingeschaltet");
    delay(1000);
  } else if (digitalRead(taster)==1 && ledZustand == 1) {
    digitalWrite(led, LOW);
    ledZustand = 0;
    Serial.println("LED ausgeschaltet");
    delay(1000);
  }
}
//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
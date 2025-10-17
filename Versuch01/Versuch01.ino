int led = 13;
int taster = 3;

void setup() {
  
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  pinMode(taster, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(13, HIGH);
  //delay(1000);
  //digitalWrite(13, LOW);
  //delay(1000);

  if (digitalRead(taster)==1) {
    digitalWrite(led, HIGH);
    delay(1000);
  } else {
    digitalWrite(led, LOW);
    
  }
}
//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
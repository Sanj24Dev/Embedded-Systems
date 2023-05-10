int pushBtn = 2;
int pushBtnVal = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pushBtn, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}
// the loop function runs over and over again forever
void loop() {
  pushBtnVal = digitalRead(pushBtn);
  if(pushBtnVal == HIGH)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  Serial.println(pushBtnVal);
}

unsigned int count;
void setup() {
  // put your setup code here, to run once:
  count = 0;
  pinMode(7, OUTPUT);
  delay(1000);
  while(count <= 5) {
    digitalWrite(7,LOW);
    delay(100);
    digitalWrite(7,HIGH);
    delay(100);
    count++;
  }
  while(1){}

}

void loop() {


}

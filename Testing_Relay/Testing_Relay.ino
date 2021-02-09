//Code to test relays 
void setup() {
  // initialize digital pin as an output.
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
}

void loop() {
  digitalWrite(5, HIGH);    // turn the relay ON
  delay(1000);                       // wait for a second
  digitalWrite(5, LOW);     // turn the relay OFF
  delay(1000);                      // wait for a second
  digitalWrite(4, HIGH);    // turn the relay ON
  delay(1000);                       // wait for a second
  digitalWrite(4, LOW);     // turn the relay OFF
  delay(1000);                       // wait for a second
  digitalWrite(14, HIGH);   // turn the relay ON
  delay(1000);                       // wait for a second
  digitalWrite(14, LOW);    // turn the relay OFF
  delay(1000);                       // wait for a second
  digitalWrite(12, HIGH);   // turn the relay ON
  delay(1000);                       // wait for a second
  digitalWrite(12, LOW);    // turn the relay OFF
  delay(1000);                      // wait for a second
}

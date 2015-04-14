/*
  AltBlink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  Patrick Kubiak
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int ledA = 13;
int ledB = 12;
 
// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(ledA, OUTPUT);     
  pinMode(ledB, OUTPUT);
}
 
// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(ledA, HIGH);     // turn the LED A on
  digitalWrite(ledB, LOW);      // turn the LED B off
  delay(1000);                  // wait for a second
  digitalWrite(ledA, LOW);      // turn the LED A off
  digitalWrite(ledB, HIGH);     // turn the LED B on
  delay(1000);                  // wait for a second
}

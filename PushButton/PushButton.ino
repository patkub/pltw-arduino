/*
  Pushbutton
  Reads a VEX bumber switch (or pushbutton) a d turns on an LED.
 
  Patrick Kubiak
 */
 
// definitions
const int buttonPin = 12;
const int ledPin = 13;
int buttonState = 0, ledState = 0, oldState = -1;
 
// setup
void setup() {                
  // initialize button and led
  pinMode(buttonPin, OUTPUT);     
  pinMode(ledPin, OUTPUT);
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}
 
// loop forever
void loop() {
  buttonState = digitalRead(buttonPin);   // get button state
  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);           // button pressed, turn on led
  }
  else if (buttonState == LOW) {
    digitalWrite(ledPin, LOW);            // button released, turn off led
  }
  
  ledState = digitalRead(ledPin);         // get led state
  
  if (ledState != oldState) {
    Serial.println(ledState);             // print out led state only if it changed
    oldState = ledState;
  }
}

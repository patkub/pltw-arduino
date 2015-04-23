/*
  Analog Read Serial
  Reads value of potentiometer and prints it out every millisecond
 
  Patrick Kubiak
 */
 
// definitions
const int potPin = A0;
int potValue;
 
// setup
void setup() {                
  // initialize potentiometer
  pinMode(potPin, OUTPUT);
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}
 
// loop forever
void loop() {
  potValue = analogRead(potPin);    //get analog value
  Serial.println(potValue);
  delay(1);
}


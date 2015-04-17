/*
  Memsic2125
  
  Reads input from a Memsic 2125 two-axis accelerometer.
  Converts pulse outputs into acceleration in milli-g's 
  (1/1000 of earth's gravity) and prints them out.
 
  Patrick Kubiak
 */
 
// definitions
const int joyX = 2;    // x output of accelerometer
const int joyY = 3;    // y output of accelerometer
 
// setup
void setup() {                
  // initialize accelerometer input
  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);
    
  // initialize serial communication at 9600 bits per second
  Serial.begin(9600);
}
 
// loop forever
void loop() {
  int pulseX, pulseY;  // store pulse widths
  int accelX, accelY;  // store accelerations
  
  // read X and Y pulse
  pulseX = pulseIn(joyX, HIGH);
  pulseY = pulseIn(joyY, HIGH);
  
  // convery pulse width to acceleration in milli-g's
  // earth's gravtiy is 1000 milli-g's, or 1g
  accelX = ((pulseX / 10) - 500) * 8;
  accelY = ((pulseY / 10) - 500) * 8;
  
  // display acceleration
  Serial.println("X = " + String(accelX) + "\t Y = " + String(accelY));
  delay(100);
}

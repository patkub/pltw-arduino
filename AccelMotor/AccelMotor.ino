/*
  AccelerometerMotor
  
  Use an accelerometer to drive a motor.
  Tilting an accelerometer forward makes the motor move forward.
  Tilting an accelerometer backward makes the motor move backward.
  Forward and backward are arbitrary.
  
  Patrick Kubiak
 */
 
#include <Servo.h>
 
// definitions
Servo myServo;           // create servo object to control a servo
 
const int memX = 2;      // x output of accelerometer
const int memY = 3;      // y output of accelerometer
 
// setup
void setup() {                
  myServo.attach(6);    // attach the servo on pin 6 to the servo object 
  Serial.begin(9600);
  
  // initialize accelerometer input
  pinMode(memX, INPUT);
  pinMode(memY, INPUT);
  
  // initialize serial communication at 9600 bits per second
  Serial.begin(9600);
}
 
// loop forever
void loop() {
  int pulseX, pulseY;  // store pulse widths
  int accelX, accelY;  // store accelerations
  
  // read X and Y pulse
  pulseX = pulseIn(memX, HIGH);
  pulseY = pulseIn(memY, HIGH);
  
  // convery pulse width to acceleration in milli-g's
  // earth's gravtiy is 1000 milli-g's, or 1g
  accelX = ((pulseX / 10) - 500) * 8;
  accelY = ((pulseY / 10) - 500) * 8;
  
  // scale and adjust so that the motor does not move while the breadboard sits on a table
  accelY = map(accelY, -950, 700, 0, 180);
  
  // set servo position according to scaled value
  myServo.write(accelY);
  
  // display acceleration
  Serial.println("X = " + String(accelX) + "\t Y = " + String(accelY));;
  delay(100);
}


/*
  MicroServoPot: Controlling a servo motor using a potentiometer (variable resistor)
  Reads an analog input on pin A1 and moves a VEX 393 2-wire motor with a Motor Controller 29. 
  With the sketch uploaded, open the serial monitor to see the values. (Tools -> Serial Monitor) (Ctrl + Shift + M)
  This example code is in the public domain.
 */
 
#include <Servo.h> 
 
Servo myservo;                        // create servo object to control a servo 
 
int potpin = A1;                      // analog pin used to connect the potentiometer
int val;                              // variable to read the value from the analog pin 
 
void setup() 
{ 
  myservo.attach(6);                   // attaches the servo on pin 6 to the servo object 
  myservo.writeMicroseconds(1500);     // set servo to mid-point
  Serial.begin(9600);                  // initialize serial communication at 9600 bits per second
} 
 
void loop() 
{ 
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 1000, 2000); // scale it to use it with the servo (value between 1000 and 2000) 
  myservo.writeMicroseconds(val);      // sets the servo position according to the scaled value
  Serial.println(val);                 // print out the value between 
  delay(15);                           // waits for the servo to get there 
}


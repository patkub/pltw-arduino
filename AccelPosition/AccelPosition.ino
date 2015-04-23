/*
  AccelPosition
  
  Reads input from a Memsic 2125 two-axis accelerometer.
  Converts pulse outputs into acceleration in milli-g's 
  (1/1000 of earth's gravity) and prints them out.
  Displays center, front, back, left, and right position LEDs.
  
  Patrick Kubiak
 */
 
// definitions
const int FRONT = -150;  // front threshold
const int BACK = 150;    // back threshold
const int LEFT = 150;    // left threshold
const int RIGHT = -150;  // right threshold

const int memX = 2;      // x output of accelerometer
const int memY = 3;      // y output of accelerometer

const int ledF = 13;     // front led
const int ledB = 12;     // back led
const int ledL = 11;     // left led
const int ledR = 10;     // right led
const int ledC = 9;      // center led

// setup
void setup() {                
  // initialize accelerometer input
  pinMode(memX, INPUT);
  pinMode(memY, INPUT);
  
  // initialize display LEDs
  pinMode(ledF, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(ledL, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledC, OUTPUT);
    
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
  
  // display acceleration
  Serial.println("X = " + String(accelX) + "\t Y = " + String(accelY));
  displayPosition(accelX, accelY);
  delay(100);
}

void displayPosition(int accelX, int accelY) {
  digitalWrite(ledF, LOW);
  digitalWrite(ledB, LOW);
  digitalWrite(ledL, LOW);
  digitalWrite(ledR, LOW);
  digitalWrite(ledC, LOW);
  
  if (accelY >= FRONT && accelY <= BACK && accelX <= LEFT && accelX >= RIGHT)
  {
    digitalWrite(ledC, HIGH);
  }
  else
  {
    if (accelY < FRONT)
    {
      digitalWrite(ledF, HIGH);
    }
    
    if (accelY > BACK)
    {
      digitalWrite(ledB, HIGH);
    }
    
    if (accelX > LEFT)
    {
      digitalWrite(ledL, HIGH);
    }
    
    if (accelX < RIGHT)
    {
      digitalWrite(ledR, HIGH);
    }
  }
}


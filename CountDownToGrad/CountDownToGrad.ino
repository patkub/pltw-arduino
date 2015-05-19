#include <SoftwareSerial.h>
#include <SimpleTimer.h>

//--- SETTINGS ---
const String strTitle = "2015 GRADUATION";    // title - max 16 characters
const String strGrad = "CONGRATS GRADS!";     // timer done message - max 16 characters
const int lcdPin = 2;                         // pin for LCD

// time to count down from
const int dMax = 29;                          // days
const int hMax = 7;                           // hours
const int mMax = 40;                          // minutes
const int sMax = 0;                           // seconds
//--- END OF SETTINGS ---

// time definitions
const double secDay = 86400;                  // seconds in a day
double seconds;                               // seconds left
int days = 0;                                 // days count

// objects
SoftwareSerial mySerial = SoftwareSerial(255, lcdPin);
SimpleTimer myTimer;
int timerId;                                  // reference to clock timer

void setup() {
  pinMode(lcdPin, OUTPUT);                           // set lcdPin as output to LCD
  mySerial.begin(9600);                              // start serial
  timerId = myTimer.setInterval(1000, modifyClock);  // update clock every 1 sec
  setupTime();                                       // setup time left
  delay(100);
}

void loop() {
  myTimer.run();    // run the timer
}

// Modify clock display.
void modifyClock() {
  String time = getClockTime();
  
  if (time == "done")
  {
    myTimer.disable(timerId);
    onTimerEnd();
  }
  else
  {
    mySerial.write(22);                   // no cursor
    mySerial.write(12);                   // clear
    mySerial.println(strTitle);           // first line, display title
    mySerial.write(148);                  // new line
    mySerial.print(time + " PK");         // second line, display time and initials
    Serial.println("Time: " + time);      // console debug
  }
}

// Get time left in DD:HH:MM:SS format.
String getClockTime() {
  int m = 0, h = 0, d = 0;
  double s;
  
  seconds--;  //update seconds
  
  if (seconds <= 0)
  {
    // a day has passed, increment day and reset seconds
    days++;
    seconds = secDay;    
  }
  
  // calc days left
  d = dMax - days;
  
  // calc seconds left
  s = seconds;
  
  if (d < 0)
  {
    // timer done
    return "done";
  }
  
  // calc minutes left
  m = (int)(s / 60);
  s -= m * 60;
  
  // calc hours left
  h = (int)(m / 60);
  m -= h * 60;
  
  // format DD:HH:MM:SS
  String strD = printDigits(d, true);
  String strH = printDigits(h, true);
  String strM = printDigits(m, true);
  String strS = printDigits((int)s, false);
  return strD+strH+strM+strS;
}

/*
 * Print out digits appending necessary leading zero's and colons.
 *
 * digits - integer digits to print
 * bColon - True to include a trailing colon, false to not
 * return String representation of digits
 */
String printDigits(int digits, boolean bColon) {
  String s;
  if (digits < 10) {
    s += "0";
  }
  s += String(digits);
  if (bColon) {
    s += ":";
  }
  return s;
}

// Timer is done.
void onTimerEnd()
{
  mySerial.write(12);                   // clear
  mySerial.println(strTitle);           // first line, display title
  mySerial.write(148);                  // new line
  mySerial.print(strGrad);              // second line, display congrats
  Serial.println("Timer done!");        // console debug
}

// Setup time to count
void setupTime()
{
  seconds = (hMax * 60 * 60) + (mMax * 60) + sMax;   // seconds left
  days = 0;                                          // days elapsed 
}


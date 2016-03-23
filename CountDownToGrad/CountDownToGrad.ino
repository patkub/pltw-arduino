#include <SoftwareSerial.h>
#include <SimpleTimer.h>

//--- SETTINGS ---
String strTitle = " 2016 GRADUATION";         // title - must be 16 characters, add padding to front
const String strGrad = "CONGRATS GRADS!";     // timer done message - max 16 characters
const int lcdPin = 2;                         // pin for LCD

// time to count down from
const int dMax = 74;                          // days
const int hMax = 0;                           // hours
const int mMax = 0;                           // minutes
const int sMax = 0;                           // seconds
//--- END OF SETTINGS ---

const int onBoardLEDPin = 13;                 // built in led on pin 13

// time definitions
const double secDay = 86400;                  // seconds in a day
double seconds;                               // seconds left
int days = 0;                                 // days count

// objects
SoftwareSerial mySerial = SoftwareSerial(255, lcdPin);
SimpleTimer myTimer;
int timerId, timerId2, timerId3;                          // reference to clock timer, light timer, and title timer
String strTime, strLine2;

// Configure display and timer.
void setup() {
  pinMode(lcdPin, OUTPUT);                                // set lcdPin as output to LCD
  pinMode(onBoardLEDPin, OUTPUT);                         // set onBoardLEDPin as output to LCD
  mySerial.begin(9600);                                   // start serial
  timerId = myTimer.setInterval(1000, modifyClock);       // update clock every 1 sec
  timerId2 = myTimer.setInterval(500, flashLED);          // flash light every 0.5 sec
  timerId3 = myTimer.setInterval(500, updateTitle);       // update title every 0.5 sec
  setupTime();                                            // setup time left
  delay(100);
  
  // pad title string with 16 whitespaces in the front
  for (int i = 0; i < 16; i++) {
     strTitle = " " + strTitle;
  }
}

// Run the timer.
void loop() {
  myTimer.run();    // run the timer
}

// Modify clock display.
void modifyClock() {
  double s;
  
  seconds--;  //update seconds
  
  if (seconds <= 0)
  {
    // a day has passed, increment day and reset seconds
    days++;
    seconds = secDay;    
  }
  
  // calc days left
  int d = dMax - days;
  strTime = getClockTime(d);
  
  if (d < 0) {
    // timer done
    myTimer.disable(timerId);
    myTimer.disable(timerId2);
    myTimer.disable(timerId3);
    onTimerEnd();
  }
  else {
    mySerial.write(22);                   // no cursor
    mySerial.write(12);                   // clear
    mySerial.println(strTitle);           // first line, display title
    mySerial.write(148);                  // new line
    
    int spaceCount = 16 - strTime.length() - 2;
    String strSpace = "";
    for (int i = 0; i < spaceCount; i++) {
      strSpace += " ";
    }
    
    strLine2 = strTime + strSpace + "PK";
    
    mySerial.print(strLine2);      // second line, display time and initials
    Serial.println("Time: " + strTime);   // console debug
  }
}

void flashLED() {
  int ledState = digitalRead(onBoardLEDPin);
  if (ledState == HIGH) {
    digitalWrite(onBoardLEDPin, LOW);
  }
  else if (ledState == LOW) {
    digitalWrite(onBoardLEDPin, HIGH);
  }
}

void updateTitle() {
  strTitle = strTitle.charAt(strTitle.length() - 1) + strTitle.substring(0, strTitle.length() - 1);
  
  mySerial.write(22);                   // no cursor
  mySerial.write(12);                   // clear
  mySerial.println(strTitle);           // first line, display title
  mySerial.write(148);                  // new line
  
  int spaceCount = 16 - strTime.length() - 2;
  String strSpace = "";
  for (int i = 0; i < spaceCount; i++) {
    strSpace += " ";
  }
  
  strLine2 = strTime + strSpace + "PK";
  
  mySerial.print(strLine2);      // second line, display time and initials
  Serial.println("Time: " + strTime);   // console debug
}

// Get time left in DD:HH:MM:SS format.
String getClockTime(int daysLeft) {
  // calc seconds left
  double s = seconds;
  
  // calc minutes left
  int m = (int)(s / 60);
  s -= m * 60;
  
  // calc hours left
  int h = (int)(m / 60);
  m -= h * 60;
  
  // format DD:HH:MM:SS
  String strD = printDigits(daysLeft, true);
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

// Setup time to count.
void setupTime()
{
  seconds = (hMax * 60 * 60) + (mMax * 60) + sMax;   // seconds left
  days = 0;                                          // days elapsed 
}


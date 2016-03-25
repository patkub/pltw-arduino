#include <SoftwareSerial.h>
#include <SimpleTimer.h>

//--- SETTINGS ---
const String strTitle = "2016 GRADUATION www.antuple.net/projects/";    // title - must be a multiple of 16 characters
const String strGrad = "CONGRATS GRADS!";     // timer done message - must be a multiple of 16 characters
const int lcdPin = 2;                         // pin for LCD
const int scrollDirection = 0;                // direction to scroll text in, 0 = right to left, 1 = left to right

// time to count down from
const int dMax = 70;                          // days
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
String strTitleDisplay = strTitle;
String strGradDisplay = strGrad;
boolean bTimerDone = false;

// Configure display and timer.
void setup() {
  pinMode(lcdPin, OUTPUT);                                // set lcdPin as output to LCD
  pinMode(onBoardLEDPin, OUTPUT);                         // set onBoardLEDPin as output to LCD
  mySerial.begin(9600);                                   // start serial
  mySerial.write(22);                                     // no cursor
  timerId = myTimer.setInterval(1000, updateClock);       // update clock time every 1 sec
  timerId2 = myTimer.setInterval(500, flashLED);          // flash light every 0.5 sec
  timerId3 = myTimer.setInterval(350, updateLCD);         // update lcd every 0.35 sec
  setupTime();                                            // setup time left
  delay(100);
  
  // pad title string with 16 whitespaces
  int padding = strTitle.length() % 16;
  
  // scroll title
  for (int i = 0; i < padding; i++) {
     if (scrollDirection == 0) {
       // scroll text right to left
       strTitleDisplay = " " + strTitleDisplay;
     } else if (scrollDirection == 1) {
       // scroll text left to right
       strTitleDisplay = strTitleDisplay + " ";
     }
  }
  
  // pad graduation string with 16 whitespaces
  padding = strGrad.length() % 16;
  
  // scroll timer done message
  for (int i = 0; i < padding; i++) {
    if (scrollDirection == 0) {
       // scroll text right to left
       strGradDisplay = " " + strGradDisplay;
     } else if (scrollDirection == 1) {
       // scroll text left to right
       strGradDisplay = strGradDisplay + " ";
     }
  }
}

// Run the timer.
void loop() {
  myTimer.run();    // run the timer
}

// Update clock counter.
void updateClock() {
  if (bTimerDone) {
   return; 
  }
  
  seconds--;  // update seconds
  
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
    onTimerEnd();
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

// move the cursor to a specific place
// e.g.: cursorSet(3,2) sets the cursor to x = 3 and y = 2
void cursorSet(int xpos, int ypos){  
  Serial.write(254);
  Serial.write(71);               
  Serial.write(xpos);   //Column position   
  Serial.write(ypos); //Row position 
} 

void updateLCD() {
  if (bTimerDone) {
    if (scrollDirection == 0) {
      // scroll text right to left
      strGradDisplay = strGradDisplay.substring(1, strGradDisplay.length()) + strGradDisplay.charAt(0);
    } else if (scrollDirection == 1) {
      // scroll text left to right
      strGradDisplay = strGradDisplay.charAt(strGradDisplay.length() - 1) + strGradDisplay.substring(0, strGradDisplay.length() - 1);
    }
    
    mySerial.print(strGradDisplay);       // first line, display congrats
    mySerial.write(148);                  // new line
    
    int spaceCount = 16 - strTime.length() - 2;
    String strSpace = "";
    for (int i = 0; i < spaceCount; i++) {
      strSpace += " ";
    }     
    
    // format time in DD:HH:MM:SS
    strTime = formateClockTime(0, 0, 0, 0);
    
    // second line, display time and initials
    strLine2 = strTime + strSpace + "PK";
    mySerial.print(strLine2);
    Serial.println("Time: " + strTime);   // console debug
  } else {
    if (scrollDirection == 0) {
      // scroll text right to left
      strTitleDisplay = strTitleDisplay.substring(1, strTitleDisplay.length()) + strTitleDisplay.charAt(0);
    } else if (scrollDirection == 1) {
      // scroll text left to right
      strTitleDisplay = strTitleDisplay.charAt(strTitleDisplay.length() - 1) + strTitleDisplay.substring(0, strTitleDisplay.length() - 1);
    }
    
    mySerial.println(strTitleDisplay);
    int spaceCount = 16 - strTime.length() - 2;
    String strSpace = "";
    for (int i = 0; i < spaceCount; i++) {
      strSpace += " ";
    }
    
    // second line, display time and initials
    strLine2 = strTime + strSpace + "PK";
    mySerial.print(strLine2);
    Serial.println("Time: " + strTime);   // console debug
  }
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
  return formateClockTime(daysLeft, h, m, s);
}

// format time in DD:HH:MM:SS
String formateClockTime(int days, int hours, int minutes, int seconds) {
  return printDigits(days, true) + printDigits(hours, true) + printDigits(minutes, true) + printDigits(seconds, false);
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
    // disable timers
    myTimer.disable(timerId2);
    mySerial.write(12);                   // clear
    bTimerDone = true;
}

// Setup time to count.
void setupTime()
{
  seconds = (hMax * 60 * 60) + (mMax * 60) + sMax;   // seconds left
  days = 0;                                          // days elapsed 
}


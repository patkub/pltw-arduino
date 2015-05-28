#include <SoftwareSerial.h>

//--- SETTINGS ---
const String strTitle = "MORRA";         // title - all phrases have 16 character limit
const int lcdPin = 2;                    // pin for LCD
const int btnHand = 4;                   // pin for hand button
const int btnGuess = 3;                  // pin for guess button
const int btnPlay = 5;                   // pin for play button

const String strNoWinner = "No winner";
const String strTie = "It's a tie!";
const String strPlayerWins = "You win round!";
const String strCompWins = "CPU wins round!";
//--- END OF SETTINGS ---

// player
int playerHand = 1;
int playerGuess = 2;

// score
int playerScore = 0;
int compScore = 0;

// result of round
String strRoundResult = "";

// keep track of button changes
int oldBtnStateHand, oldBtnStateGuess, oldBtnStatePlay;
int showResult = 0;                     // result to show (1, 2, 3, 4, or 0 for none)

SoftwareSerial mySerial = SoftwareSerial(255, lcdPin);

void setup() {
  pinMode(btnHand, INPUT);     // set hand button pin as input
  pinMode(btnGuess, INPUT);    // set guess button pin as input
  pinMode(btnPlay, INPUT);     // set play button pin as input
  mySerial.begin(9600);        // start serial
  mySerial.write(22);          // no cursor
  mySerial.write(12);          // clear
  updateLCD();
  delay(100);
}

void loop() {
  int btnStateHand = digitalRead(btnHand);
  int btnStateGuess = digitalRead(btnGuess);
  int btnStatePlay = digitalRead(btnPlay);
  
  if (btnStatePlay != oldBtnStatePlay && showResult == 4) {
    updateLCD();
    showResult = 0;
  }
  else if (btnStatePlay != oldBtnStatePlay && showResult == 3) {
    showResult = 4;
  }
  else if (btnStatePlay != oldBtnStatePlay && showResult == 2) {
    displayResultOfRound();
    showResult = 3;
  }
  else if (btnStatePlay != oldBtnStatePlay && showResult == 1) {
    showResult = 2;
  }
  else if (showResult == 0) {    
    if (btnStateHand == HIGH && btnStateHand != oldBtnStateHand) {
      nextHand();
      updateLCD();
      oldBtnStateHand = btnStateHand;
    }
    
    if (btnStateGuess == HIGH && btnStateGuess != oldBtnStateGuess) {
      nextGuess();
      updateLCD();
      oldBtnStateGuess = btnStateGuess;
    }
    
    if (btnStatePlay == HIGH && btnStatePlay != oldBtnStatePlay) {
      playRound();
    }
    
    if (btnStateHand == LOW) {
      oldBtnStateHand = btnStateHand;
    }
    
    if (btnStateGuess == LOW) {
      oldBtnStateGuess = btnStateGuess;
    }
  }
  
  if (btnStatePlay != oldBtnStatePlay) {
    oldBtnStatePlay = btnStatePlay;
  }
}

// increment player hand
void nextHand() {
  if (playerHand < 5) {
    playerHand++;
  }
  else {
    playerHand = 1;
  }
}

// increment player guess
void nextGuess() {
  if (playerGuess < 10) {
    playerGuess++;
  }
  else {
    playerGuess = 2;
  }
}

// play a round
void playRound() {
  // computer hand is a random number from 1 to 5
  int compHand = random(1, 6);
  
  // computer guess is computer hand plus random from 1 to 5
  int compGuess = compHand + random(1, 6);
  
  // determine round winner
  int roundWinner = getRoundWinner(compHand, compGuess);
  
  // determine result statement
  switch (roundWinner)
  {
    case -1:
    {
      // no winner
      strRoundResult = strNoWinner;
      break;
    }
    case 0:
    {
      // tie
      strRoundResult = strTie;
      break;
    }
    case 1:
    {
      // player won
      playerScore++;
      strRoundResult = strPlayerWins;
      break;
    }
    case 2:
    {
      // computer won
      compScore++;
      strRoundResult = strCompWins;
      break;
    }
  }
  
  displayRoundStats(compHand, compGuess);
}

// get winner (-1 no winner, 0 tie, 1 player won, 2 computer won)
int getRoundWinner(int compHand, int compGuess) {
  int total = playerHand + compHand;    // get total of both hands
  int winner = -1;
  
  if (compGuess == total && playerGuess == total) {
    winner = 0;    // it's a tie
  }
  else if (playerGuess == total) {
    winner = 1;    // player won
  }
  else if (compGuess == total) {
    winner = 2;    // computer won
  }
  
  return winner;
}

void updateLCD() {
  showHeader();
  
  // display hand and guess
  mySerial.print("Hand: " + String(playerHand) + " G: " + String(playerGuess));
}

// display round stats
void displayRoundStats(int compHand, int compGuess) {
  showHeader();
  
  // display round stats
  int total = playerHand + compHand;    // get total of both hands
  mySerial.print("CH:" + String(compHand) + " CG:" + String(compGuess) + " T:" + String(total));
  showResult = 1;
}

// display result of round
void displayResultOfRound() {
  showHeader();
  mySerial.print(strRoundResult);    // display result of round
}

// display header with score
void showHeader() {
  mySerial.write(12);    // clear
  
  // first line, display title and score
  mySerial.println(strTitle + " H:" + String(playerScore) + " C:" + String(compScore));
  mySerial.write(148);   // new line
}


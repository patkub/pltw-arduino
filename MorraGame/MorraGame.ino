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
int displayState = 0;                     // state of display (Stage: 0, 1 Results: 2, 3, 4, 5, 6)
int roundsToPlay = 1;                     // number of rounds to play this game
int roundsPlayed = 0;                     // number of rounds played so far
int winsNeeded = 1;                       // number of round wins needed to win game

SoftwareSerial mySerial = SoftwareSerial(255, lcdPin);

void setup() {
  pinMode(btnHand, INPUT);     // set hand button pin as input
  pinMode(btnGuess, INPUT);    // set guess button pin as input
  pinMode(btnPlay, INPUT);     // set play button pin as input
  mySerial.begin(9600);        // start serial
  mySerial.write(22);          // no cursor
  mySerial.write(12);          // clear
  delay(100);
  showRoundsToPlay();
}

void loop() {
  int btnStateHand = digitalRead(btnHand);
  int btnStateGuess = digitalRead(btnGuess);
  int btnStatePlay = digitalRead(btnPlay);
  
  if (btnStatePlay != oldBtnStatePlay && displayState == 6) {
    resetScore();
    showRoundsToPlay();
    displayState = 0;
  }
  else if (btnStatePlay != oldBtnStatePlay && displayState == 5) {
    updateLCD();
    displayState = 1;
  }
  else if (btnStatePlay != oldBtnStatePlay && displayState == 4) {
    displayGameWinner();
  }
  else if (btnStatePlay != oldBtnStatePlay && displayState == 3) {
    displayResultOfRound();
    displayState = 4;
  }
  else if (btnStatePlay != oldBtnStatePlay && displayState == 2) {
    displayState = 3;
  }
  else if (displayState == 1) {
    if (btnStateHand == HIGH && btnStateHand != oldBtnStateHand) {
      nextHand();
      updateLCD();
    }
    if (btnStateGuess == HIGH && btnStateGuess != oldBtnStateGuess) {
      nextGuess();
      updateLCD();
    }
    if (btnStatePlay == HIGH && btnStatePlay != oldBtnStatePlay) {
      playRound();
    }
  }
  else if (displayState == 0) {
    if (btnStateHand == HIGH && btnStateHand != oldBtnStateHand && roundsToPlay > 1) {
      roundsToPlay -= 2;
      showRoundsToPlay();
    }
    else if (btnStateGuess == HIGH && btnStateGuess != oldBtnStateGuess) {
      roundsToPlay += 2;
      showRoundsToPlay();
    }
    else if (btnStatePlay == HIGH && btnStatePlay != oldBtnStatePlay) {
      winsNeeded = roundsToPlay - ((int)(roundsToPlay / 2));	// best (1/2, 2/3, 3/5, 4/7, 5/9, etc.)
      updateLCD();
      displayState = 1;
    }
  }
  
  if (btnStateHand != oldBtnStateHand) {
    oldBtnStateHand = btnStateHand;
  }
  
  if (btnStateGuess != oldBtnStateGuess) {
    oldBtnStateGuess = btnStateGuess;
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
  
  roundsPlayed++;
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

// display number of rounds that will be played
void showRoundsToPlay() {
  showHeaderNoScore();
  mySerial.print("Rounds: " + String(roundsToPlay));
}

// display round stats
void displayRoundStats(int compHand, int compGuess) {
  showHeader();
  
  // display round stats
  int total = playerHand + compHand;    // get total of both hands
  mySerial.print("CH:" + String(compHand) + " CG:" + String(compGuess) + " T:" + String(total));
  displayState = 2;
}

// display result of round
void displayResultOfRound() {
  showHeader();
  mySerial.print(strRoundResult);    // display result of round
}

// check if the game is over
void displayGameWinner() {
  int tempState = 6;   // assume winner
  
  if (playerScore == winsNeeded) {
    showHeaderNoScore();
    mySerial.print("Player Won " + String(playerScore) + "-" + String(compScore));
  }
  else if (compScore == winsNeeded) {
    showHeaderNoScore();
    mySerial.print("CPU won " + String(compScore) + "-" + String(playerScore));
  }
  else {
    tempState = 5;    // no winner
  }
  
  displayState = tempState;
}

// display header with score
void showHeader() {
  mySerial.write(12);    // clear
  
  // first line, display title and score
  mySerial.println(strTitle + " H:" + String(playerScore) + " C:" + String(compScore));
  mySerial.write(148);   // new line
}

// display header without score
void showHeaderNoScore() {
  mySerial.write(12);    // clear
  
  // first line, display title and score
  mySerial.println(strTitle);
  mySerial.write(148);   // new line
}

// reset the score
void resetScore()
{
  playerScore = 0;
  compScore = 0; 
}


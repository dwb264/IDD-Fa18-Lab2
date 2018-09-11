// include the library code:
#include <LiquidCrystal.h>
#include "pitches.h"

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int ledPin = 10;
int buttonPin = 13;
int buttonState = 0;
int lastButtonState;

long startTime;
long timeElapsed;
long timeLeft = -1;

bool timerStarted;

int currentRound = 0;
String currentRoundText;

int scale[] = {
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C6
};

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("8 Crazy Eights 8");
  lcd.setCursor(0,1);
  lcd.print("Push to start>>>");

}

void loop() {
  // put your main code here, to run repeatedly:

  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    
    if (buttonState == LOW) {

      // Reset timer and display
      timerStarted = true;  
      currentRound = 0;
      timeLeft = -1;
      lcd.clear();
      digitalWrite(ledPin, LOW);

      // Start the countdown
      countdown();
    
    }

    lastButtonState = buttonState;
    
  }

  if (timerStarted) {

    if (currentRound < 9) {

      if (timeLeft == -1) {
        // Start the next round
        lcd.clear();
        currentRound++;
        soundNotification(currentRound);
        startTime = millis();
      }

      // Show the timer for the current round
      showCurrentRound();
      showTimeLeft(60);
      
    } else {
      lcd.clear();
      timerStarted = false;
    }

  } else if (currentRound > 8) {
    // Alert when timer is stopped
    
    digitalWrite(ledPin, HIGH); // Turn on light
    lcd.setCursor(0,0);
    lcd.print("TIME'S UP!"); 
    lcd.setCursor(0,1);
    lcd.print("Push to restart>");
  }
  
}

// Countdown to starting the timer
void countdown() {
  tone(8, NOTE_C5, 250);
  lcd.print("Starting in 3...");
  delay(500);
  noTone(8);
  delay(500);
  lcd.clear();

  tone(8, NOTE_C5, 250);
  lcd.print("Starting in 2...");
  delay(500);
  noTone(8);
  delay(500);
  lcd.clear();

  tone(8, NOTE_C5, 250);
  lcd.print("Starting in 1...");
  delay(500);
  noTone(8);
  delay(500);
  lcd.clear();
}

// Display the current round on the first line
void showCurrentRound() {
  currentRoundText = "Round " + String(currentRound) + " of 8";
  lcd.setCursor(0,0);
  lcd.print(currentRoundText);
}


// Show the time left on the second line
void showTimeLeft(int roundDuration) {
  lcd.setCursor(0,1);
  timeElapsed = (millis() - startTime) / 1000;
  timeLeft = roundDuration - timeElapsed;
  lcd.print(String(timeLeft) + " "); 
}


// Play a tune to notify when the round changes
void soundNotification(int roundNum) {
  for (int thisNote = 0; thisNote < roundNum; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / 8;
    tone(8, scale[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}

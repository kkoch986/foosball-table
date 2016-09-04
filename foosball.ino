// Circuit Board Pins
// 1- PhotoCell 1 +5
// 2- Arduino A0
// 3- PhotoCell 1 Ground
// 4- PhotoCell 2 +5
// 5- Arduino A1
// 6- PhotoCell 2 Ground
// 7- Arduino D2
// 8- Buzzer +
// 9- Buzzer -
//10- Button +
//11- Button -
//12- Arduino D3
#include <Adafruit_NeoPixel.h>
#include "constants.h"
#include "buzzer.h"
#include "goal.h"
#include "lights.h"

// Score related
unsigned int red_score            = 0;
unsigned int blue_score           = 0;

// CONSTANTS
const unsigned short CLOCK_SPEED  = 1;
const unsigned short BTN_PRESSED  = 1;
const unsigned short BTN_OPEN     = 0;

// PIN ASSIGNMENTS
const int buzzer_pin              = 2;
const int button_pin              = 3;
const int led_pin                 = 6;
const int sensor_pins[]           = {0,1};

// Global vars
unsigned int button_state = BTN_OPEN;
Goal redGoal(sensor_pins[RED]);
Goal blueGoal(sensor_pins[BLUE]);
Buzzer buzzer;
Lights lights(led_pin);

void setup() {    
  // since the power supply has a capacitor in it, delay for some time 
  // to make sure there is light on the sensor when we calibrate
  delay(2000); 
  
  // to read the button digital_read(button_pin) will be high when not pressed and low when pressed.
  pinMode(button_pin, OUTPUT);
  digitalWrite(button_pin, HIGH);
  
  buzzer.setup(buzzer_pin);
  redGoal.calibrate();
  blueGoal.calibrate();

  bootSequence();
  delay(500);
}

void loop() {
    lights.tick(red_score, blue_score);
    checkForGoals();
    buzzer.check();
    buttonCheck();
    
    // Advance the clock
    delay(CLOCK_SPEED);
}

void buttonCheck() {
  int newState = digitalRead(button_pin);
  if(newState == BTN_PRESSED && button_state == BTN_OPEN) {
    newGame();
  }
  button_state = newState;
}

void newGame() {  
  // Reset the score
  buzzer.playSiren();
  lights.playNewGame(red_score, blue_score);
  red_score = 0;
  blue_score = 0;
}

/**
 * Boot sequence, played when we first boot up.
 **/
void bootSequence() {
  buzzer.playBootSequence();
  buzzer.playSiren();
  lights.allOff();
  lights.playBootSequence();
}

/**
 * Check for a goal being scored.
 **/
void checkForGoals() {

  if(redGoal.check()) {
    red_score++;
    buzzer.playSiren();
  }

  if(blueGoal.check()) {
    blue_score++;
    buzzer.playSiren();
  }
  
  // Check for a win here...
  // TODO: play an animation for the winning team and maybe a song...
  if(blue_score >= MAX_SCORE || red_score >= MAX_SCORE) {
    newGame();
  }
}
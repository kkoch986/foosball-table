
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include "lights.h"
#include "constants.h"


uint32_t Wheel(byte WheelPos);

Lights::Lights(unsigned int pin_number) {
	led_pin 			= pin_number;
	last_red_score 		= 100;
	last_blue_score 	= 100;
	currentSequence 	= kSeqDrawScore;
	currentFrame 		= 0;
	winner				= NO_ONE;
	strip = Adafruit_NeoPixel(NUM_PIXELS, led_pin, NEO_GRB + NEO_KHZ800);
	strip.begin();
}

void Lights::tick(unsigned int red_score, unsigned int blue_score) {
	switch(currentSequence) {
		case kSeqDrawScore: 
			currentFrame = 0; // we dont want to advance the current frame (risk of overflow if running forever)
			scoreboardSequence(red_score, blue_score);
			break ;
		case kSeqBoot: 
		case kSeqNewGame: 
			bootSequence(red_score, blue_score); break;
			// newGameSequence(red_score, blue_score); break;
	}
	currentFrame++;
}

void Lights::playBootSequence() {
	allOff();
	currentSequence = kSeqBoot;
	currentFrame = 0;
}

void Lights::playNewGame(unsigned int red_score, unsigned int blue_score) {
	allOff();
	currentSequence = kSeqNewGame;
	currentFrame = 0;
	if(red_score > blue_score) {
		winner = RED;
	} else if(blue_score > red_score) {
		winner = BLUE;
	} else {
		winner = NO_ONE;
	}
}

void Lights::scoreboardSequence(unsigned int red_score, unsigned int blue_score) {
	if(red_score != last_red_score || blue_score != last_blue_score) {
		drawScore(red_score, blue_score);
		last_blue_score = blue_score;
		last_red_score = red_score;
	}
}

void Lights::newGameSequence(unsigned int red_score, unsigned int blue_score) {
	unsigned int sweepSpeed = 100;
	unsigned int iterations = 10;
	unsigned int numPixels 	= strip.numPixels();
	unsigned int iterationLength = numPixels;

	// figure out what color to do the wipe in
	uint32_t color = strip.Color(255,255,255);
	if(winner == RED) {
		color = strip.Color(255,0,0);
	} else if(winner == BLUE) {
		color = strip.Color(0,0,255);
	}

	// Tell if we're in the sweep portion of the animation
	if(currentFrame < sweepSpeed * iterations * numPixels) {
		unsigned int prevPos = (currentFrame - 1) / sweepSpeed;
		unsigned int pos = (currentFrame / sweepSpeed);
		if(prevPos != pos) {
			pos = pos % iterationLength;
			for(unsigned int x = 0 ; x < numPixels / 2 ; x++) {
				if(pos <= numPixels / 4) {
					if(x <= pos) {
						strip.setPixelColor(x, color);
						strip.setPixelColor(map(
							x, 
							0, 
							(numPixels / 4), 
							(numPixels / 2) - 1, 
							(numPixels / 4)
						), color);
					}
				} else if(pos > numPixels / 4 && pos <= numPixels / 2) {
					if(x <= pos) {
						strip.setPixelColor(x, strip.Color(0,0,0));
						strip.setPixelColor(map(
							x, 
							0, 
							(numPixels / 4), 
							(numPixels / 2) - 1, 
							(numPixels / 4)
						), strip.Color(0,0,0));
					}
				}
			}

			strip.show();
		}

	} else {
		currentFrame = 0;
		currentSequence = kSeqDrawScore;
		drawScore(red_score, blue_score);
		return ;
	}
}

/**
 * Handle each frame of the boot sequence.
 **/
void Lights::bootSequence(unsigned int red_score, unsigned int blue_score) {
	unsigned int speed 			= 40;
	unsigned int gap 			= 3;
	unsigned long stopAfter 	= 2600;

	if(currentFrame >= stopAfter) {
		currentFrame = 0;
		currentSequence = kSeqDrawScore;
		drawScore(red_score, blue_score);
		return ;
	}

	int offset = ((int)floor(currentFrame / speed)) % gap;
	int previousOffset = ((int)floor((currentFrame - 1) / speed)) % gap;
	if(offset != previousOffset) {
		for(unsigned int x = 0; x < strip.numPixels(); x++) {
			if( (x+offset) % gap == 0 ) {
				strip.setPixelColor(x, Wheel(map(currentFrame, 0, stopAfter, 0, 1024)));
			} else {
				strip.setPixelColor(x, strip.Color(0,0,0));
			}
		}
		strip.show();
	}
}

/**
 * Update the LEDs to reflect the current game score.
 **/
void Lights::drawScore(unsigned int red_score, unsigned int blue_score) {
  // set reds right and left colors
  strip.setPixelColor(RED_LEFT, strip.Color(255, 0, 0));
  strip.setPixelColor(RED_RIGHT, strip.Color(255, 0, 0));
 
  // set BLUEs right and left colors
  strip.setPixelColor(BLUE_LEFT, strip.Color(0, 0, 255));
  strip.setPixelColor(BLUE_RIGHT, strip.Color(0, 0, 255));
  
  // now loop over the max points and set both teams scoreboards
  for(unsigned int x = 0; x < MAX_SCORE ; x++) {
    // red
    if(red_score > x) {
      strip.setPixelColor(RED_SCOREBOARD_LIGHTS[x], strip.Color(255,0,0));
   } else {
      strip.setPixelColor(RED_SCOREBOARD_LIGHTS[x], strip.Color(0,0,0));
   }
    
    // BLUE
    if(blue_score > x) {
      strip.setPixelColor(BLUE_SCOREBOARD_LIGHTS[x], strip.Color(0,0,255));
    } else {
      strip.setPixelColor(BLUE_SCOREBOARD_LIGHTS[x], strip.Color(0,0,0));
    }
  }

  strip.show();
}

void Lights::allOff() {
	for(unsigned int i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, strip.Color(0,0,0));
	}
	strip.show();
}


uint32_t Lights::Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

#include <Adafruit_NeoPixel.h>

#define NUM_PIXELS 25

// different sequence numbers
#define kSeqDrawScore 		0
#define kSeqBoot	 		1
#define kSeqNewGame	 		2

// Pin assignments
const unsigned short BLUE_LEFT   = 0;
const unsigned short BLUE_RIGHT  = 11;
const unsigned short BLUE_SCOREBOARD_LIGHTS[] = {1,2,3,4,5,6,7,8,9,10};

const unsigned short RED_LEFT   = 12;
const unsigned short RED_RIGHT  = 23;
const unsigned short RED_SCOREBOARD_LIGHTS[] = {13,14,15,16,17,18,19,20,21,22};

class Lights {
	private:
		Adafruit_NeoPixel strip;
		unsigned int led_pin;
		unsigned int last_red_score;
		unsigned int last_blue_score;
		unsigned short currentSequence;
		unsigned long currentFrame;
		unsigned short winner;

		void scoreboardSequence(unsigned int red_score, unsigned int blue_score);
		void bootSequence(unsigned int red_score, unsigned int blue_score);
		void newGameSequence(unsigned int red_score, unsigned int blue_score);
		uint32_t Wheel(byte WheelPos);

	public:
		Lights(unsigned int pin_number);
		void tick(unsigned int red_score, unsigned int blue_score);

		void drawScore(unsigned int red_score, unsigned int blue_score);
		void allOff();
		void playBootSequence();
		void playNewGame(unsigned int red_score, unsigned int blue_score);
};


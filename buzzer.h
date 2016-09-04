
#define SIREN_SPEED  		10
#define SIREN_START  		300
#define SIREN_STOP  		1000
#define SIREN_INCREMENT  	10
#define SIREN_LOOPS  		4

class Buzzer {
	public:
		void setup(unsigned int pin_number);
		void check();
		void playSiren();
		void playBootSequence();
		void newGame();
};
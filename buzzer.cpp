#include <Arduino.h>
#include "notes.h"
#include "buzzer.h"

// SIREN BUZZER sequence
const unsigned int SIREN_LENGTH = (((SIREN_STOP - SIREN_START) / SIREN_INCREMENT) * SIREN_LOOPS);
unsigned int siren_frequencies[SIREN_LENGTH];
unsigned int siren_durations[SIREN_LENGTH];

// BOOT BUZZER SEQUENCE
const unsigned int BOOT_SEQ_LENGTH = 7;
unsigned int boot_seq_frequencies[] = {
	NOTE_D5, 0, NOTE_C5, NOTE_F5, 0, NOTE_G5, NOTE_A4
};
unsigned int boot_seq_durations[] = {
	100, 100, 100, 100, 100, 100, 100
};

// NEW GAME BUZZER SEQUENCE
const unsigned int NEW_GAME_LENGTH = 7;
unsigned int new_game_frequencies[] = {
	NOTE_E5, 0, NOTE_E5, NOTE_E5, 0, NOTE_E5, NOTE_E4
};
unsigned int new_game_durations[] = {
	50, 50, 50, 50, 50, 50, 50
};

// Keep the state of the buzzer
unsigned short buzzer_on = 0;
unsigned int *current_frequencies;
unsigned int *current_durations;
unsigned int current_sequence_length;
unsigned int current_position;
unsigned int buzzer_clock;
unsigned int waiting = 0;
unsigned int buzzer_pin;

/**
 * Initialize the buzzer stuff, pass in the pin number
 **/
void Buzzer::setup(unsigned int pin_number) {
	buzzer_pin = pin_number;

	pinMode(buzzer_pin, OUTPUT);

	// Fill out the siren sequence
	int current_freq = SIREN_START;
	for(unsigned int i = 0; i < SIREN_LENGTH ; i++) {
		siren_frequencies[i] = current_freq;
		siren_durations[i] = SIREN_SPEED;

		current_freq += SIREN_INCREMENT;
		if(current_freq >= SIREN_STOP) {
			current_freq = SIREN_START;
		}
	}
}

/**
* Buzzer Related Stuff...
**/
void Buzzer::check() {
	if(current_position > current_sequence_length) {
		buzzer_on = 0;
		current_frequencies = NULL;
		current_durations = NULL;
		buzzer_clock = 0;
		current_sequence_length = 0;
		current_position = 0;
		waiting = 0;
	} else if(buzzer_on == 1 && current_position < current_sequence_length) {
		unsigned int freq = *(current_frequencies + (current_position));
		unsigned int duration = *(current_durations + (current_position));
		if(buzzer_clock == duration) {
			tone(buzzer_pin, freq, duration);
			current_position++;
			buzzer_clock = 0;
		}
	}
	buzzer_clock++;
}

void Buzzer::playSiren() {
	current_position = 0;
	buzzer_clock = 0;
	current_frequencies = &siren_frequencies[0];
	current_durations = &siren_durations[0];
	current_sequence_length = SIREN_LENGTH;
	buzzer_on = 1;
}

void Buzzer::playBootSequence() {
	for(unsigned int i = 0 ; i < BOOT_SEQ_LENGTH; i++) {
		tone(buzzer_pin, boot_seq_frequencies[i], boot_seq_durations[i]);
		delay(boot_seq_durations[i]);
	}
}

void Buzzer::newGame() {
	// play the new game music
	current_position = 0;
	buzzer_clock = 0;
	current_frequencies = &new_game_frequencies[0];
	current_durations = &new_game_durations[0];
	current_sequence_length = NEW_GAME_LENGTH;
	buzzer_on = 1;
}

#include <Arduino.h>
#include "goal.h"

Goal::Goal(unsigned int _pin_number) {
	pin_number = _pin_number;
	status = STATUS_NORMAL;
}

void Goal::calibrate() {
	int total = 0;
	for(unsigned int r = 0 ; r < CALIBRATE_SIZE ; r++) {
	total += analogRead(pin_number);
		delay(CALIBRATE_DELAY);
	}

	expectation = total / CALIBRATE_SIZE;
}

bool Goal::check() {
	int reading = this->readSensor();
	if(status == STATUS_NORMAL) {
		if(reading == STATUS_OPEN) {
			status = reading;
			return true;
		}
	}
	status = reading;
	return false;
}

/**
 * Read a given sensor and compare to its expected value
 * returns 1 if ON (dark) 0 if OFF (light), returns -1
 * if POS is outside the number of sensors.
 **/
int Goal::readSensor() {
	int reading = analogRead(pin_number);

	// Compute the acceptable ranges
	// TODO: cache these in an array during calibration
	float windowSize = (expectation * ACCEPTABLE_PCT);
	float bottomLine = expectation - windowSize;
	float topLine = expectation + windowSize;

	// If the reading falls below the acceptable range of calibrated value
	// return that it is on.
	if(reading <= bottomLine) {
		return STATUS_OPEN;
	}

	// If the reading falls above the range, recalibrate and return that it is off
	// TODO: only recalibrate this specific sensor since each full calibration 
	// requires CALIBRATE_DELAY * CALIBRATE_SIZE * NUM_SENSORS ms to complete.
	// which would be an unacceptable delay while playing (10 * 10 * 2 = 200ms)
	if(reading >= topLine) {
		this->calibrate();
	}

	// Return that its off.
	return STATUS_NORMAL;
}
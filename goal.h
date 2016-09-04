

const unsigned short STATUS_NORMAL = 1;
const unsigned short STATUS_OPEN = 0;

const unsigned int CALIBRATE_SIZE = 10;
const unsigned int CALIBRATE_DELAY = 10;
const float ACCEPTABLE_PCT = 0.4;

class Goal {
	unsigned int pin_number;
	unsigned int expectation;
	unsigned int status;

	public:
		Goal(unsigned int pin_number);
		void calibrate();
		bool check(); // return true if the sensor is open and was not on the last check

	private:
		int readSensor();
};
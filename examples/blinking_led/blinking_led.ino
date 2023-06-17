#include <Arduino.h>
#include "StateMachine.hpp"

class LED: StateMachine {

private:

	static const int ON = 0;
	static const int OFF = 1;

	// PIN used by the LED
	int pin;

	// time in ms to led the LED on / off
	int blinkingTime;

public:
	LED(int pin, int blinkingTime) {
		this->pin = pin;
		this->blinkingTime = blinkingTime;
		this->state = ON;
	}

	// do what we normally do in setup(): setting PIN modes etc.
	void init() override {
		digitalWrite(pin, LOW);
		pinMode(pin, OUTPUT);
	}

	// do the magic
	void step() override {

		// if state is ON: light up and wait some time :)
		if (this->state == ON) {
			digitalWrite(this->pin, HIGH);
			this->wait(this->blinkingTime, OFF);
		} else {
			// otherwise: set light off and wait again!
			digitalWrite(this->pin, LOW);
			this->wait(blinkingTime, ON);
		}
	}
};

//define our leds
LED led1(13, 300);
LED led2(14, 1000);

// initialize all known stateMachines
void setup() {
	StateMachine::registry->init();
}

void loop() {
	// let the magic start
	StateMachine::registry->nextStep();
}

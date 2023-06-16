#include <Arduino.h>
#include "StateMachine.hpp"

class LED: StateMachine {

private:

	static const int ON = 0;
	static const int OFF = 1;

	int pin;

public:
	LED(int pin) {
		this->pin = pin;
		this->state = ON;
	}

	void init() override {
		digitalWrite(pin, LOW);
		pinMode(pin, OUTPUT);
	}

	void step() override {
		if (this->state == ON) {
			digitalWrite(this->pin, HIGH);
			this->wait(1000, OFF);
		} else {
			digitalWrite(this->pin, LOW);
			this->wait(2000, ON);
		}
	}
};

void setup() {
	new LED(13);
	StateMachine::registry->init();
}

void loop() {
	StateMachine::registry->nextStep();
}

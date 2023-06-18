#include <Arduino.h>
#include "StateMachine.hpp"

class LED: public StateMachine {

private:

	static const int SWITCH_ON = 1;
	static const int ON = 2;
	static const int SWITCH_OFF = 3;
	static const int OFF = 4;

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
		if (this->state == SWITCH_ON) {
			this->state = ON;
			digitalWrite(this->pin, HIGH);
			this->wait(1000, SWITCH_OFF);
		}
		else if (this->state == SWITCH_OFF) {
			this->state = OFF;
			digitalWrite(this->pin, LOW);
		}
	}

	bool isAvailable() override {
		return this->state == OFF;
	}

	void switchOn() {
		this->state = SWITCH_ON;
	}
};

class LEDControl : StateMachine {

private:
	static const int SWITCH_LED_ON = 0;
	static const int IDLE = 1;

	LED* led;

public:
	LEDControl(LED* led) {
		this->led = led;
	}

	void step() override {
		if (this->state == SWITCH_LED_ON) {
			this->led->switchOn();
			this->waitFor(this->led, IDLE);
		}
		else if (this->state == IDLE) {
			this->wait(2000, SWITCH_LED_ON);
		}
	}
};

	LED led(13);
	LEDControl control(&led);

void setup() {

	StateMachine::registry->init();
}

void loop() {
	StateMachine::registry->nextStep();
}

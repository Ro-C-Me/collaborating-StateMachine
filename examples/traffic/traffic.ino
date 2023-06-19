#include <Arduino.h>
#include "StateMachine.hpp"


/**
 * this class models a traffic light with 3 leds (red, yellow and green)
 */
class TrafficLight: public StateMachine {

private:

	static const int RED = 0;
	static const int RED_YELLOW = 1;
	static const int GREEN = 2;
	static const int YELLOW = 3;

	// PIN used by traffic lights LED's
	uint8_t redPin;
	uint8_t yellowPin;
	uint8_t greenPin;

public:
	TrafficLight(uint8_t redPin, uint8_t yellowPin, uint8_t greenPin) {
		this->redPin = redPin;
		this->yellowPin = yellowPin;
		this->greenPin = greenPin;
	}

	// do what we normally do in setup(): setting PIN modes etc.
	void init() override {
		digitalWrite(redPin, LOW);
		pinMode(redPin, OUTPUT);

		digitalWrite(yellowPin, LOW);
		pinMode(yellowPin, OUTPUT);

		digitalWrite(greenPin, LOW);
		pinMode(greenPin, OUTPUT);
	}

	// do the magic
	void step() override {

		if (this->state == RED) {
			digitalWrite(redPin, HIGH);
			digitalWrite(yellowPin, LOW);
			digitalWrite(greenPin, LOW);
			Serial.println("Stop!");
			this->wait(2000, RED_YELLOW);
		} else if (this->state == RED_YELLOW) {
			digitalWrite(redPin, HIGH);
			digitalWrite(yellowPin, HIGH);
			digitalWrite(greenPin, LOW);
			Serial.println("Get ready!");
			this->wait(500, GREEN);
		} else if (this->state == GREEN) {
			digitalWrite(redPin, LOW);
			digitalWrite(yellowPin, LOW);
			digitalWrite(greenPin, HIGH);
			Serial.println("Go!");
			this->wait(4000, YELLOW);
		} else if (this->state == YELLOW) {
			digitalWrite(redPin, LOW);
			digitalWrite(yellowPin, HIGH);
			digitalWrite(greenPin, LOW);
			Serial.println("Prepare for stop!");
			this->wait(500, RED);
		}
	}

	/**
	 * our traffic is "avilable" when it shows green
	 */
	bool isAvailable() override {
		return this->state == GREEN;
	}
};

class Cars : public StateMachine {

private:

	static const int WAITING = 0;
	static const int PROCEED = 1;
	static const int DRIVING = 2;
	static const int BRAKING = 3;


	TrafficLight* trafficLight;



public:

	Cars(TrafficLight* trafficLight) {
		this->trafficLight = trafficLight;
	}

	void step() override {
		if (this->state == WAITING) {
			this->waitFor(trafficLight, PROCEED);
		}
		if (this->state == PROCEED) {
			this->state = DRIVING;
			Serial.println("wrooommm");
		}

		if (this->state == DRIVING) {
			// when driving: check if traffic light still green
			if (!this->trafficLight->isAvailable()) {
				// if not - BREAK
				Serial.println("noticed a non-green traffic light!");
				this->state = BRAKING;
			}
		}
		else if (this->state == BRAKING) {
			Serial.println("queeeek");
			this->state = WAITING;
			this->waitFor(trafficLight, PROCEED);
		}
	}
};

//define our leds
TrafficLight trafficlight(13, 14, 15);

Cars cars(&trafficlight);

// initialize all known stateMachines
void setup() {
	Serial.begin(9600);
	StateMachine::registry->init();
}

void loop() {
	// let the magic start
	StateMachine::registry->nextStep();
}

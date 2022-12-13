#include "StateMachine.hpp"

#ifdef testing
#include <iostream>
#include "Testing.hpp"
#else
#include <Arduino.h>
#endif

Registry *StateMachine::registry = new Registry();

void StateMachine::waiting(int time) {
	this->waitUntil = millis() + long(time);
}

int StateMachine::nextStep() {
	if (this->waitUntil < millis()) {
		this->step();
	}

	return this->state;
}

StateMachine::StateMachine() {

	if (StateMachine::registry == NULL) {
		StateMachine::registry = new Registry();
	}

	StateMachine::registry->add(this);
}

bool Registry::add(StateMachine *machine) {

	if (this->nrOfMachines < MAX_NR_OF_MACHINES) {
		this->machines[this->nrOfMachines++] = machine;
		return true;
	} else {
		return false;
	}
}

void Registry::nextStep() {
	for (int i = 0; i < this->nrOfMachines; i++) {
		this->machines[i]->nextStep();
	}
}

#ifdef testing

class WaitMachine : public StateMachine {

private:

	const int FINISHED_WAITING = 1;

	const int WAITING = 2;

	int interval = 0;

	public:
	WaitMachine(int interval) {
		this->interval = interval;
		this->state = 2;
	}

	void step() override {
		if (this->state == WAITING) {
			std::cout << std::string("Waiting finished, will do again(") + std::to_string(this->interval) + ")\n";
			this->state = FINISHED_WAITING;
		}

		else {
			this->waiting(this->interval);
			this->state = WAITING;
		}
	}

	bool isAvailable() override {
		return this->state == FINISHED_WAITING;
	}
};

class WaitForMachine : public StateMachine {

private:
	StateMachine* toWaitFor;

public:
		WaitForMachine(StateMachine* toWaitFor) {
			this->toWaitFor = toWaitFor;
		}

		void step() override {
			if (this->toWaitFor->isAvailable()) {
				std::cout << std::string("It's available\n");
			}
		}

		bool isAvailable() override {
			return false;
		}
};

int main() {

 WaitMachine w1(1000);
 WaitForMachine w2(&w1);

 while (true) {
	StateMachine::registry->nextStep();
//	registry.nextStep();
 }

return 0;
}

#endif

#include "StateMachine.hpp"

#ifdef testing
#include <iostream>
#include "Testing.hpp"
#else
#include <Arduino.h>
#endif

Registry *StateMachine::registry = new Registry();

void StateMachine::wait(int time, int stateAfter) {
	this->waitUntil = millis() + long(time);
	this->stateAfterWaiting = stateAfter;
}

void StateMachine::waitFor(StateMachine* machine, int stateAfter) {
	this->machineToWaitFor = machine;
	this->stateAfterWaiting = stateAfter;
}
int nextStepsCalled = 0;

void StateMachine::nextStep() {
	nextStepsCalled++;
	// waiting some time?
	if (this->waitUntil > 0) {
		// waiting finished?
		unsigned long tmpWaitUntil = this->waitUntil;
		unsigned long tmpMillis =  millis();

		if (this->waitUntil < tmpMillis) {
			this->waitUntil = 0;
			this->state = this->stateAfterWaiting;
		}
	}

	// waiting for a machine?
	else if (this->machineToWaitFor) {
		// machine available?
		if (this->machineToWaitFor->isAvailable()) {
			this->machineToWaitFor = nullptr;
			this->state = this->stateAfterWaiting;
			this->step();
		}
	} else {
		// no waiting - just do it :D
		this->step();
	}

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
		this->state = 1;
	}

	void step() override {
		if (this->state == FINISHED_WAITING) {
			std::cout << std::string("Waiting finished\n");
			this->state = WAITING;
		}
		else {
			std::cout << std::string("Will wait again(") + std::to_string(this->interval) + ")\n";
			this->wait(this->interval, FINISHED_WAITING);
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
			std::cout << std::string("It's available\n");
			this->waitFor(toWaitFor, this->state);
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
 }

return 0;
}

#endif

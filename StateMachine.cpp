#include "StateMachine.hpp"

#ifdef testing
#include <iostream>
#include "tests/ArduinoMock.hpp"
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

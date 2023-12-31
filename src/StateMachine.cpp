#include "StateMachine.hpp"
#include <Arduino.h>

Registry *StateMachine::registry = new Registry();

void StateMachine::wait(int time, int stateAfter) {
	this->waitUntil = millis() + long(time);
	this->stateAfterWaiting = stateAfter;
}

void StateMachine::waitFor(StateMachine* machine, int stateAfter) {
	this->machineToWaitFor = machine;
	this->stateAfterWaiting = stateAfter;
}

void StateMachine::nextStep() {

	// waiting some time?
	if (this->waitUntil > 0) {

		if (this->waitUntil < millis()) {
			this->waitUntil = 0;
			this->state = this->stateAfterWaiting;
			this->step();
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

void StateMachine::init() {}
bool StateMachine::isAvailable() {return true;}

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

void Registry::init() {
	for (int i = 0; i < this->nrOfMachines; i++) {
		this->machines[i]->init();
	}
}

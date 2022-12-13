#include "StateMachine.hpp"

#ifdef testing
#include <iostream>
#include "Testing.hpp"
#else
#include <Arduino.h>
#endif


Registry* StateMachine::registry = new Registry();

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
	}
	else {
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
	int interval = 0;

	public:
	WaitMachine(int interval) {
		this->interval = interval;
	}

	void step() override {
		std::cout << std::string("Waiting finished, will do again(") + std::to_string(this->interval) + ")\n";
		this->waiting(this->interval);
	}
};



int main() {

 WaitMachine w1(1000);
 WaitMachine w2(5000);
 WaitMachine w3(6000);

 while (true) {
	StateMachine::registry->nextStep();
//	registry.nextStep();
 }

return 0;
}

#endif

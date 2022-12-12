#include "StateMachine.hpp"

#ifdef testing
#include <iostream>
#include "Testing.hpp"
#else
#include <Arduino.h>
#endif

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
std::cout << "Hello, StateMachine\n";

 WaitMachine w1(1000);
 WaitMachine w2(5000);
 WaitMachine w3(5000);

 Registry registry;

 std::cout << std::to_string(registry.add(&w1)) + "\n";
 std::cout << std::to_string(registry.add(&w2)) + "\n";
 std::cout << std::to_string(registry.add(&w3)) + "\n";

 while (true) {
	registry.nextStep();
 }

return 0;
}

#endif

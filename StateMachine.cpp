#include "StateMachine.hpp"

#ifdef testing
#include <iostream>
#include "Testing.hpp"
#endif



void StateMachine::waiting(int time) {
 this->waitUntil = millis() + long(1000);
}

int StateMachine::nextStep() {
	 if (this->waitUntil < millis()) {
		 this->step();
	 }

	 return this->state;
}

StateMachine::StateMachine() {

}

#ifdef testing

class WaitMachine : public StateMachine {

	public:

	void step() override {
		std::cout << "Waiting finished, will do again\n";
		this->waiting(1000);
	}
};



int main() {
std::cout << "Hello, StateMachine\n";

 WaitMachine w;

 while (true) {
	 w.nextStep();
 }

return 0;
}

#endif

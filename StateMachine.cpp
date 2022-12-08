#include "StateMachine.hpp"

#ifdef testing
#include <iostream>
#include "Testing.hpp"
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

 while (true) {
	 w1.nextStep();
	 w2.nextStep();
 }

return 0;
}

#endif

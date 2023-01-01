#ifdef testing
#include "ArduinoMock.hpp"

#include "../StateMachine.hpp"

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

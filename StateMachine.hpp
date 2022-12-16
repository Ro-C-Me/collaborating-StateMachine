class StateMachine;

class Registry {

private:
	static const int MAX_NR_OF_MACHINES = 2;

	int nrOfMachines = 0;
	StateMachine* machines[MAX_NR_OF_MACHINES];

public:
	bool add(StateMachine* machine);
	void nextStep();
};


class StateMachine {

private:

	unsigned long waitUntil = 0;

	StateMachine* machineToWaitFor = nullptr;

	int stateAfterWaiting = 0;

protected:

	int state = 0;

	virtual void step() = 0;

public:

	static Registry* registry;

	StateMachine();

	virtual bool isAvailable() = 0;

	void wait(int time, int stateAfter);

	void waitFor(StateMachine* machine, int stateAfter);

	void nextStep();



};


class StateMachine {

private:
	unsigned long waitUntil = 0;

protected:
	int state = 0;

	virtual void step() = 0;

public:

	StateMachine();

	void waiting(int time);

	int nextStep();

};

class Registry {

private:
	static const int MAX_NR_OF_MACHINES = 2;

	int nrOfMachines = 0;
	StateMachine* machines[MAX_NR_OF_MACHINES];

public:
	bool add(StateMachine* machine);
	void nextStep();
};

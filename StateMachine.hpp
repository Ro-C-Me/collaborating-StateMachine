class StateMachine {

private:
	unsigned long waitUntil = 0;

protected:
	int state = 0;

public:

	StateMachine();

	void waiting(int time);

	int nextStep();

	virtual void step() = 0;

};

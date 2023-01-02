#ifdef testing
#include <chrono>
#include <iostream>

const int LOW = 0;
const int HIGH = 1;
const int INPUT = 2;
const int OUTPUT = 3;


static unsigned long millis() {
	 std::chrono::time_point<std::chrono::system_clock> now =
	     std::chrono::system_clock::now();
	 auto duration = now.time_since_epoch();
	 auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	 return long(millis);
}

static void digitalWrite(int pin, int val) {
	std::string s;

	if (val == LOW) {
		s = "LOW";
	}
	else if (val == HIGH) {
		s = "HIGH";
	}
	else {
		s = "INVALID VALUE";
	}
	std::cout << millis() % 100000 << std::string(" digitalWrite: pin " + std::to_string(pin) + " -> " + s + "\n");
}

static void pinMode(int pin, int mode) {
	std::string s;

	if (mode == INPUT) {
		s = "INPUT";
	}
	else if (mode == OUTPUT) {
		s = "OUTPUT";
	}
	else {
		s = "INVALID VALUE";
	}
	std::cout << std::string("pinMode: pin " + std::to_string(pin) + " -> " + s + "\n");
}

#endif

# waiting-StateMachine

In many simple applications, delay() is used whenever the program should wait some time. For example, most Arduino beginner tutorials start with a blinking LED and therefore a delay between ON and OFF and another one betweeen OFF and ON. 

Things are getting more complicated when two LEDs should blink, each with an own frequency.
Operating systems offer threads for such cases, but on microcontrollers like Arduino, we don't have such a comfort - we have to help ourselves. One idea could be to remember the current state of each LED and the time for the next switch. Implementing this for two LEDs direclty in our sketch seems to be possible, but when we add some more LEDs or something completely different like a motor, a sensor which has to be monitored etc., our code will become ugly at some point.

But keeping the current state (e.g. ON and OFF for the LED example) and the possible transitions between the available states is a well known concept called "State Machine". This library offers a base class "StateMachine" and a Registry which automatically remembers all (ok, not all, but up to 256 ^^ ) derived StateMachine instances.

The registry will call the step() function of every StateMachine once in a loop() call. When implementing a StateMachine, you should keep the time required for a step() call as small as possible, because a delay in one StateMachine's step() will result in all other StateMachines being called later. Of course, delay() calls are a really bad idea, but there are many other things which can slow down the whole process like waiting for the Serial port etc. 

Complete examples can be found in the examples section, I suggest to start with the blinking led.

The StateMachine class provides to waiting functions:

**void wait(int time, int stateAfter);**

The step() function of this instance will called after the given time (in ms). Before the call, the state will be changed to the given state.

```C++
...
this->state = WAITING;
this->wait(10000, DO_STH);
...
```
will keep the state WAITING for the next 10s, but the step() function won't be called in this time. After 10s, the state will be changed to DO_STH automatically, before the step() is called again. Of course, other StateMachine instances' step() will be called in the meantime.

**void waitFor(StateMachine* machine, int stateAfter);**

```C++
...
this->state = WAITING;
this->relay.pull(1000);
this->wait(this->relay, DO_STH);
...
```
In this case, we instruct a relay (implemented as a StateMachine) to pull for a second and then wait until it is available again. If it is done, we want to set our own state to "DO_STH" before continuing. For this, it is obviously important for the relay to tell us if it's still pulling by overriding it's 

**bool isAvailable()**

in the case of a relay, we should implement it like

```C++
bool isAvailable() override {
return this->state != PULLING;
}
```


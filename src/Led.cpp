#include <Arduino.h>
#include "Led.h"

LED::LED(int pin) : pin(pin) {}

void LED::begin() {
	pinMode(pin, OUTPUT);
	off();
}

void LED::on() {
	digitalWrite(pin, HIGH);
}

void LED::off() {
	digitalWrite(pin, LOW);
}

void LED::toggle() {
	digitalWrite(pin, !digitalRead(pin));
}

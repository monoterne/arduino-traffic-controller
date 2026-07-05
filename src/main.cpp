#include <Arduino.h>
#include "Led.h"

namespace { // D2-D3-D4
    LED yellowLED(2);
    LED redLED(3);
    LED greenLED(4);    

    void disableAllLEDs() {
        yellowLED.off();
        redLED.off();
        greenLED.off();
    }
}

void setup() {
    Serial.begin(115200);

    Serial.println("Booting...");

    yellowLED.begin();
    redLED.begin();
    greenLED.begin();

    disableAllLEDs();
}

void loop() {
    yellowLED.on();
    redLED.off();
    greenLED.off();
    delay(500);

    yellowLED.off();
    redLED.on();
    greenLED.off();
    delay(500);

    yellowLED.off();
    redLED.off();
    greenLED.on();
    delay(500);

    disableAllLEDs();
    delay(500);
}

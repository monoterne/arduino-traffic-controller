#include <Arduino.h>

//                                         D
constexpr uint8_t PEDESTRIAN_BUTTON_PIN  = 5;
constexpr uint8_t MAINTENANCE_BUTTON_PIN = 6;
constexpr uint8_t EMERGENCY_BUTTON_PIN   = 7;

//                                 D
constexpr uint8_t YELLOW_LED_PIN = 2;
constexpr uint8_t RED_LED_PIN    = 3;
constexpr uint8_t GREEN_LED_PIN  = 4;

enum class TrafficState
{
    Red,
    Yellow,
    Green
};

TrafficState currentState = TrafficState::Red;

unsigned long previousMillis = 0;
constexpr unsigned long INTERVAL = 1000;

void setup() {
    Serial.begin(115200);

    pinMode(PEDESTRIAN_BUTTON_PIN, INPUT_PULLUP);
    pinMode(MAINTENANCE_BUTTON_PIN, INPUT_PULLUP);
    pinMode(EMERGENCY_BUTTON_PIN, INPUT_PULLUP);

    pinMode(YELLOW_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);

    Serial.println("Traffic Controller Booting...");
}

void loop() {

    if (digitalRead(PEDESTRIAN_BUTTON_PIN) == LOW) {
        Serial.println("[BUTTON] D5");
    }

    if (digitalRead(MAINTENANCE_BUTTON_PIN) == LOW) {
        Serial.println("[BUTTON] D6");
    }

    if (digitalRead(EMERGENCY_BUTTON_PIN) == LOW) {
        Serial.println("[BUTTON] D7");
    }

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= INTERVAL) {
        previousMillis = currentMillis;
        switch (currentState) {
        case TrafficState::Red:
            digitalWrite(RED_LED_PIN, HIGH);
            digitalWrite(YELLOW_LED_PIN, LOW);
            digitalWrite(GREEN_LED_PIN, LOW);

            // Serial.println("RED");

            currentState = TrafficState::Green;
            break;

        case TrafficState::Green:
            digitalWrite(RED_LED_PIN, LOW);
            digitalWrite(YELLOW_LED_PIN, LOW);
            digitalWrite(GREEN_LED_PIN, HIGH);

            // Serial.println("GREEN");

            currentState = TrafficState::Yellow;
            break;

        case TrafficState::Yellow:
            digitalWrite(RED_LED_PIN, LOW);
            digitalWrite(YELLOW_LED_PIN, HIGH);
            digitalWrite(GREEN_LED_PIN, LOW);

            // Serial.println("YELLOW");

            currentState = TrafficState::Red;
            break;
        }
    }   
}

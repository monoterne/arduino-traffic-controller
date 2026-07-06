#include <Arduino.h>

//                                         D
constexpr uint8_t PEDESTRIAN_BUTTON_PIN  = 5;
constexpr uint8_t MAINTENANCE_BUTTON_PIN = 6;
constexpr uint8_t EMERGENCY_BUTTON_PIN   = 7;

//                                 D
constexpr uint8_t YELLOW_LED_PIN = 2;
constexpr uint8_t RED_LED_PIN    = 3;
constexpr uint8_t GREEN_LED_PIN  = 4;

enum class TrafficState {
    Red,
    Yellow,
    Green
};

TrafficState current_state = TrafficState::Red;

unsigned long previous_millis = 0;
constexpr unsigned long INTERVAL = 1000;
constexpr unsigned long DEBOUNCE_TIME = 50;

constexpr uint8_t NUM_BUTTONS = 3;
const uint8_t BUTTON_PINS[NUM_BUTTONS] = {PEDESTRIAN_BUTTON_PIN, MAINTENANCE_BUTTON_PIN, EMERGENCY_BUTTON_PIN};
const char *BUTTON_NAMES[NUM_BUTTONS] = {"Pedestrian", "Maintenance", "Emergency"};

bool button_state[NUM_BUTTONS] = {HIGH, HIGH, HIGH};
bool last_reading[NUM_BUTTONS] = {HIGH, HIGH, HIGH};
unsigned long last_debounce_time[NUM_BUTTONS] = {0, 0, 0};

void poll_buttons() {
    for (uint8_t i = 0; i < NUM_BUTTONS; ++i) {
        bool reading = digitalRead(BUTTON_PINS[i]);

        if (reading != last_reading[i]) {
            last_debounce_time[i] = millis();
        }

        if (millis() - last_debounce_time[i] > DEBOUNCE_TIME) {
            if (reading != button_state[i]) {
                button_state[i] = reading;
                if (button_state[i] == LOW) {
                    Serial.print(BUTTON_NAMES[i]);
                    Serial.println(" pressed");
                } else {
                    Serial.print(BUTTON_NAMES[i]);
                    Serial.println(" released");
                }
            }
        }

        last_reading[i] = reading;
    }
}

void setup() {
    Serial.begin(115200);

    for (uint8_t i = 0; i < NUM_BUTTONS; ++i) {
        pinMode(BUTTON_PINS[i], INPUT_PULLUP);
        button_state[i] = HIGH;
        last_reading[i] = HIGH;
        last_debounce_time[i] = 0;
    }

    pinMode(YELLOW_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);

    Serial.println("Traffic Controller Booting...");
}

void loop() {
    poll_buttons();

    unsigned long current_millis = millis();

    if (current_millis - previous_millis >= INTERVAL) {
        previous_millis = current_millis;
        switch (current_state) {
        case TrafficState::Red:
            digitalWrite(RED_LED_PIN, HIGH);
            digitalWrite(YELLOW_LED_PIN, LOW);
            digitalWrite(GREEN_LED_PIN, LOW);

            Serial.println("[LED] RED");

            current_state = TrafficState::Green;
            break;

        case TrafficState::Green:
            digitalWrite(RED_LED_PIN, LOW);
            digitalWrite(YELLOW_LED_PIN, LOW);
            digitalWrite(GREEN_LED_PIN, HIGH);

            Serial.println("[LED] GREEN");

            current_state = TrafficState::Yellow;
            break;

        case TrafficState::Yellow:
            digitalWrite(RED_LED_PIN, LOW);
            digitalWrite(YELLOW_LED_PIN, HIGH);
            digitalWrite(GREEN_LED_PIN, LOW);

            Serial.println("[LED] YELLOW");

            current_state = TrafficState::Red;
            break;
        }
    }
}

#include <Arduino.h>

//                                         D
constexpr uint8_t PEDESTRIAN_BUTTON_PIN  = 5;
constexpr uint8_t MAINTENANCE_BUTTON_PIN = 6;
constexpr uint8_t EMERGENCY_BUTTON_PIN   = 7;

//                                 D
constexpr uint8_t YELLOW_LED_PIN = 2;
constexpr uint8_t RED_LED_PIN    = 3;
constexpr uint8_t GREEN_LED_PIN  = 4;

enum class ButtonMode {
    Pedestrian,
    Maintenance,
    Emergency
};

ButtonMode current_mode = ButtonMode::Pedestrian;

constexpr unsigned long DEBOUNCE_TIME = 50;
constexpr unsigned long MAINTENANCE_INTERVAL = 500;
constexpr unsigned long EMERGENCY_INTERVAL = 250;

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
                    if (i == 0) {
                        current_mode = ButtonMode::Pedestrian;
                        Serial.println("[MODE] Pedestrian");
                    } else if (i == 1) {
                        current_mode = ButtonMode::Maintenance;
                        Serial.println("[MODE] Maintenance");
                    } else if (i == 2) {
                        current_mode = ButtonMode::Emergency;
                        Serial.println("[MODE] Emergency");
                    }
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

    if (current_mode == ButtonMode::Pedestrian) {
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(YELLOW_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, HIGH);
        return;
    } else if (current_mode == ButtonMode::Maintenance) {
        unsigned long phase = millis() / MAINTENANCE_INTERVAL;
        if (phase % 2 == 0) {
            digitalWrite(RED_LED_PIN, HIGH);
            digitalWrite(YELLOW_LED_PIN, HIGH);
            digitalWrite(GREEN_LED_PIN, HIGH);
        } else {
            digitalWrite(RED_LED_PIN, LOW);
            digitalWrite(YELLOW_LED_PIN, LOW);
            digitalWrite(GREEN_LED_PIN, LOW);
        }
    } else if (current_mode == ButtonMode::Emergency) {
        unsigned long phase = millis() / EMERGENCY_INTERVAL;

        if (phase % 2 == 0) {
            digitalWrite(RED_LED_PIN, HIGH);
        } else {
            digitalWrite(RED_LED_PIN, LOW);
        }
        digitalWrite(YELLOW_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, LOW);
    }
}

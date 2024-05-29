#include <NewPing.h>

#define TRIGGER_PIN 8
#define ECHO_PIN 13
#define MAX_DISTANCE 200 // Maximum distance to detect obstacles (in cm)
#define MIN_DISTANCE_BLINK 30 // Minimum distance to blink lights (in cm)
#define MAX_DISTANCE_BLINK 60 // Maximum distance to blink lights (in cm)
#define RELAY_PIN 4 // Pin connected to the relay module
#define LED_PIN_1 6 // Pin connected to the first LED
#define LED_PIN_2 7 // Pin connected to the second LED
#define LED_PIN_3 3 // Pin connected to the third LED

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing object

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
}

void loop() {
  delay(50);
  unsigned int distance = sonar.ping_cm(); // Get distance in cm

  if (distance > 0 && distance <= MIN_DISTANCE_BLINK) {
    // Turn on lights
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_PIN_1, HIGH);
    digitalWrite(LED_PIN_2, HIGH);
    digitalWrite(LED_PIN_3, HIGH);
  } else if (distance > MIN_DISTANCE_BLINK && distance < MAX_DISTANCE_BLINK) {
    // Blink lights
    for (int i = 0; i < 3; i++) {
      digitalWrite(RELAY_PIN, HIGH);
      digitalWrite(LED_PIN_1, HIGH);
      digitalWrite(LED_PIN_2, HIGH);
      digitalWrite(LED_PIN_3, HIGH);
      delay(3000);
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(LED_PIN_1, LOW);
      digitalWrite(LED_PIN_2, LOW);
      digitalWrite(LED_PIN_3, LOW);
      delay(3000);
    }
  } else {
    // Turn off lights
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(LED_PIN_1, LOW);
    digitalWrite(LED_PIN_2, LOW);
    digitalWrite(LED_PIN_3, LOW);
  }
}

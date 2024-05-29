#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

#define TRIGGER_PIN 3
#define ECHO_PIN 2
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD I2C address and number of characters.

void setup() {
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight
  pinMode(8,OUTPUT);
  digitalWrite(8,HIGH);
}

void loop() {
  delay(100); // Wait 100ms between pings (about 10 pings/sec). 29ms should be the shortest delay between pings.
  
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).

  // Convert ping time to distance
  float distance_cm = uS / US_ROUNDTRIP_CM; // Convert ping time to distance in cm
  float distance_m = distance_cm / 100.0; // Convert distance to meters

  // Display distance on LCD
  lcd.clear(); // Clear the LCD screen
  lcd.setCursor(0, 0); // Set cursor to first line
  lcd.print("Dist(cm): "); // Display "Distance: "
  lcd.print(distance_cm, 2);

  lcd.setCursor(0, 1); // Set cursor to second line
  lcd.print("Dist(m):  ");
  lcd.print(distance_m, 2); // Display distance in meters with 2 decimal places


  delay(3000);
}

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define the address for the I2C LCD
#define LCD_ADDRESS 0x27  // Change this address to match your LCD

// Define the number of columns and rows for the LCD
#define LCD_COLUMNS 16
#define LCD_ROWS 2


LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS); // Initialize LCD

// Define the pins
const int sensorPin = A0; // LM35 sensor connected to analog pin A0
const int relayPin = 2;   // Relay connected to digital pin 2

// Define the temperature thresholds
const float lowerThreshold = 28.0; // Lower threshold temperature in Celsius
const float upperThreshold = 30.0; // Upper threshold temperature in Celsius

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Set relay pin as output
  pinMode(relayPin, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5,HIGH);
  
  // Initialize LCD
  lcd.init();

  // Turn on the backlight
  lcd.backlight();

  // Print initial message
  lcd.setCursor(0, 0);
  lcd.print("BRODER TEMP MONITOR");
}


void loop() {
  // Read the voltage from the LM35 sensor
  int sensorValue = analogRead(sensorPin);
  
  // Convert the sensor value to temperature in Celsius
  float voltage = sensorValue * (5.0 / 1023.0);
  float temperatureC = voltage * 100.0;
  
  // Print temperature to serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println("°C");
  //LCD SETUP
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperatureC);
  lcd.print("  C");

  
  
  // Check if temperature is below lower threshold
  if (temperatureC < lowerThreshold) {
    // Turn on the relay to activate the heating coil
    digitalWrite(relayPin, HIGH);
    Serial.println("Heating coil ON");
  }
  // Check if temperature is above upper threshold
  else if (temperatureC > upperThreshold) {
    // Turn off the relay to deactivate the heating coil
    digitalWrite(relayPin, LOW);
    Serial.println("Heating coil OFF");
  }
  
  // Add a delay to prevent rapid switching
  delay(1000); // Adjust delay as needed
}

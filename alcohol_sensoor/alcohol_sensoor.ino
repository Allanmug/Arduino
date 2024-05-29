#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define the address for the I2C LCD
#define LCD_ADDRESS 0x27  // Change this address to match your LCD

// Define the number of columns and rows for the LCD
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// Define the pin for the MQ-3 sensor
#define MQ3_PIN A0

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS); // Initialize LCD

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize LCD
  lcd.init();

  // Turn on the backlight
  lcd.backlight();

  // Print initial message
  lcd.setCursor(0, 0);
  lcd.print("Alcohol: -- ppm");
}

void loop() {
  // Read analog value from MQ-3 sensor with higher resolution
  int adc_value = analogRead(MQ3_PIN);

  // Convert analog value to ppm using a more sensitive calibration approach
  float ppm = map(adc_value, 0, 1023, 0, 2000); // Adjusted conversion for higher sensitivity

  // Print ppm on the LCD
  Serial.print("Alcohol: ");
  Serial.println(ppm);
  lcd.setCursor(0, 1);
  lcd.print("Alcohol: ");
  lcd.print(ppm);
  lcd.print(" ppm");

  // Wait for a while before taking the next reading
  delay(500); // Decreased delay for faster response, adjust as needed
}


//////////////////
//LCD GND to Arduino GND
//LCD VCC to Arduino 5V
//LCD SDA to Arduino A4
//LCD SCL to Arduino A5

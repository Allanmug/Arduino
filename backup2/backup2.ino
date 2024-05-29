#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <DHT.h>

#define DHTPIN 5     // Humidity pin
#define DHTTYPE DHT11  // DHT 11
#define MOISTURE_PIN A2
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2
#define PUMP 10

SoftwareSerial gsm(2, 3); // GSM module on pins 2 (RX) and 3 (TX)
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

String receivedMessage;
int h;  // Stores humidity value
int t; // Stores temperature value
int moistureLevel;
int lowerThreshold = 0;
int upperThreshold = 0;

unsigned long previousSMSTime = 0;
const unsigned long smsInterval = 5000; // Check for incoming SMS every 5 seconds

void setup() {
  Serial.begin(9600);
  gsm.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();
  pinMode(PUMP, OUTPUT);

  gsm.println("AT+CMGF=1"); // Set SMS mode to text mode
  gsm.println("AT+CMGD=1,4"); // Delete all SMS messages
  delay(2000);
  gsm.println("AT+CNMI=1,2,0,0,0"); // Configure GSM module to forward received SMS to Arduino
  SendMessagestart();
  delay(1000);
}

void loop() {
  unsigned long currentMillis = millis();

  // Check for incoming SMS every smsInterval milliseconds
  if (currentMillis - previousSMSTime >= smsInterval) {
    previousSMSTime = currentMillis;
    checkForIncomingSMS();
  }

  readSensors();
  // Add any additional functionality here that needs to be continuously performed
}

void readSensors() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  int analogReading = analogRead(MOISTURE_PIN);
  moistureLevel = map(analogReading, 0, 1023, 0, 100);

  lcd.setCursor(0, 0);
  lcd.print("Moisture: ");
  lcd.print(moistureLevel);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print("C  Humidity: ");
  lcd.print(h);
  lcd.print("%");
}

void checkForIncomingSMS() {
  if (gsm.available()) {
    receivedMessage = gsm.readString();
    Serial.println(receivedMessage);

    int index1 = receivedMessage.indexOf("\"", 0);
    int index2 = receivedMessage.indexOf("\"", index1 + 1);
    String phoneNumber = receivedMessage.substring(index1 + 1, index2);
    delay(2000);

    if (receivedMessage.indexOf("Pump on") != -1) {
      digitalWrite(PUMP, HIGH); // Turn on the pump
      delay(2000); // Delay for stability
      sendSMS(phoneNumber, "Pump turned on");
    } else if (receivedMessage.indexOf("Pump off") != -1) {
      digitalWrite(PUMP, LOW); // Turn off the pump
      delay(2000); // Delay for stability
      sendSMS(phoneNumber, "Pump turned off");
    }
    receivedMessage = "";
  }
}

void sendSMS(String phoneNumber, String message) {
  gsm.print("AT+CMGS=\""); // Start SMS transmission
  gsm.print(phoneNumber);
  gsm.println("\"");
  delay(1000);
  gsm.print(message);
  gsm.println((char)26); // End SMS transmission with CTRL+Z
  delay(1000);

  // Recommendation methods
  if (moistureLevel < 30) {
    gsm.println("AT+CMGS=\"" + phoneNumber + "\"\r");
    delay(1000);
    gsm.print("Low moisture level detected. Consider irrigating your crops.");
    gsm.println((char)26);
    delay(1000);
  } else if (moistureLevel >= 30 && moistureLevel <= 70) {
    gsm.println("AT+CMGS=\"" + phoneNumber + "\"\r");
    delay(1000);
    gsm.print("Optimal moisture level detected. Your crops are adequately watered.");
    gsm.println((char)26);
    delay(1000);
  } else {
    gsm.println("AT+CMGS=\"" + phoneNumber + "\"\r");
    delay(1000);
    gsm.print("High moisture level detected. Monitor your crops to prevent waterlogging.");
    gsm.println((char)26);
    delay(1000);
  }
}

void SendMessagestart() {
  gsm.println("AT+CMGS=\"+256771163653\"\r"); 
  delay(1000);
  gsm.print("YOU'RE WELCOME TO PROJECT WATER MANAGEMENT \n Please provide crop to be irrigated\n 1) Maize\n 2) vegetables \n 3) Rice ");
  delay(1000);
  gsm.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

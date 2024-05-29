#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

#define DHTPIN 3     // Humidity pin
#define DHTTYPE DHT11  // DHT 11
#define ONE_WIRE_BUS 2 
#define PUMP 8

SoftwareSerial gsm(10, 11); // GSM module on pins 10 (RX) and 11 (TX)
DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

String receivedMessage;
int h;  // Stores humidity value
int moistureLevel;
int lowerThreshold = 0;
int upperThreshold = 0;
const int reading_in_air = 1024;
const int reading_in_fluid = 305;
int n = 1;
unsigned long previousSMSTime = 0;
const unsigned long smsInterval = 5000; // Check for incoming SMS every 5 seconds
bool recommendationSent = false; // Flag to track if recommendation has been sent

void setup() {
  Serial.begin(9600);
  gsm.begin(9600);
  dht.begin();
  pinMode(PUMP, OUTPUT);
  pinMode(4, OUTPUT);

  gsm.println("AT+CMGF=1"); // Set SMS mode to text mode
  gsm.println("AT+CMGD=1,4"); // Delete all SMS messages
  delay(2000);
  gsm.println("AT+CNMI=1,2,0,0,0"); // Configure GSM module to forward received SMS to Arduino
  SendMessageStart();
  delay(1000);
}

void loop() {
  digitalWrite(4, HIGH);
  delay(1000);
  digitalWrite(4, LOW);
  delay(1000);
  digitalWrite(4, HIGH);
  unsigned long currentMillis = millis();

  // Check for incoming SMS every smsInterval milliseconds
  if (currentMillis - previousSMSTime >= smsInterval) {
    previousSMSTime = currentMillis;
    checkForIncomingSMS();
  }

  if (n == 1) {
    sendAutomaticRecommendation();
  }
  
  readSensors();
  n++;
  receivedMessage = "";
}

void readSensors() {
  h = dht.readHumidity();
  sensors.requestTemperatures(); 
  float t = sensors.getTempCByIndex(0);
  int analog_reading = analogRead(A1); // Read analog value from sensor
  float percentage = ((analog_reading - reading_in_air) / (float)(reading_in_fluid - reading_in_air)) * 100;
  moistureLevel = min(100, max(0, (int)percentage));
  Serial.print("Moisture: ");
  Serial.print(moistureLevel);
  Serial.println("%");
  Serial.print("Temperature: ");
  Serial.println(t);
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println("%");
}

void checkForIncomingSMS() {
  if (gsm.available()) {
    receivedMessage = gsm.readString();
    Serial.println(receivedMessage);

    int index1 = receivedMessage.indexOf("\"", 0);
    int index2 = receivedMessage.indexOf("\"", index1 + 1);
    String phoneNumber = receivedMessage.substring(index1 + 1, index2);
    delay(2000);

    if (receivedMessage.indexOf("pump on") != -1) {
      digitalWrite(PUMP, HIGH); // Turn on the pump
      delay(2000); // Delay for stability
      sendSMS(phoneNumber, "Pump turned on");
    } else if (receivedMessage.indexOf("pump off") != -1) {
      digitalWrite(PUMP, LOW); // Turn off the pump
      delay(2000); // Delay for stability
      sendSMS(phoneNumber, "Pump turned off");
    } else if (receivedMessage.indexOf("maize") != -1) {
      lowerThreshold = 45;
      upperThreshold = 60;
      sendSMS(phoneNumber, "You have chosen maize");
      delay(2000);  
    } else if (receivedMessage.indexOf("vegetables") != -1) {
      lowerThreshold = 50;
      upperThreshold = 70;
      sendSMS(phoneNumber, "You have chosen vegetables");
      delay(2000);  
    } else if (receivedMessage.indexOf("rice") != -1) {
      lowerThreshold = 80;
      upperThreshold = 100;
      sendSMS(phoneNumber, "You have chosen rice");
      delay(2000);   
    } else if (receivedMessage.indexOf("State") != -1) {
      sendAutomaticRecommendation();
      delay(2000);   
    }
   
    delay(1000);
    receivedMessage = "";
  }
}

void sendAutomaticRecommendation() {
  if (!recommendationSent) {
    if (moistureLevel < lowerThreshold) {
      sendSMS("+256756901316", "Low moisture level detected. Consider irrigating your crops.");
    } else if (moistureLevel > lowerThreshold && moistureLevel < upperThreshold) {
      sendSMS("+256756901316", "Optimal moisture level detected. Your crops are adequately watered.");
    } else if (moistureLevel > upperThreshold) {
      sendSMS("+256756901316", "High moisture level detected. Monitor your crops to prevent waterlogging.");
    }
    recommendationSent = true;
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
}

void SendMessageStart() {
  gsm.println("AT+CMGS=\"+256756901316\"\r"); 
  delay(1000);
  gsm.print("YOU'RE WELCOME TO PROJECT WATER MANAGEMENT \n what type of crop do you want to irrigate\n 1) Maize\n 2) Vegetables \n 3) Rice ");
  delay(1000);
  gsm.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

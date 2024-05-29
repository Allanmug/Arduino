#include <SoftwareSerial.h> 
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"

#define PUMP 8
#define DHTPIN 3     // humidity pin
#define DHTTYPE DHT11   // DHT 11
#define ONE_WIRE_BUS 2  // Data wire is plugged into digital pin 2 on the Arduino

// Initializing 
SoftwareSerial gsm(10, 11); // RX, TX
DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);  // Setup a oneWire instance to communicate with any OneWire device
DallasTemperature sensors(&oneWire); // Pass oneWire reference to DallasTemperature library

// Function prototypes
void sendMessage(String message);
void sendWelcomeMessage();
void clearGsmBuffer();
void processReceivedMessage(String message);

// Constants
const int reading_in_air = 1024;
const int reading_in_fluid = 305;
const String phoneNumber = "+256780109484";
const unsigned long messageInterval = 60000; // 1 minute
const unsigned long bufferClearInterval = 180000; // 3 minutes
int moistureLevel;
float humidityValue;  
float temperatureC ;


// Variables
unsigned long lastMessageTime = 0;
unsigned long lastBufferClearTime = 0;

void setup() {
  Serial.begin(9600); 
  gsm.begin(9600);
  dht.begin();
  
  pinMode(PUMP, OUTPUT);
  pinMode(4, OUTPUT);
  
  // Configure the GSM module to receive SMS messages
/*  gsm.println("AT+CMGF=1"); 
  gsm.println("AT+CMGD=1,4");
  delay(2000);
  gsm.println("AT+CNMI=1,2,0,0,0"); 
  delay(1000);
  */
  sendWelcomeMessage();
}

void loop() {
  digitalWrite(4, HIGH);
  delay(1000);
  digitalWrite(4, LOW);
  delay(1000);
  digitalWrite(4, HIGH);
  
  // Read sensor data
  int analog_reading = analogRead(A1); // Read analog value from sensor
  float percentage = ((analog_reading - reading_in_air) / (float)(reading_in_fluid - reading_in_air)) * 100;
  int moistureLevel = min(100, max(0, (int)percentage));
  float humidityValue = dht.readHumidity(); 
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0); 
  Serial.print("moisture :");
  Serial.print(moistureLevel);
  Serial.print("//");
  Serial.print("Temprature :");
  Serial.print(temperatureC);
  Serial.print("//");
  Serial.print("humidity :");
  Serial.println(humidityValue);
 /*  irrigationLogic(moistureLevel,humidityValue,temperatureC);
  
  
  // Send sensor data every messageInterval milliseconds
/*  if (millis() - lastMessageTime >= messageInterval) {
    sendMessage(String(moistureLevel) + " " + String(humidityValue) + " " + String(temperatureC));
    lastMessageTime = millis();
  }
*/
  // Check for incoming SMS
/*  if (gsm.available()) {
    String receivedMessage = gsm.readString();
    Serial.println(receivedMessage);
    processReceivedMessage(receivedMessage);
  }
*/
  // Clear GSM buffer every bufferClearInterval milliseconds
 /* if (millis() - lastBufferClearTime >= bufferClearInterval) {
    clearGsmBuffer();
    lastBufferClearTime = millis();
  }
  */
  delay(1000); // Ensure some delay to prevent overwhelming the GSM module
}

void sendMessage(String message) {
  gsm.println("AT+CMGS=\"" + phoneNumber + "\"\r"); 
  delay(1000);
  gsm.print(message);
  delay(1000); 
  gsm.println((char)26); // ASCII code of CTRL+Z
  delay(1000);
}

void sendWelcomeMessage() {
  sendMessage("WELCOME TO PROJECT AUTOMATED SMS CONTROLLED IRRIGATION\n Temperature(T) in degrees Celcius \n Moisture(M) in percentage \n Humidity(H) in percentage\n  MOISTURE HUMIDITY TEMPERATURE");
}

void clearGsmBuffer() {
  while (gsm.available()) {
    gsm.read(); // Read and discard one character from the buffer
    delay(10);  // Optional delay to avoid busy-waiting
  }
}

void processReceivedMessage(String message) {
  int index1 = message.indexOf("\"");
  int index2 = message.indexOf("\"", index1 + 1); 
  String senderPhoneNumber = message.substring(index1 + 1, index2);
  
  if (senderPhoneNumber == phoneNumber) {
    if (message.indexOf("PUMP ON") != -1 || message.indexOf("YES IRRIGATE") != -1) {
      digitalWrite(PUMP, HIGH); 
      sendMessage("PUMP IS ON");
    } else if (message.indexOf("PUMP OFF") != -1 || message.indexOf("DONT IRRIGATE") != -1) {
      digitalWrite(PUMP, LOW);
      sendMessage("PUMP IS OFF");    
    } else if (message.indexOf("STATUS") != -1) {
       sendMessage(String(moistureLevel) + " " + String(humidityValue) + " " + String(temperatureC));      
    }
  }
}


void irrigationLogic(float soilMoisture, float humidity, float temperature) {
    // Define threshold for the weighted average
    float weightedAverageThreshold = 35.0; // Adjust as needed

    // Define weights for each parameter (moisture, humidity, temperature)
    float moistureWeight = 0.6; // Higher weight for moisture
    float humidityWeight = 0.3;
    float temperatureWeight = 0.1;

    // Calculate weighted average
    float weightedAverage = (moistureWeight * soilMoisture) + (humidityWeight * humidity) + (temperatureWeight * temperature);

    // Check if weighted average is below threshold
    if (weightedAverage < weightedAverageThreshold) {
      sendMessage(String(soilMoisture) + " " + String(humidity) + " " + String(temperature) + "\n" + "Irrigation Required");  
    } 
}

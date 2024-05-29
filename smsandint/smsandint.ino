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
#define PUMP 8
//#define redLED 5
//#define greenLED 6 
#define buzzer 7

SoftwareSerial gsm(2, 3); // GSM module on pins 2 (RX) and 3 (TX)
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

String receivedMessage;
String phoneNumber;
int h;  // Stores humidity value
int t; // Stores temperature value
int moistureLevel;
int lowerThreshold = 0;
int upperThreshold = 0;

void setup() {
  Serial.begin(9600);
  gsm.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();
  pinMode(PUMP, OUTPUT);
  //pinMode(redLED, OUTPUT);
 // pinMode(greenLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  gsm.println("AT+CMGF=1"); // Set SMS mode to text mode
  gsm.println("AT+CMGD=1,4"); // Delete all SMS messages
  delay(2000);
  gsm.println("AT+CNMI=1,2,0,0,0"); // Configure GSM module to forward received SMS to Arduino
  SendMessagestart();
  delay(2000);
}

void loop() {
  readSensors();
  checkForIncomingSMS();
  sendDataToThingSpeak();
  delay(1000); // Delay 1 sec.
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
    receivedMessage = gsm.readString();
    Serial.println(receivedMessage);

    int index1 = receivedMessage.indexOf("\"", 0);
    int index2 = receivedMessage.indexOf("\"", index1 + 1);
    phoneNumber = receivedMessage.substring(index1 + 1, index2);
    delay(2000);

    if (receivedMessage.indexOf("1") != -1 || receivedMessage.indexOf("Maize") != -1) {
      lowerThreshold = 45;
      upperThreshold = 60;
      sendSMS("You have chosen maize");
      delay(2000);
    } else if (receivedMessage.indexOf("2") != -1 || receivedMessage.indexOf("Vegetables") != -1) {
      lowerThreshold = 50;
      upperThreshold = 70;
      sendSMS("You have chosen vegetables");
      delay(2000);
    } else if (receivedMessage.indexOf("3") != -1 || receivedMessage.indexOf("Rice") != -1) {
      lowerThreshold = 80;
      upperThreshold = 100;
      sendSMS("You have chosen rice");
      delay(2000);
    }
    receivedMessage = "";
  }
}

void sendDataToThingSpeak() {
  float M = moistureLevel;
  gsm.println("AT");
  delay(1000);
  gsm.println("AT+CPIN?");
  delay(1000);
  gsm.println("AT+CREG?");
  delay(1000);
  gsm.println("AT+CGATT?");
  delay(1000);
  gsm.println("AT+CIPSHUT");
  delay(1000);
  gsm.println("AT+CIPSTATUS");
  delay(2000);
  gsm.println("AT+CIPMUX=0");
  delay(2000);
  gsm.println("AT+CSTT=\"internet.mtn\"");
  delay(1000);
  gsm.println("AT+CIICR");// Bring up wireless connection
  delay(3000);
  gsm.println("AT+CIFSR");// Get local IP address
  delay(2000);
  gsm.println("AT+CIPSPRT=0");
  delay(3000);
  gsm.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");// Start up the connection
  delay(6000);
  gsm.println("AT+CIPSEND");// Begin send data to remote server
  delay(4000);

  // Prepare data to send to ThingSpeak
  String str = "GET https://api.thingspeak.com/update?api_key=8LYZ7S3V1UOLRVCX&field1=" + String(t) + "&field2=" + String(h) + "&field3=" + String(M);
  Serial.println(str);
  gsm.println(str);// Begin send data to remote server
  
  delay(4000);
  gsm.println((char)26);// Sending
  delay(5000);// Waiting for reply, the time depends on the internet condition
  gsm.println();
  gsm.println("AT+CIPSHUT");// Close the connection
  delay(100);
}

void sendSMS(String message) {
  gsm.print("AT+CMGS=\"");
  gsm.print(phoneNumber);
  gsm.println("\"");
  delay(1000);
  gsm.print(message);
  gsm.println((char)26); // ASCII code of CTRL+Z
  delay(1000);
}

void SendMessagestart() {
  gsm.println("AT+CMGS=\"+256771163653\"\r");
  delay(1000);
  gsm.print("YOU'RE WELCOME TO PROJECT WATER MANAGEMENT \n what type of crop do you want to irrigate\n 1) Maize\n 2) Vegetables \n 3) Rice ");
  delay(1000);
  gsm.println((char)26); // ASCII code of CTRL+Z
  delay(1000);
}

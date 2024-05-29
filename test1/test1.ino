#include <SoftwareSerial.h>

// Define GSM module connections
#define GSM_RX 10
#define GSM_TX 11

// Set up SoftwareSerial connection to GSM module
SoftwareSerial gsmModule(GSM_RX, GSM_TX);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  gsmModule.begin(9600);

  // Wait for the GSM module to start
  delay(1000);

  // Send AT commands to configure GSM module
  sendATCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  sendATCommand("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"");
  sendATCommand("AT+SAPBR=1,1");
  sendATCommand("AT+SAPBR=2,1");
  sendATCommand("AT+HTTPINIT");
  sendATCommand("AT+HTTPPARA=\"CID\",1");
  sendATCommand("AT+HTTPPARA=\"URL\",\"api.thingspeak.com/update\"");
}

void loop() {
  // Upload data to ThingSpeak
  sendDataToThingSpeak("CHANNELAPIKEY", 700);

  // Wait for some time before sending the next data
  delay(60000); // Example: Send data every minute
}

// Function to send AT command and read response
void sendATCommand(String command) {
  gsmModule.println(command);
  delay(1000); // Wait for response
  while (gsmModule.available()) {
    Serial.write(gsmModule.read());
  }
}

// Function to send data to ThingSpeak
void sendDataToThingSpeak(String apiKey, int data) {
  String command = "AT+HTTPDATA=35,10000";
  sendATCommand(command);
  command = "api_key=" + apiKey + "&field1=" + String(data);
  sendATCommand(command);
  sendATCommand("AT+HTTPACTION=1");
}

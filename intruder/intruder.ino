#include <NewPing.h>
#include <SoftwareSerial.h> 
#define TRIGGER_PIN 8 // Ultrasonic sensor trigger pin
#define ECHO_PIN 13    // Ultrasonic sensor echo pin

// initialize the GSM module with the correct pins
NewPing sonar(TRIGGER_PIN, ECHO_PIN);
SoftwareSerial gsm(10, 11); // RX, TX

#define FRONT 5
#define BACK 6
#define CENTER 7


void sendMassage(String to_send);
void SendMessagestart();
int n = 1;
int num;
bool intruderDetected = false;

// initialize a variable to store the received message
String receivedMessage;
String phoneNumber;

void setup() {
  // set up the serial communication with the computer
  
  pinMode(FRONT, OUTPUT);
  pinMode(BACK, OUTPUT);
  pinMode(CENTER, OUTPUT);
  

  Serial.begin(9600);
  gsm.begin(9600);

  // configure the GSM module to receive SMS messages
  gsm.println("AT+CMGF=1");
  gsm.println("AT+CMGD=1,4");
  delay(2000);
  gsm.println("AT+CNMI=1,2,0,0,0");
  delay(1000);
}

void loop() {
  if (gsm.available()) {
    if (n == 1) {
      SendMessagestart();
    }
    receivedMessage = gsm.readString();
    Serial.println(receivedMessage);
    String message = receivedMessage;
    int index1 = message.indexOf("\"", 0);
    int index2 = message.indexOf("\"", index1 + 1);
    phoneNumber = message.substring(index1 + 1, index2);
  }

  // Check for intruder using ultrasonic sensor
  unsigned int distance = sonar.ping_cm();
  Serial.println(distance);

  if (distance < 100 && !intruderDetected) { // Adjust this threshold according to your setup
    intruderDetected = true;
    digitalWrite(FRONT, HIGH);
    digitalWrite(BACK, HIGH);
    digitalWrite(CENTER, HIGH);
    delay(3000); // Blink for 500 milliseconds
    digitalWrite(FRONT, LOW);
    digitalWrite(BACK, LOW);
    digitalWrite(CENTER, LOW);
  }

  if (distance < 50 && distance > 0 ) { // Confirmation: intruder moves closer
    digitalWrite(FRONT, HIGH);
    digitalWrite(BACK, HIGH);
    digitalWrite(CENTER, HIGH);
    sendMassage("Intruder detected! Lights turned on.");
  } 
  if (intruderDetected && distance >= 50) { // Intruder moves away
    intruderDetected = false;
    digitalWrite(FRONT, LOW);
    digitalWrite(BACK, LOW);
    digitalWrite(CENTER, LOW);
  }

  n += 1;
  receivedMessage = "";
  delay (3000);
}

void sendMassage(String to_send) {
   gsm.println("AT+CMGS=\"+256756901316\"\r");
  delay(1000);
  gsm.print(to_send);
  gsm.println((char)26); // ASCII code of CTRL+Z
  delay(1000);
}

void SendMessagestart() {
  gsm.println("AT+CMGS=\"+256756901316\"\r");
  delay(1000);
  gsm.print("YOU'RE WELCOME TO PROJECT INTRUDER ALART");
  delay(1000);
  gsm.println((char)26); // ASCII code of CTRL+Z
  delay(1000);
}

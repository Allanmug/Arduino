#include <NewPing.h>
#include <SoftwareSerial.h> 
#define TRIGGER_PIN 8 // Ultrasonic sensor trigger pin
#define ECHO_PIN 13    // Ultrasonic sensor echo pin

// initialize the GSM module with the correct pins
NewPing sonar(TRIGGER_PIN, ECHO_PIN);


#define  APPLIANCE 3
#define  FRONT 4
#define  BACK 6
#define  CENTER 7


// initialize a variable to store the received message


void setup() {
  // set up the serial communication with the computer
  pinMode(APPLIANCE, OUTPUT);
  pinMode(FRONT, OUTPUT);
  pinMode(BACK, OUTPUT);
  pinMode(CENTER, OUTPUT);

  Serial.begin(9600);

  // configure the GSM module to receive SMS messages
 ;
}

void loop() {
  // Check for intruder using ultrasonic sensor
  unsigned int distance = sonar.ping_cm();
  Serial.println(distance);

  if (distance > 30 && distance < 60) { // Adjust this threshold according to your setup
    
    digitalWrite(APPLIANCE, HIGH); 
      digitalWrite(FRONT, HIGH); 
      digitalWrite(BACK, HIGH); 
      digitalWrite(CENTER, HIGH);
    delay(3000); // Blink for 500 milliseconds
    digitalWrite(FRONT, LOW);
    digitalWrite(BACK, LOW);
    digitalWrite(CENTER, LOW);
  }

  if (distance > 0 && distance < 30  ) { // Confirmation: intruder moves closer
    digitalWrite(APPLIANCE, HIGH); 
    digitalWrite(FRONT, HIGH); 
    digitalWrite(BACK, HIGH); 
    digitalWrite(CENTER, HIGH);
  } else if (distance >= 50) { // Intruder moves away
   
    digitalWrite(FRONT, LOW);
    digitalWrite(BACK, LOW);
    digitalWrite(CENTER, LOW);
  }else {
    digitalWrite(FRONT, LOW);
    digitalWrite(BACK, LOW);
    digitalWrite(CENTER, LOW);
    }

  delay (3000);
}

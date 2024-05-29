#include <SoftwareSerial.h> 

// initialize the GSM module with the correct pins
SoftwareSerial gsm(10, 11); // RX, TX
#define  APPLIANCE 2
#define  FRONT 4
#define  BACK 6
#define  CENTER 7

void sendmassage(String to_send);
void SendMessagestart();
int n=1;
int num;
// initialize a variable to store the received message
String receivedMessage;
String phoneNumber;

void setup() {
  // set up the serial communication with the computer
  pinMode(APPLIANCE, OUTPUT);
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
      if (n==1){
      SendMessagestart();
      }
      receivedMessage = gsm.readString();
      Serial.println(receivedMessage);
      String message = receivedMessage;
      int index1 = message.indexOf("\"", 0);
      int index2 = message.indexOf("\"", index1 + 1);
      phoneNumber = message.substring(index1 + 1, index2);
      
      }
      
     if (phoneNumber.indexOf("+256756901316") != -1) {
     
     if (receivedMessage.indexOf("ALL ON") != -1 || receivedMessage.indexOf("BYONA ON") != -1 ) {
      digitalWrite(APPLIANCE, HIGH); 
      digitalWrite(FRONT, HIGH); 
      digitalWrite(BACK, HIGH); 
      digitalWrite(CENTER, HIGH);
     sendmassage("All switches are on");
       
    }
    else if (receivedMessage.indexOf("ALL OFF") != -1 || receivedMessage.indexOf("BYONA OFF") != -1 ) {
      digitalWrite(APPLIANCE, LOW);
      digitalWrite(FRONT, LOW);
      digitalWrite(BACK, LOW);
      digitalWrite(CENTER, LOW);
      sendmassage("All lights are off");
       
    }
    
    if (receivedMessage.indexOf("FRONT ON") != -1  || receivedMessage.indexOf("MUMASO YAKA") != -1 ) {
      digitalWrite(FRONT, HIGH); 
      sendmassage("Front Lights are on");
     
    }
    else if (receivedMessage.indexOf("FRONT OFF") != -1  || receivedMessage.indexOf("MUMASO VAKO") != -1) {
      digitalWrite(FRONT, LOW);
      sendmassage("Front Lights are off");
    
    }
    if (receivedMessage.indexOf("BACK ON") != -1  || receivedMessage.indexOf("MABEGA YAKA") != -1) {
      digitalWrite(BACK, HIGH); 
      sendmassage("Back lights are on");
       
    }
    else if (receivedMessage.indexOf("BACK OFF") != -1  || receivedMessage.indexOf("MABEGA VAKO") != -1) {
      digitalWrite(BACK, LOW);
      sendmassage("Back lights are off");
    }
     if (receivedMessage.indexOf("CENTER ON") != -1  || receivedMessage.indexOf("MAKATI YAKA") != -1) {
      digitalWrite(CENTER, HIGH); 
      sendmassage("Center light is on");
       
    }
    else if (receivedMessage.indexOf("CENTER OFF") != -1 || receivedMessage.indexOf("MAKATI VAKO") != -1) {
      digitalWrite(CENTER, LOW);
      sendmassage("Center light is off");
       
    }

    if (receivedMessage.indexOf("APPLIANCE ON") != -1 || receivedMessage.indexOf("TEKAKO FRIGI") != -1) {
      digitalWrite(APPLIANCE, HIGH); 
      sendmassage("Appliance is on");
       
    }
    else if (receivedMessage.indexOf("APPLIANCE OFF") != -1 || receivedMessage.indexOf("JAKO FRIGI") != -1) {
      digitalWrite(APPLIANCE, LOW);
      sendmassage("Appliance is off");
       
    }
    
     }
     n+=1;
    
      //if(receivedMessage = ""){
      //digitalWrite(FRONT, HIGH);
      //delay(5000); 
      //digitalWrite(BACK, HIGH); 
      //delay(5000);
      //digitalWrite(CENTER, HIGH);
      //delay(5000);
      //digitalWrite(FRONT, LOW);
      //delay(5000);
      //digitalWrite(BACK, LOW);
      //delay(5000);
      //digitalWrite(CENTER, LOW);
      //delay(5000);
      //}
      receivedMessage = "";
   
}
void sendmassage(String to_send){
      gsm.print("AT+CMGS=\"");
      gsm.print(phoneNumber);
      gsm.println("\"");
      delay(1000);
      gsm.print(to_send);
      gsm.println((char)26);// ASCII code of CTRL+Z
      delay(1000);
       
  }

   void SendMessagestart()
{
  gsm.println("AT+CMGS=\"+256756901316\"\r"); 
  delay(1000);
  gsm.print("YOU'RE WELCOME TO PROJECT AUTOMATION OF HOME APPLIANCES\nWord used not recognized\n+try\n1.FRONT ON\n2.BACK ON\n3.CENTER ON\n4.APPLIANCE ON\nALL ON\n relplace ON with OFF to turn off");
  delay(1000);
  gsm.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

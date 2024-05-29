#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h> 
#include <DHT.h>


SoftwareSerial gsm(2, 3);

#define DHTPIN 5   // humidity pin
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

#define moisturepin A2
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
#define PUMP 8
#define redLED 5
#define greenLED 6 
#define buzzer 7


void SendMessagestart();
void sendmassage();
String receivedMessage;
String phoneNumber;
int h;  //Stores humidity value
int t; //Stores temperature value
int moistureLevel;
float humidityValue;  
float temperatureC ;
float voltage_reading;
int lowerthreshold ;
int upperthreshold ;


int n=1;
void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
   gsm.begin(9600);
   dht.begin();
   lcd.init();; //initialize the lcd
   lcd.backlight(); //open the backlight
   pinMode(PUMP, OUTPUT);
   pinMode(redLED,OUTPUT);
   pinMode(greenLED,OUTPUT);
   pinMode(buzzer,OUTPUT);
  
  
  // configure the GSM module to receive SMS messages
  gsm.println("AT+CMGF=1"); 
  gsm.println("AT+CMGD=1,4");
  delay(2000);
  gsm.println("AT+CNMI=1,2,0,0,0"); 

  SendMessagestart();
   delay(2000);
}

void loop() {
  h = dht.readHumidity();
  Serial.println(h);
  t = dht.readTemperature();
  Serial.println(t);
  int analog_reading = analogRead(A2); // Read analog value from sensor
  int moisturePercentage = map(analog_reading, 0, 1023, 0, 100);


  //lcd screen setup
    lcd.setCursor(0, 0);
    lcd.print (" Moisture:");
    lcd.print(moisturePercentage);
    lcd.print("%");
    
    lcd.setCursor(0, 1);
    lcd.print("Temp:");
    lcd.print(t);
    lcd.print("C");
    lcd.print(" Hum:");
    lcd.print(h);
    lcd.print("%");
    
  delay(1000); //Delay 1 sec.

   //check for incoming massage
   
      String receivedMessage = gsm.readString();
      Serial.println(receivedMessage);
      receivedMessage = gsm.readString();
      Serial.println(receivedMessage);
      String message = receivedMessage;
      int index1 = message.indexOf("\"", 0);
      int index2 = message.indexOf("\"", index1 + 1);
      phoneNumber = message.substring(index1 + 1, index2);
      delay(2000);
     
     if (receivedMessage.indexOf("1") != -1 || receivedMessage.indexOf("Maize") != -1 ) {
         lowerthreshold = 45;
         upperthreshold = 60;
     sendmassage("You have choosen maize"); 
     delay(2000);  
    }else if (receivedMessage.indexOf("2") != -1 || receivedMessage.indexOf("Vegetables") != -1 ) {
         lowerthreshold = 50 ;
         upperthreshold = 70;
     sendmassage("You have choosen vegatables");  
     delay(2000);  
    }else if (receivedMessage.indexOf("3") != -1 || receivedMessage.indexOf("Rice") != -1 ) {
        lowerthreshold =80;
        upperthreshold= 100;
     sendmassage("You have choosen rice");  
     delay(2000);   
    }
if(moisturePercentage > 1){
    if(moisturePercentage < lowerthreshold){
      digitalWrite(PUMP, HIGH);
      }else if(moisturePercentage > upperthreshold){
        digitalWrite(PUMP,LOW);
        }}
 receivedMessage = "";
delay(3000);
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
  gsm.println("AT+CMGS=\"+256771163653\"\r"); 
  delay(1000);
  gsm.print("YOU'RE WELCOME TO PROJECT WATER MANAGEMENT \n what type of crop do you what to irrigate\n 1) Maize\n 2) vegetables \n 3) Rice ");
  delay(1000);
  gsm.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

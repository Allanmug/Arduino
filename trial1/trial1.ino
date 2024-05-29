#define moisturepin A2
void setup() {
  // put your setup code here, to run once:
pinMode(8,OUTPUT);
}

void loop() {
  int analog_reading = analogRead(A2); // Read analog value from sensor
  int moisturePercentage = map(analog_reading, 0, 1023, 0, 100);
 if (moisturePercentage < 30) {
   digitalWrite(8,HIGH);
  }else if (moisturePercentage > 60){
    digitalWrite(8,LOW);
    }

    delay(3000);
 
  
  // put your main code here, to run repeatedly:

}

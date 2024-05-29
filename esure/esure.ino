#define  APPLIANCE 2
#define  FRONT 4
#define  BACK 6
#define  CENTER 7

void setup() {
  // put your setup code here, to run once:
  pinMode(APPLIANCE, OUTPUT);
  pinMode(FRONT, OUTPUT);
  pinMode(BACK, OUTPUT);
  pinMode(CENTER, OUTPUT);
}

void loop() {
  digitalWrite(APPLIANCE,HIGH);
  digitalWrite(FRONT,HIGH);
  digitalWrite(BACK,HIGH);
  digitalWrite(CENTER,HIGH);
  // put your main code here, to run repeatedly:

}

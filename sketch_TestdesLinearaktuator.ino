int relais1pin = 25;
int relais2pin = 33;

void setup() {
  pinMode(relais1pin,OUTPUT);
  pinMode(relais2pin,OUTPUT);
}

void loop() {
  // keine Bewegung
  digitalWrite(relais1pin,HIGH);
  digitalWrite(relais2pin,HIGH);
  delay(1000);

  // raus 
  digitalWrite(relais1pin,HIGH);
  digitalWrite(relais2pin,LOW);
  delay(2000);

  // keine Bewegung
  digitalWrite(relais1pin,LOW);
  digitalWrite(relais2pin,LOW);
  delay(1000);

  // rein
  digitalWrite(relais1pin,LOW);
  digitalWrite(relais2pin,HIGH);
  delay(1000);

}

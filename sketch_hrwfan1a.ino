
#include "Arduino.h"
#include <SPI.h>
#include <TFT_eSPI.h> 
#include "EspMQTTClient.h"
#include "ArxStringUtils.h"
#include "DHT.h"
#include <Adafruit_Sensor.h>
//Dht sensor implementierung
#define DHTPIN 25  
#define DHTTYPE DHT22


DHT dht(DHTPIN, DHTTYPE);


//Oled screen implementierung
TFT_eSPI tft = TFT_eSPI();


//Definition der pins und variabeln
float temp;
int innen_temp=0;
int count=0;
const int fan_control_pin=21;
unsigned long start_time;
int gasd = 13;
int gasa= 36;
int fan;
int xpos=0; 
int ypos=0; 
int echo=17;
int trigger=27;
long dauer=0;
long entfernung=0;
int red=15;
int green=12;
//
//variabeln für übergabe des Mqtt servers

String interior_co2, interior_temp;
String exterior_co2, exterior_temp;
String fan_speed, distance;

//MQTT inistialisierung

EspMQTTClient client(
  "Jonny-netgear",
  "123456789",
  "hrw-fablab.de", // <----------- DAS HAT SICH VERÄNDERT
  "gruppe4",
  "Kj+bu79P2LM8gX6b",
  "esws20g4fx-esp32"
);

void setup() {
  // Mqtt 
   client.enableDebuggingMessages();
  //dht
  dht.begin();
   
   //screen
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);

  //ein und ausgang
  pinMode(gasd, INPUT);
  pinMode(gasa, INPUT);
  pinMode(fan_control_pin,OUTPUT);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  //leds auf low
  digitalWrite(red,LOW);
  digitalWrite(green,LOW);

}
void updateDisplay() {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 10);
  }
  
void onConnectionEstablished()
{
  client.subscribe("ES/WS20/gruppe4/interior/temperature", [](const String & payload) { // <-- Temp außen
    innen_temp = payload.toInt();
    
    updateDisplay();
  });
  
}

void raus()
{
  if(entfernung<=12)
{
  digitalWrite(relais1pin,HIGH);
  digitalWrite(relais2pin,LOW);
  delay(1000);
  
}
else{ 
  digitalWrite(relais1pin,LOW);
  digitalWrite(relais2pin,LOW);
  delay(1000);
  }

}

void rein()
{
    if(entfernung<=12)
{
  digitalWrite(relais1pin,LOW);
  digitalWrite(relais2pin,HIGH);
  delay(1000);
  
}
else{ 
  digitalWrite(relais1pin,LOW);
  digitalWrite(relais2pin,LOW);
  delay(1000);
  }
}


void loop() {
  
  client.loop();
  
  //Dht Sensor
  float t = dht.readTemperature();
  float h = dht.readHumidity();
 
  tft.setTextColor(TFT_WHITE,TFT_BLACK); 
  tft.setCursor(xpos,ypos); 
  int analogSensor = analogRead(gasa);
  int digitalSensor = digitalRead(gasd);
  temp=float(analogRead(39)-420)/10;

  //abstand
//---------------------------------------------------------------------------
digitalWrite(trigger, LOW);

delay(5);

digitalWrite(trigger, HIGH);

delay(10);

digitalWrite(trigger, LOW);

dauer = pulseIn(echo, HIGH);

entfernung = (dauer/2) * 0.03432;
//fanControl
//-----------------------------------------------------------------------------


if(t<30) 
{
  digitalWrite(fan_control_pin,NULL);
  fan=0;
  digitalWrite(green,HIGH);
  digitalWrite(red,LOW);
  rein();
}
if(innen_temp<30)
{
  digitalWrite(fan_control_pin,NULL);
  fan=0;
  digitalWrite(green,HIGH);
  digitalWrite(red,LOW);
  rein();
}
if(t>=30)
{
  if(entfernung>=5){
  digitalWrite(fan_control_pin,HIGH);fan=1;
  raus();
  }

  digitalWrite(red,HIGH);
  digitalWrite(green,LOW);
}
if(innen_temp>=30)
{
  if(entfernung>=5){digitalWrite(fan_control_pin,HIGH);fan=1;raus();}
  
  digitalWrite(red,HIGH);
  digitalWrite(green,LOW);
}
if(digitalSensor==0)
{
  if(entfernung>=5){digitalWrite(fan_control_pin,HIGH);fan=1;raus();}
  
  digitalWrite(red,HIGH);
  digitalWrite(green,LOW);
}
if(analogSensor>=1200)
{
  if(entfernung>=5){digitalWrite(fan_control_pin,HIGH);fan=1;raus();}
  digitalWrite(red,HIGH);
  digitalWrite(green,LOW);
}

if(h>=80)
{
  if(entfernung>=5){digitalWrite(fan_control_pin,HIGH);fan=1;raus();}
  digitalWrite(red,HIGH);
  digitalWrite(green,LOW);
}
if(h<=20)
{
  if(entfernung>=5){digitalWrite(fan_control_pin,HIGH);fan=1;raus();}
  digitalWrite(red,HIGH);
  digitalWrite(green,LOW);
}


  





  
//TFtdisplay
//---------------------------------------------------------------------------
  tft.print(t);
  tft.println("C");
  tft.println("");
  tft.println(h);
  tft.println("g/m3");
  tft.println("");
  tft.print (analogSensor);
  tft.println("Co2");
  tft.println("");
  tft.print(entfernung);
  tft.println("cm");
  if(digitalSensor==0)
{tft.println("CO2 Warnung");
  }
  if(analogSensor>=1200)
{tft.println("CO2 Warnung");
  }
  


//Mqttpublish
//---------------------------------------------------------------------------

 client.publish("ES/WS20/gruppe4/window/distance",String(entfernung) , true);

 client.publish("ES/WS20/gruppe4/window/Co2",String(analogSensor) , true);
  
 client.publish("ES/WS20/gruppe4/window/fan",String(fan) , true); // You can activate the retain flag by setting the third parameter to true
 
  


  
  delay(2000);
}

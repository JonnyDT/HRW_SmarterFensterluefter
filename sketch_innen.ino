
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
int gasa= 36;
int xpos=0; 
int ypos=0; 

DHT dht(DHTPIN, DHTTYPE);


//Oled screen implementierung
TFT_eSPI tft = TFT_eSPI();

EspMQTTClient client(
  "Jonny-netgear",
  "123456789",
  "hrw-fablab.de", // <----------- DAS HAT SICH VERÄNDERT
  "gruppe4",
  "Kj+bu79P2LM8gX6b",
  "esws20g4fx-esp32"
);

void setup() {
  dht.begin();
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  pinMode(gasa, INPUT);
  // put your setup code here, to run once:dht.begin();

}
void onConnectionEstablished()
{
//  client.subscribe("ES/WS20/gruppe4/interior/temperature", [](const String & payload) { // <-- Temp außen
//    innen_temp = payload.toInt();
//    
//    updateDisplay();
//  });
  
}
void updateDisplay() {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 10);
  }
  
void loop() {
  int analogSensor = analogRead(gasa);
   float t = dht.readTemperature();
  float h = dht.readHumidity();
  tft.setTextColor(TFT_WHITE,TFT_BLACK); 
  tft.setCursor(xpos,ypos); 
  // put your main code here, to run repeatedly:

  tft.print(t);
  tft.println("C");
  tft.println("");
  tft.println(h);
  tft.println("%");
  tft.println("");
  tft.print (analogSensor);
  tft.println("Co2");

  
delay(5000);

}

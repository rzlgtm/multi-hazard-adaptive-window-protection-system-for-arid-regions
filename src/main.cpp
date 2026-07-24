#include <Arduino.h>
#include <DHT.h>

//temperature and humidity sensor init
#define DHTPIN 1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE); 

unsigned long lastDHTReadTime = 0;        //the last time DHT was read
const unsigned long dhtInterval = 2000;   //breathing for DHT

unsigned long currentTime;
float humidity, temp;

void setup(){
  Serial.begin(9600);
  dht.begin();
}

void loop(){

  currentTime = millis();
  if (currentTime - lastDHTReadTime >= dhtInterval){
    lastDHTReadTime = currentTime;
    humidity = dht.readHumidity();
    temp = dht.readTemperature();

    //telemetry
    if (isnan(humidity) || isnan(temp)) {
      Serial.println(F("failed to read from h&t sensor"));
    } else {
      Serial.print(F("Humidity: ")); Serial.print(humidity);
      Serial.print(F("%  Temperature: ")); Serial.print(temp); Serial.println(F("°C"));
    }
  }

}
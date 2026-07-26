#include <Arduino.h>
#include <DHT.h>
#include <algorithm>

//temperature and humidity sensor init
const int DHTPIN = 1;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE); 
unsigned long lastDHTReadTime = 0;        //the last time DHT was read
const unsigned long dhtInterval = 2000;   //breathing for DHT
float currentHum, currentTemp;
float initialTemp, initialHum;

//motor
const int motorIn1 = 25;
const int motorIn2 = 26;
const int motorEnA = 27;
const int freq = 30000;
const int ledChannel = 0;
const int resolution = 8;

//MQ-135 gas detector
const int gasRead = 4; //some analog pin
int currentGas;

//rain detector
const int rainRead = 2; //some analog pin
int currentRain;

//photoresistor
const int lightRead = 3; //some analog pin
float initialLight;
float currentLight;

// trend, history tracking 
unsigned long lastTrendCheckTime = 0;
const unsigned long trendInterval = 5000; 

//light SDT
const int maxLightPoints = 5;
float lightHistory[maxLightPoints] = {0.0}; 
int lightIndex = 0;
float lightFluctuation, maxLight, minLight;

//temp SDT
const int maxTempPoints = 5;
float tempHistory[maxTempPoints] = {0.0};
int tempIndex = 0;
float tempFluctuation, maxTemp, minTemp;

const int maxHumPoints = 5;
float humHistory[maxHumPoints] = {0.0};
int humIndex = 0;
float humFluctuation, maxHum, minHum;

//states
bool windowIsOpen = true;

unsigned long currentTime;

void closeWindow();
void openWindow();
void stopMotor();
void sendPhoneNotification(String reason);

int i;

void setup(){
  Serial.begin(9600);
  dht.begin();

  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorEnA, OUTPUT);

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(motorEnA, ledChannel);

  pinMode(gasRead, INPUT);
  pinMode(rainRead, INPUT);
  pinMode(lightRead, INPUT);

  // light level baseline
  initialLight = analogRead(lightRead);
  for(i = 0; i < maxLightPoints; i++) {
    lightHistory[i] = initialLight;
  }
  //temp baseline
  initialTemp = dht.readTemperature();
  for(i = 0; i< maxTempPoints; i++) { 
    tempHistory[i] = initialTemp;
  }
  // humidity baseline
  initialHum = dht.readHumidity();
  for(i = 0; i< maxHumPoints; i++) { 
    humHistory[i] = initialHum;
  }
}

void loop(){
  currentTime = millis();

  if (currentTime-lastTrendCheckTime >= trendInterval){
    lastTrendCheckTime = currentTime;

    currentLight = analogRead(lightRead);
    currentGas = analogRead(gasRead);
    currentRain = analogRead(rainRead); // * maybe digitalRead??
    currentTemp = dht.readTemperature();
    currentHum = dht.readHumidity();

    //recording light trend
    lightHistory[lightIndex] = currentLight;
    lightIndex = (lightIndex + 1) % maxLightPoints; //circular buffer
    auto lightResult = std::minmax_element(lightHistory, lightHistory + maxLightPoints);
    maxLight = *lightResult.second;
    minLight = *lightResult.first;
    lightFluctuation = maxLight - minLight;  //finding variance in light readings

    //recording temp trend
    tempHistory[tempIndex] = currentTemp;
    tempIndex = (tempIndex+1)%maxTempPoints;
    auto tempResult = std::minmax_element(tempHistory, tempHistory + maxTempPoints);
    maxTemp = *tempResult.second;
    minTemp = *tempResult.first;
    tempFluctuation = maxTemp - minTemp;

    //recording humidity trend
    humHistory[humIndex] = currentHum;
    humIndex = (humIndex + 1)%maxHumPoints;
    auto humResult = std::minmax_element(humHistory, humHistory + maxHumPoints);
    maxHum = *humResult.second;
    minHum = *humResult.first;
    humFluctuation = maxHum - minHum;

    //RAIN: rainRead high + (temperature drop || humidity rise)

    //DUST STORM: humidity drop + light fluctuation

    //GAS: gas sensor

    //EXTREME HEAT: temp high
  }

}

void closeWindow() {
  analogWrite(motorEnA, 150);
  digitalWrite(motorIn1, HIGH);
  digitalWrite(motorIn2, LOW);
  delay(3000); // * force sensor?
  stopMotor();
}

void openWindow() {
  analogWrite(motorEnA, 150);
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, HIGH);
  delay(3000);
  stopMotor();
}

void stopMotor() {
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
  analogWrite(motorEnA, 0);
}

void sendPhoneNotification(String reason){
  Serial.print(F('[NOTIFICATION SENT]: ')); Serial.println(reason); // ! add actual notification
}
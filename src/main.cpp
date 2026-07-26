#include <Arduino.h>
#include <DHT.h>

//temperature and humidity sensor init
const int DHTPIN = 1;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE); 
unsigned long lastDHTReadTime = 0;        //the last time DHT was read
const unsigned long dhtInterval = 2000;   //breathing for DHT
float humidity, temp;

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
int initialLight;
int currentLight;

// trend, history tracking 
unsigned long lastTrendCheckTime = 0;
const unsigned long trendInterval = 5000; 
int lightHistory[5] = {0};                 // sliding window for light fluctuations
int lightIndex = 0;
int baselineLight = 0;

//states
bool windowIsOpen = true;

unsigned long currentTime;

void closeWindow();
void openWindow();
void stopMotor();
void sendPhoneNotification(String reason);


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

  //TODO: light level baseline for swinging door trending algorithm
  initialLight = analogRead(lightRead);
  for(int i = 0; i < 5; i++) {
    lightHistory[i] = initialLight;
  }
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

  if (currentTime-lastTrendCheckTime >= trendInterval){
    lastTrendCheckTime = currentTime;

    currentLight = analogRead(lightRead);
    currentGas = analogRead(gasRead);
    currentRain = analogRead(rainRead); // * maybe digitalRead??


  }

}
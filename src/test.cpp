#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "H153-381_3A6C";
const char* password = "hqrcrhtvbx3";
const char* ntfy_topic = "multi-hazard-window-protection-system"; 
String serverPath = "https://ntfy.sh/" + String(ntfy_topic);
HTTPClient http;

const int DHTPIN = 32;
const int DHTTYPE = DHT22;
DHT dht(DHTPIN, DHTTYPE); 

const int motorIn1 = 17;
const int motorIn2 = 16;
const int motorEnA = 4;
const int freq = 30000;
const int ledChannel = 0;
const int resolution = 8;

const int gasRead = 33;   // MQ-135 analog pin
const int rainRead = 34;   // Rain sensor digital pin
const int lightRead = 35;  // Photoresistor analog pin

unsigned long lastTestTime = 0;
const unsigned long testInterval = 2000; 
void sendTestNotification();

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println(F("\n--- STARTING HARDWARE DIAGNOSTIC SUITE ---"));

  dht.begin();
  pinMode(gasRead, INPUT);
  pinMode(rainRead, INPUT);
  pinMode(lightRead, INPUT);

  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorEnA, OUTPUT);

  #if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
    ledcAttach(motorEnA, freq, resolution);
  #else
    ledcSetup(0, freq, resolution);
    ledcAttachPin(motorEnA, 0);
  #endif

  Serial.print(F("Connecting to Wi-Fi: "));
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  unsigned long wifiStart = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - wifiStart < 10000) {
    delay(500);
    Serial.print(F("."));
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("\n[SUCCESS] wi-fi connected!"));
    sendTestNotification();
  } else {
    Serial.println(F("\n[WARNING] wi-fi connection failed"));
  }

  Serial.println(F("motor test: forward"));
  analogWrite(motorEnA, 150);
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, HIGH);
  delay(1000);
  
  Serial.println(F("motor test: reverse"));
  digitalWrite(motorIn1, HIGH);
  digitalWrite(motorIn2, LOW);
  delay(1000);
  
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
  analogWrite(motorEnA, 0);
  Serial.println(F("motor test complete"));
}

void loop() {
  unsigned long currentTime = millis();

  // Print live sensor data every 2 seconds
  if (currentTime - lastTestTime >= testInterval) {
    lastTestTime = currentTime;

    // Read DHT22
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // Read Analog & Digital Sensors
    int rawLight = analogRead(lightRead);
    int rawGas = analogRead(gasRead);
    int rawRain = digitalRead(rainRead);

    Serial.println(F("----------------------------------------"));
    
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println(F("[ERROR] dht22 failed to read!"));
    } else {
      Serial.print(F("Temp: ")); Serial.print(temperature); Serial.print(F(" °C | "));
      Serial.print(F("Humidity: ")); Serial.print(humidity); Serial.println(F(" %"));
    }

    Serial.print(F("Photoresistor: ")); Serial.println(rawLight);

    Serial.print(F("Gas Sensor: ")); Serial.println(rawGas);

    // pull down wiring check
    Serial.print(F("Rain Sensor: ")); 
    if (rawRain == HIGH) {
      Serial.println(F("HIGH (wet)"));
    } else {
      Serial.println(F("LOW (dry)"));
    }
    
    Serial.println(F("motor test: reverse"));
    digitalWrite(motorIn1, HIGH);
    digitalWrite(motorIn2, LOW);
    delay(10000);
    
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, LOW);
    analogWrite(motorEnA, 0);
  }
}

void sendTestNotification() {
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(serverPath);
    http.addHeader("Content-Type", "text/plain");
    http.addHeader("X-Title", "Diagnostic Test");
    
    int httpResponseCode = http.POST("Hardware diagnostic test started successfully.");
    if (httpResponseCode > 0) {
      Serial.println(F("[SUCCESS] Test notification sent to ntfy.sh!"));
    } else {
      Serial.print(F("[ERROR] Failed to send notification. HTTP code: "));
      Serial.println(httpResponseCode);
    }
    http.end();
  }
}
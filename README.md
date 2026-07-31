# Multi-Hazard Window Protection System

An automated, IoT-enabled window protection system powered by an **ESP32** microcontroller. This project monitors environmental hazards in real-time—including rain, dust/sandstorms, hazardous gases, and extreme heat—and automatically closes or opens a window using a DC motor while sending instant push notifications via ntfy.sh.

<a href= 'https://youtu.be/_L7gEJOztdI'>Video Presentation</a>

---

## Features

* **Automated Hazard Detection:**
* **Rain:** Detects moisture and triggers closure based on rain sensor signals.
* **Dust/Sandstorms:** Monitors humidity drops and light fluctuations to identify extreme dry conditions.
* **Contaminated Air/Odor:** Utilizes an MQ-series gas sensor to detect sulfurous air.
* **Extreme Heat:** Tracks temperature spikes exceeding predefined heatwave limits.


* **Data Filtering:** Implements median-filtering and circular buffer trend tracking (min/max variance) to prevent false triggers from sensor noise.
* **IoT Push Notifications:** Integrates with `ntfy.sh` over Wi-Fi to send instant alert messages directly to your phone or desktop.
* **Mechanical Safety:** Incorporates a physical limit switch and a safety timeout mechanism to protect the window mechanism and motor hardware.

---

## Hardware Components

* **Microcontroller:** ESP32 Development Board
* **Sensors:**
  * DHT22
  * MQ-Series Gas Sensor (MQ-135)
  * Rain Sensor Module(2 wires with a 10K ohm pull-down resistor, short circuited by water drops)
  * Photoresistor (LDR) with analog circuit

* **Actuators & Control:**
  * DC Motor with Motor Driver (TB6612FNG)
  * Mechanical Limit Switch (4-pin Button)


---

## Pinout Mapping

| Component | ESP32 Pin | Description |
| --- | --- | --- |
| **Limit Switch** | GPIO 21 | Safety stop switch (INPUT_PULLUP) |
| **DHT Sensor** | GPIO 14 | Temperature and humidity data line |
| **Motor IN1** | GPIO 17 | Motor direction control 1 |
| **Motor IN2** | GPIO 16 | Motor direction control 2 |
| **Motor EnB (PWM)** | GPIO 4 | Motor speed control / PWM channel |
| **STBY (Standby)** | GPIO 22 | Motor driver standby pin |
| **Gas Sensor** | GPIO 33 | Analog gas reading (`gasRead`) |
| **Rain Sensor** | GPIO 34 | Rain module digital/analog reading (`rainRead`) |
| **Photoresistor** | GPIO 35 | Analog light level reading (`lightRead`) |

---

## Software & Dependencies

The firmware is written in C++ for the **Arduino IDE** (targeting the ESP32 platform). Ensure you have the following libraries installed via the Arduino Library Manager:

* `DHT sensor library` (by Adafruit)
* `WiFi` (Built-in for ESP32)
* `HTTPClient` (Built-in for ESP32)
* `<algorithm>` (Standard C++ library)

---

## Configuration & Setup

1. **Clone the Repository:**
```bash
git clone https://github.com/your-username/multi-hazard-window-protection.git

```


2. **Configure Wi-Fi and Notifications:**
Open the source file and update your network credentials and notification topic:
```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* ntfy_topic = "your-unique-ntfy-topic-name";

```


3. **Upload Firmware:**
* Select your ESP32 board and correct COM port in the Arduino IDE.
* Compile and flash the code to your ESP32 board.

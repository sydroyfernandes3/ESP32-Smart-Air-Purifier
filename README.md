# ESP32-Smart-Air-Purifier
Air quality monitor pushing sensor data to Thingspeak
# IoT Smart Air Quality Monitor & Purifier with Web Dashboard

## 📌 Project Overview
This project is an automated, full-stack IoT environmental monitoring system built on the ESP32 microcontroller. It continuously tracks ambient temperature, humidity, and air quality (gas levels). 

Based on real-time readings, the system autonomously triggers hardware responses (LED indicators, an active buzzer, and a ventilation fan via a relay). Furthermore, it pushes live data to a ThingSpeak cloud backend, which is then fetched and visualized on a custom frontend web application for remote monitoring.

## 🚀 Key Features
* **Real-Time Sensing:** Reads temperature and humidity via a DHT22 sensor and gas/air quality via an MQ135 analog sensor.
* **Automated Hardware Control:** Automatically toggles a relay (to power an exhaust fan or purifier) and triggers an alarm buzzer when air quality drops to "Poor" levels.
* **Custom Web App Interface:** Includes a dedicated frontend web application that fetches and visualizes the ESP32 sensor data in real-time by querying the ThingSpeak REST API.
* **Local Visual Output:** Displays real-time metrics and status ("Good", "Moderate", "Poor") on a 16x2 I2C LCD screen.
* **Cloud Backend Integration:** Pushes sensor data securely to a ThingSpeak channel every 20 seconds over Wi-Fi.

## 🛠️ Hardware Requirements
* ESP32 Development Board
* MQ135 Air Quality / Gas Sensor
* DHT22 Temperature & Humidity Sensor
* 16x2 LCD Display with I2C Module
* 5V Relay Module (to control an external fan/purifier)
* Active Buzzer
* 3x LEDs (Green, Yellow, Red)
* Jumper Wires & Breadboard

## 🔌 Pin Mapping / Circuit Connections

| Component | ESP32 Pin | Notes |
| :--- | :--- | :--- |
| DHT22 Data | GPIO 4 | - |
| MQ135 Analog | GPIO 34 | Analog input |
| LCD I2C SDA | GPIO 21 | I2C Data |
| LCD I2C SCL | GPIO 22 | I2C Clock |
| Green LED | GPIO 14 | Indicates "Good" air quality |
| Yellow LED | GPIO 27 | Indicates "Moderate" air quality |
| Red LED | GPIO 26 | Indicates "Poor" air quality |
| Fan Relay | GPIO 25 | Triggers ventilation fan |
| Active Buzzer | GPIO 33 | Alarm for poor air quality |

## 💻 Software & Libraries
To run this code, you will need the Arduino IDE configured for the ESP32 board, along with the following libraries:
* `Wire.h` (Built-in for I2C)
* `WiFi.h` (Built-in for ESP32)
* `LiquidCrystal_I2C` 
* `DHT sensor library` 
* `ThingSpeak` 

## ⚙️ Setup & Installation

### 1. Hardware Setup
1. Clone this repository or download the source code.
2. Open the `.ino` file in the Arduino IDE.
3. Update the Wi-Fi credentials in the code:
   ```cpp
   #define WIFI_SSID "YOUR_WIFI_NAME"
   #define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
   

   #Update the ThingSpeak credentials with your specific Channel Number and Write API Key:

unsigned long myChannelNumber = YOUR_CHANNEL_ID; 
const char * myWriteAPIKey = "YOUR_THINGSPEAK_WRITE_API_KEY";

  #Compile and upload the code to your ESP32 board.

Greenhouse Automation System with IoT and Arduino Cloud Integration

This project is an IoT-based Greenhouse Automation System developed for the CSE328 Internet of Things course at Akdeniz University. It uses the ESP32 Development Kit to monitor and control environmental parameters (temperature, humidity, soil moisture, light intensity) in a greenhouse, ensuring optimal plant growth. The system integrates sensors, actuators, and Arduino IoT Cloud for real-time monitoring and remote control. A local OLED display provides on-site data visualization, and a button allows switching between display modes.
Features

Environmental Monitoring: Measures temperature, humidity (BME280), soil moisture, and light intensity.
Automated Control:
Fan: Activates when temperature exceeds 25.5°C.
LED: Turns on/off based on light intensity (threshold: 400).
Water Valve: Opens (servo at 60°) for 1000ms when soil moisture exceeds 1500, closes (120°) when moist.


Remote Access: Arduino IoT Cloud dashboard for real-time data visualization and manual control of fan, LED, and valve.
Local Display: SH1106 OLED shows three modes:
Temperature and humidity.
Actuator states (Fan: ON/OFF, Led: ON/OFF, Valve: OPEN/CLOSE).
Soil moisture and light intensity.


Power Efficiency: Relay module for servo motor minimizes power draw.
Secure Communication: MQTT over TLS for cloud data exchange, WPA2-PSK for Wi-Fi.

Used Components
Hardware

ESP32 Development Kit: Central microcontroller.
BME280 Sensor: Temperature and humidity (I2C, address 0x76).
Soil Moisture Sensor: Analog pin 34.
Light Sensor: Analog pin 35.
Servo Motor (MG996R): PWM pin 13, relay module (pin 14).
Fan: Relay module (pin 15).
LED Module: Pin 12.
SH1106 OLED Display: 128x64, I2C (address 0x3C).
Button: Pin 4, internal pull-up.
Power Supply: Arduino, powerbank, jumper cables, resistors.

Software

Arduino IDE: For coding and uploading.
Libraries:
WiFi.h: ESP32 Wi-Fi connectivity.
ArduinoIoTCloud.h: Cloud integration.
Adafruit_BME280.h, Adafruit_GFX.h, Adafruit_SH110X.h: Sensor and OLED.
ESP32Servo.h: Servo control.


Arduino IoT Cloud: Real-time dashboard and control.
GitHub: Source code management.

Installation
Prerequisites

Arduino IDE: Download and install from arduino.cc.
ESP32 Board Support: Add ESP32 to Arduino IDE:
Go to File > Preferences, add to Additional Boards Manager URLs:https://raw.githubusercontent.com/espressif/arduino-esp32/master/package_esp32_index.json


Go to Tools > Board > Boards Manager, search for esp32, install.


Libraries: Install via Arduino Library Manager:
Adafruit_BME280
Adafruit_GFX
Adafruit_SH110X
ESP32Servo
ArduinoIoTCloud
WiFi


Arduino IoT Cloud Account: Sign up at cloud.arduino.cc.

Hardware Setup

Connect components to ESP32 as follows:
BME280: I2C (SCL: GPIO 22, SDA: GPIO 21, address 0x76).
Soil Moisture Sensor: Analog pin 34.
Light Sensor: Analog pin 35.
Servo Motor: PWM pin 13, relay module (pin 14) for power.
Fan: Relay module (pin 15).
LED: Pin 12.
OLED (SH1106): I2C (SCL: GPIO 22, SDA: GPIO 21, address 0x3C).
Button: Pin 4 (with internal pull-up).


Power the ESP32 and components using an Arduino or powerbank.
Assemble on a breadboard, ensure proper grounding and voltage (3.3V/5V).

Software Setup

Clone this repository:git clone https://github.com/muhammeteryilmaz/greenhouseAutomation.git


Open greenHouseAutomation.ino in Arduino IDE.
Configure Arduino IoT Cloud:
Create a new Thing (greenHouseAutomation) in Arduino IoT Cloud.
Add variables: temperature, humidity, soilMoisture, lightIntensity, fanStatus, valveStatus, ledStatus, servoAngle, fanControl, ledControl, valveControl, tempThreshold, soilMoistureThreshold, lightThreshold.
Generate arduino_secrets.h with your Wi-Fi credentials and device key:#define SECRET_SSID "your_wifi_ssid"
#define SECRET_PASS "your_wifi_password"
#define SECRET_DEVICE_KEY "your_device_key"

Upload the code to ESP32:
Select Tools > Board > ESP32 Arduino > ESP32 Dev Module.
Connect ESP32 via USB, select the correct port.
Click Upload.



Usage

Power On: Connect the ESP32 to power. The system initializes, connects to Wi-Fi, and syncs with Arduino IoT Cloud.
Local Monitoring:
Press the button (GPIO 4) to cycle through OLED display modes:
Mode 0: Temperature (e.g., 25.3°C) and humidity (e.g., 60%).
Mode 1: Actuator states (Fan: ON/OFF, Led: ON/OFF, Valve: OPEN/CLOSE).
Mode 2: Soil moisture (e.g., 1200) and light intensity (e.g., 500).


Remote Monitoring and Control:
Open Arduino IoT Cloud dashboard.
View real-time charts for temperature, humidity, soil moisture, and light intensity.
Use switches (fanControl, ledControl, valveControl) to manually control actuators.
Adjust thresholds (tempThreshold, soilMoistureThreshold, lightThreshold) for automated control.


Automation:
Fan activates if temperature > 25.5°C.
LED toggles based on light intensity (<400).
Valve opens/closes based on soil moisture (>1500).





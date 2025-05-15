# greenhouseAutomation

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


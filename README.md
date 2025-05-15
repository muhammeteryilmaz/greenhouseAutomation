# Greenhouse Automation System with IoT and Arduino Cloud Integration

This is an IoT-based Greenhouse Automation System developed for the **CSE328 Internet of Things** course at **Akdeniz University**. It utilizes the **ESP32 Development Kit** to monitor and automate key environmental parameters (temperature, humidity, soil moisture, light intensity) to support optimal plant growth.

Real-time monitoring, control, and automation are achieved via **Arduino IoT Cloud**, local OLED display, and a push-button for switching display modes.

---

## Features

- **Environmental Monitoring**
  - Measures temperature and humidity using **BME280**
  - Detects **soil moisture** and **light intensity**
  
- **Automated Control**
  - **Fan**: Activates if temperature > `25.5°C`
  - **LED**: Turns on/off based on light intensity (threshold: `400`)
  - **Water Valve**: Opens when soil moisture > `1500`, closes otherwise
  
- **Remote Access**
  - Real-time data and manual actuator control via **Arduino IoT Cloud**

- **Local Display**
  - **SH1106 OLED** shows:
    - Mode 0: Temperature & humidity
    - Mode 1: Actuator states
    - Mode 2: Soil moisture & light intensity

- **Power Efficiency**
  - Relay-controlled servo for water valve reduces power usage

- **Secure Communication**
  - Uses **MQTT over TLS** and **WPA2-PSK** for secure connectivity

---

## Hardware Components

| Component            | Details                                    |
|----------------------|---------------------------------------------|
| ESP32 Dev Kit        | Main microcontroller                        |
| BME280 Sensor        | Temp & humidity via I2C (Addr: `0x76`)     |
| Soil Moisture Sensor | Analog input (GPIO `34`)                   |
| Light Sensor         | Analog input (GPIO `35`)                   |
| Servo Motor (MG996R) | PWM: `GPIO 13`, Power via Relay (`GPIO 14`)|
| Fan                  | Controlled via Relay (GPIO `15`)           |
| LED Module           | Digital output (`GPIO 12`)                 |
| OLED Display SH1106  | I2C: SDA `GPIO 21`, SCL `GPIO 22` (`0x3C`) |
| Button               | Digital input (`GPIO 4`, internal pull-up) |
| Power Supply         | Arduino/Powerbank                          |

---

## Software & Libraries

- **IDE**: [Arduino IDE](https://www.arduino.cc/en/software)
- **Cloud**: [Arduino IoT Cloud](https://cloud.arduino.cc)
- **Libraries Used**:
  - `WiFi.h`
  - `ArduinoIoTCloud.h`
  - `Adafruit_BME280.h`
  - `Adafruit_GFX.h`
  - `Adafruit_SH110X.h`
  - `ESP32Servo.h`

---

## Installation

### 1. Prerequisites

- Install **Arduino IDE**
- Add **ESP32 Board Support**:
  - File > Preferences > Additional Board URLs:  
    ```
    https://raw.githubusercontent.com/espressif/arduino-esp32/master/package_esp32_index.json
    ```
  - Tools > Board > Boards Manager > Search **esp32**, then install

- Install Libraries via Library Manager:
  - Adafruit BME280
  - Adafruit GFX
  - Adafruit SH110X
  - ESP32Servo
  - ArduinoIoTCloud
  - WiFi

- Create an **Arduino IoT Cloud account**

---

### 2. Hardware Setup

| Component           | ESP32 Pin |
|---------------------|-----------|
| BME280 (I2C)         | SDA: GPIO 21, SCL: GPIO 22 |
| Soil Moisture Sensor | GPIO 34  |
| Light Sensor         | GPIO 35  |
| Servo Motor          | PWM GPIO 13 via Relay GPIO 14 |
| Fan                  | Relay GPIO 15 |
| LED                  | GPIO 12  |
| OLED SH1106          | SDA: GPIO 21, SCL: GPIO 22 |
| Button               | GPIO 4 (with pull-up) |

> **Power:** Use an external power bank or Arduino power source. Assemble everything on a breadboard with proper grounding.

---

### 3. Software Setup

```bash
git clone https://github.com/muhammeteryilmaz/greenhouseAutomation.git
cd greenhouseAutomation

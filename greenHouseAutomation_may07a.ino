#include "arduino_secrets.h"
#include "thingProperties.h"
#include <WiFi.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <ESP32Servo.h>
#include <Wire.h>

// Pin definitions
#define SOIL_MOISTURE_PIN 34
#define LIGHT_SENSOR_PIN  35
#define SERVO_PIN         13
#define FAN_RELAY_PIN     15
#define LED_PIN           12
#define SERVO_RELAY_PIN   14 // Relay IN2 for servo power
#define BUTTON_PIN        4  // Button for screen change

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define i2c_Address 0x3C
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// BME280
Adafruit_BME280 bme;

// Servo
Servo servo;

// Valve control variables
#define VALVE_OPEN_VALUE 60   // Continuous servo: rotate to open
#define VALVE_CLOSE_VALUE 120 // Continuous servo: rotate to close
#define VALVE_ROTATION_TIME 1000 // 1000ms rotation
static bool isValveOpen = false; // Tracks valve state (local)

// Screen control
volatile int screenMode = 0; // 0: Temp/Humidity, 1: Fan/LED/Valve, 2: Soil/Light

void IRAM_ATTR changeScreen() {
  screenMode = (screenMode + 1) % 3; // Cycle through 0, 1, 2
}

void setup() {
  // Initialize pins
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(FAN_RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SERVO_RELAY_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Button with internal pull-up
  digitalWrite(FAN_RELAY_PIN, HIGH); // Relay off (active-low)
  digitalWrite(LED_PIN, LOW);
  digitalWrite(SERVO_RELAY_PIN, HIGH); // Servo power off

  // Attach interrupt for button
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), changeScreen, FALLING);

  // Initialize servo
  servo.setPeriodHertz(50); // MG996R uses 50Hz PWM
  servo.attach(SERVO_PIN, 500, 2500); // 500µs (~0), 2500µs (~180)
  servo.write(90); // Neutral (stop)
  Serial.begin(115200);
  delay(1500);
  Serial.println("Servo initialized to neutral");

  // Initialize OLED
  if (!display.begin(i2c_Address, true)) {
    Serial.println(F("SH1106 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("Initializing..."));
  display.display();

  // Initialize BME280
  if (!bme.begin(0x76)) {
    Serial.println(F("Could not find BME280 sensor!"));
    for (;;);
  }

  // Initialize cloud properties
  initProperties();

  // Connect to Arduino IoT Cloud
  Serial.println("Connecting to Arduino Cloud...");
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  while (!ArduinoCloud.connected()) {
    ArduinoCloud.update();
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Arduino Cloud");

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();

  // Read sensors
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  lightIntensity = analogRead(LIGHT_SENSOR_PIN);

  // Automation logic (local control, unless manual control is enabled)
  if (!fanControl) {
    if (temperature > tempThreshold) {
      fanStatus = true;
      digitalWrite(FAN_RELAY_PIN, LOW); // Relay on (active-low)
    } else {
      fanStatus = false;
      digitalWrite(FAN_RELAY_PIN, HIGH); // Relay off
    }
  } else {
    digitalWrite(FAN_RELAY_PIN, fanStatus ? LOW : HIGH); // Manual control
  }

  if (!valveControl) {
    if (soilMoisture > soilMoistureThreshold && !isValveOpen) {
      valveStatus = true;
      digitalWrite(SERVO_RELAY_PIN, LOW); // Power servo
      servo.write(VALVE_OPEN_VALUE); // Open valve (60)
      delay(VALVE_ROTATION_TIME); // 1000ms
      servo.write(90); // Stop
      digitalWrite(SERVO_RELAY_PIN, HIGH); // Cut power
      isValveOpen = true;
      Serial.println("Valve opened (auto): isValveOpen = true, valveStatus = true");
    } else if (soilMoisture <= soilMoistureThreshold && isValveOpen) {
      valveStatus = false;
      digitalWrite(SERVO_RELAY_PIN, LOW); // Power servo
      servo.write(VALVE_CLOSE_VALUE); // Close valve (120)
      delay(VALVE_ROTATION_TIME); // 1000ms
      servo.write(90); // Stop
      digitalWrite(SERVO_RELAY_PIN, HIGH); // Cut power
      isValveOpen = false;
      Serial.println("Valve closed (auto): isValveOpen = false, valveStatus = false");
    }
  } else {
    // Manual control handled by onValveStatusChange
  }

  if (!ledControl) {
    if (lightIntensity < lightThreshold) {
      ledStatus = false;
      digitalWrite(LED_PIN, LOW);
    } else {
      ledStatus = true;
      digitalWrite(LED_PIN, HIGH);
    }
  } else {
    digitalWrite(LED_PIN, ledStatus ? HIGH : LOW); // Manual control
  }

  // Update OLED based on screenMode
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  if (screenMode == 0) {
    // Screen 0: Temperature and Humidity
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Temp: ");
    display.setTextSize(2);
    display.setCursor(0, 10);
    display.print(temperature);
    display.print(" C");
    display.setTextSize(1);
    display.setCursor(0, 35);
    display.print("Humidity: ");
    display.setTextSize(2);
    display.setCursor(0, 45);
    display.print(humidity);
    display.print(" %");
  } else if (screenMode == 1) {
    // Screen 1: Fan, Led, Valve Status
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Fan: ");
    display.print(fanStatus ? "ON" : "OFF");
    display.setCursor(0, 16);
    display.print("Led: ");
    display.print(ledStatus ? "ON" : "OFF");
    display.setCursor(0, 32);
    display.print("Valve: ");
    display.print(isValveOpen ? "OPEN" : "CLOSE");
  } else {
    // Screen 2: Soil Moisture and Light Intensity
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Soil: ");
    display.setTextSize(2);
    display.setCursor(0, 10);
    display.print(soilMoisture);
    display.setTextSize(1);
    display.setCursor(0, 35);
    display.print("Light: ");
    display.setTextSize(2);
    display.setCursor(0, 45);
    display.print(lightIntensity);
  }
  display.display();

  delay(500); // Update every 0.5 seconds
}

// Cloud property change handlers
void onFanStatusChange() {
  if (fanControl) {
    digitalWrite(FAN_RELAY_PIN, fanStatus ? LOW : HIGH);
    Serial.print("Fan manually set to: ");
    Serial.println(fanStatus ? "ON" : "OFF");
  }
}

void onValveStatusChange() {
  if (valveControl) {
    Serial.print("Manual valve change: valveStatus = ");
    Serial.println(valveStatus);
    if (valveStatus && !isValveOpen) {
      digitalWrite(SERVO_RELAY_PIN, LOW);
      servo.write(VALVE_OPEN_VALUE); // Open valve (60)
      delay(VALVE_ROTATION_TIME);
      servo.write(90);
      digitalWrite(SERVO_RELAY_PIN, HIGH);
      isValveOpen = true;
      valveStatus = true;
      Serial.println("Valve manually opened: isValveOpen = true, valveStatus = true");
    } else if (!valveStatus && isValveOpen) {
      digitalWrite(SERVO_RELAY_PIN, LOW);
      servo.write(VALVE_CLOSE_VALUE); // Close valve (120)
      delay(VALVE_ROTATION_TIME);
      servo.write(90);
      digitalWrite(SERVO_RELAY_PIN, HIGH);
      isValveOpen = false;
      valveStatus = false;
      Serial.println("Valve manually closed: isValveOpen = false, valveStatus = false");
    }
  }
}

void onServoAngleChange() {
  digitalWrite(SERVO_RELAY_PIN, LOW);
  servo.write(servoAngle);
  delay(VALVE_ROTATION_TIME);
  servo.write(90);
  digitalWrite(SERVO_RELAY_PIN, HIGH);
  isValveOpen = (servoAngle == VALVE_OPEN_VALUE); // true if servoAngle = 60
  valveStatus = isValveOpen;
  Serial.print("Servo angle set to: ");
  Serial.print(servoAngle);
  Serial.print(", isValveOpen = ");
  Serial.print(isValveOpen);
  Serial.print(", valveStatus = ");
  Serial.println(valveStatus);
}

void onLedStatusChange() {
  if (ledControl) {
    digitalWrite(LED_PIN, ledStatus ? HIGH : LOW);
    Serial.print("Led manually set to: ");
    Serial.println(ledStatus ? "ON" : "OFF");
  }
}

void onFanControlChange() {
  Serial.print("Fan control mode: ");
  Serial.println(fanControl ? "Manual" : "Auto");
}

void onLedControlChange() {
  Serial.print("Led control mode: ");
  Serial.println(ledControl ? "Manual" : "Auto");
}

void onValveControlChange() {
  Serial.print("Valve control mode: ");
  Serial.println(valveControl ? "Manual" : "Auto");
}

void onTemperatureChange() {
  Serial.print("Temperature updated: ");
  Serial.println(temperature);
}

void onHumidityChange() {
  Serial.print("Humidity updated: ");
  Serial.println(humidity);
}

void onSoilMoistureChange() {
  Serial.print("Soil moisture updated: ");
  Serial.println(soilMoisture);
}

void onLightIntensityChange() {
  Serial.print("Light intensity updated: ");
  Serial.println(lightIntensity);
}

void onTempThresholdChange() {
  Serial.print("Temperature threshold updated: ");
  Serial.println(tempThreshold);
}

void onSoilMoistureThresholdChange() {
  Serial.print("Soil moisture threshold updated: ");
  Serial.println(soilMoistureThreshold);
}

void onLightThresholdChange() {
  Serial.print("Light threshold updated: ");
  Serial.println(lightThreshold);
}
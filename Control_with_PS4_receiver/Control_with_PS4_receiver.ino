#include <Arduino.h> // Explicitly include Arduino core for ESP32 functions
#include <M5AtomS3.h>
#include <M5Unified.h>
#include <WiFi.h>
#include <esp_now.h>
#include <ESP32Servo.h>

Servo myServo;

// Motor 1 (Left or Front)
const int motor1Pin1 = 7;
const int motor1Pin2 = 6;
const int motor1PWM = 8;
const int motor1PWMChannel = 0; // LEDC channel for Motor 1

// Motor 2 (Right or Rear)
const int motor2Pin1 = 5;
const int motor2Pin2 = 38;
const int motor2PWM = 39;
const int motor2PWMChannel = 1; // LEDC channel for Motor 2

void moveMotor(String dir, int spd) {
  // Clamp speed to valid 8-bit range (0–255)
  spd = constrain(spd, 0, 255);

  if (dir == "F") {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
  } else if (dir == "B") {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
  } else {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
  }

  ledcWrite(motor1PWMChannel, spd); // Write to PWM channel
  ledcWrite(motor2PWMChannel, spd);
}

// Callback when data is received
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int len) {
  String cmd = String((char*)data);
  cmd.trim(); // Remove newline or space

  M5.Display.clear();
  M5.Display.setCursor(0, 0);

  if (cmd.startsWith("L")) {
    int angle = cmd.substring(1).toInt();
    myServo.write(angle);
    M5.Display.printf("Servo: %d\n", angle);
  } else if (cmd.startsWith("F") || cmd.startsWith("B")) {
    String dir = cmd.substring(0, 1);
    int spd = cmd.substring(1).toInt();
    moveMotor(dir, spd);
    M5.Display.printf("Motor: %s %d\n", dir.c_str(), spd);
  } else if (cmd == "S") {
    moveMotor("S", 0);
    M5.Display.println("Motor: Stop");
  } else {
    M5.Display.println("Unknown cmd");
  }
}

void setup() {
  M5.begin();
  Serial.begin(115200);
  M5.Display.begin();
  M5.Display.setTextSize(2);

  // Setup servo (uses LEDC internally, avoid channel 0 and 1)
  myServo.attach(2); // Explicitly use channel 2 for servo

  // Setup motor direction pins
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  // Setup PWM for motors
  ledcSetup(motor1PWMChannel, 5000, 8); // Channel 0, 5 kHz, 8-bit
  ledcAttachPin(motor1PWM, motor1PWMChannel); // Attach pin to channel
  ledcSetup(motor2PWMChannel, 5000, 8); // Channel 1, 5 kHz, 8-bit
  ledcAttachPin(motor2PWM, motor2PWMChannel); // Attach pin to channel
  ledcWrite(motor1PWMChannel, 0); // Initialize to 0
  ledcWrite(motor2PWMChannel, 0);

  // Setup ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    M5.Display.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  M5.Display.setCursor(0, 0);
  M5.Display.println("Receiver Ready");
}

void loop() {
  // Nothing here; just wait for ESP-NOW commands
}

/*#include <M5AtomS3.h>
#include <WiFi.h>
#include <esp_now.h>
#include <ESP32Servo.h>
#include <M5Unified.h>

Servo myServo;

// Motor 1 (Left or Front)
const int motor1Pin1 = 7;
const int motor1Pin2 = 6;
const int motor1PWM = 8;

// Motor 2 (Right or Rear)
const int motor2Pin1 = 5;
const int motor2Pin2 = 38;
const int motor2PWM = 39;

void moveMotor(String dir, int spd) {
  if (dir == "F") {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
  } else if (dir == "B") {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
  } else {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
  }

  ledcWrite(motor1PWM, spd);
  ledcWrite(motor2PWM, spd);
}

// === Callback when data is received ===
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  String cmd = String((char*)data);
  cmd.trim(); // remove newline or space

  M5.Display.clear();
  M5.Display.setCursor(0, 0);

  if (cmd.startsWith("L")) {
    int angle = cmd.substring(1).toInt();
    myServo.write(angle);
    M5.Display.printf("Servo: %d\n", angle);
  } else if (cmd.startsWith("F") || cmd.startsWith("B")) {
    String dir = cmd.substring(0, 1);
    int spd = cmd.substring(1).toInt();
    moveMotor(dir, spd);
    M5.Display.printf("Motor: %s %d\n", dir.c_str(), spd);
  } else if (cmd == "S") {
    moveMotor("S", 0);
    M5.Display.println("Motor: Stop");
  } else {
    M5.Display.println("Unknown cmd");
  }
}

void setup() {
  M5.begin();
  Serial.begin(115200);
  M5.Display.begin();
  M5.Display.setTextSize(2);

  // Setup servo
  myServo.attach(2);

  // Setup motor direction pins
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  // Attach LEDC PWM for motors (ESP32 v3.x)
  ledcAttach(motor1PWM, 5000, 8);
  ledcAttach(motor2PWM, 5000, 8);
  ledcWrite(motor1PWM, 0);
  ledcWrite(motor2PWM, 0);

  // Setup ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    M5.Display.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  M5.Display.setCursor(0, 0);
  M5.Display.println("Receiver Ready");
}

void loop() {
  // nothing here; just wait for ESP-NOW commands
}*/

/*// ===== Receiver Code (ESP-NOW -> Control) =====
#include <M5AtomS3.h>
#include <WiFi.h>
#include <esp_now.h>
#include <ESP32Servo.h>
#include <M5Unified.h>

Servo myServo;

int speed = 0;

// Motor 1 (Left or Front)
int motor1Pin1 = 7; // G7
int motor1Pin2 = 6; // G6
int motor1PWM  = 8; // G8

// Motor 2 (Right or Rear)
int motor2Pin1 = 5;  // G5
int motor2Pin2 = 38; // G38
int motor2PWM  = 39; // G39

void moveMotor(String dir, int spd) {
  if (dir == "F") {
    // Forward
    digitalWrite(7, HIGH); // motor1Pin1
    digitalWrite(6, LOW);  // motor1Pin2
    digitalWrite(5, HIGH); // motor2Pin1
    digitalWrite(38, LOW); // motor2Pin2
  } else if (dir == "B") {
    // Backward
    digitalWrite(7, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(38, HIGH);
  } else {
    // Stop
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);
    digitalWrite(38, LOW);
  }

  ledcWrite(8, spd);  // PWM on G8
  ledcWrite(39, spd); // PWM on G39
}

void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  String command = String((char *)incomingData);
  M5.Display.clear();
  M5.Display.setCursor(0, 0);

  if (command.startsWith("L")) {
    int angle = command.substring(1).toInt();
    myServo.write(angle);
    M5.Display.printf("L %d", angle);
  } 
  else if (command.startsWith("F") || command.startsWith("B")) {
    String dir = command.substring(0, 1);
    int spd = command.substring(1).toInt();
    moveMotor(dir, spd);
    M5.Display.printf("%s %d", dir.c_str(), spd);
  } 
  else if (command == "S") {
    moveMotor("S", 0);
    M5.Display.print("S");
  }
}

void setup() {
  M5.begin();
  Serial.begin(115200);
  myServo.attach(2); // Servo to G2
  // Set motor direction pins
  pinMode(7, OUTPUT); // motor1Pin1
  pinMode(6, OUTPUT); // motor1Pin2
  pinMode(5, OUTPUT); // motor2Pin1
  pinMode(38, OUTPUT); // motor2Pin2

  // Attach PWM (new LEDC API)
  ledcAttach(8, 5000, 8);   // PWM for motor 1 (left/front)
  ledcAttach(39, 5000, 8);  // PWM for motor 2 (right/rear)

  ledcWrite(8, 0);
  ledcWrite(39, 0);
  M5.Display.setCursor(0, 0);
  M5.Display.printf("Starting...\n");

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.print("Receiver Ready jaaaa55555");
}

void loop() {
}*/

/*#include <M5AtomS3.h>
#include <WiFi.h>
#include <esp_now.h>
#include <ESP32Servo.h>
#include <M5Unified.h>

Servo myServo;
int motorPin1 = 1;
int motorPin2 = 2;
int motorPWM = 3;
int speed = 0;

void setup() {
  M5.begin();
  Serial.begin(115200);
  myServo.attach(5);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  //ledcSetup(0, 5000, 8);
  //ledcAttachPin(motorPWM, 0);
  ledcAttach(motorPWM, 5000, 8);
  ledcWrite(motorPWM, speed); 
  M5.Display.setCursor(0, 0);
  M5.Display.printf("Starting...\n");
}

void moveMotor(String dir, int spd) {
  if (dir == "F") {
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
  } else if (dir == "B") {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
  } else {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
  }
  ledcWrite(0, spd);
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    
    if (command.startsWith("L")) {
      int angle = command.substring(1).toInt();
      myServo.write(angle);
      M5.Display.clear();
      M5.Display.setCursor(0, 0);
      M5.Display.printf("L %d", angle);
    }
    else if (command.startsWith("F") || command.startsWith("B")) {
      String dir = command.substring(0,1);
      int spd = command.substring(1).toInt();
      moveMotor(dir, spd);
      M5.Display.clear();
      M5.Display.setCursor(0, 0);
      M5.Display.printf("%s %d", dir.c_str(), spd);
    }
    else if (command == "S") {
      moveMotor("S", 0);
      M5.Display.clear();
      M5.Display.setCursor(0, 0);
      M5.Display.printf("S");
    }
  }
}*/

/*Servo myServo;
int angle = 90;

void setup() {
  M5.begin();
  Serial.begin(115200);
  myServo.attach(5);
  myServo.write(angle);
  M5.Display.setTextSize(2);
  M5.Display.printf("Starting...\n");
  M5.Display.printf("Servo angle: %d\n", angle);
}

void loop() {
  if (Serial.available()) {
    int val = Serial.read();
    myServo.write(val);

    M5.Display.clear();
    M5.Display.setCursor(0, 0);
    M5.Display.printf("Servo: %d", val);
  }
}*/




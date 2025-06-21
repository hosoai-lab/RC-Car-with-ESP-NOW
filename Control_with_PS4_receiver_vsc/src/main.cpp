#include <Arduino.h>
#include <M5AtomS3.h>
#include <M5Unified.h>
#include <WiFi.h>
#include <esp_now.h>
#include <ESP32Servo.h>

Servo myServo;

int IN1 = 7;   // motor1Pin1
int IN2 = 6;   // motor1Pin2
int ENA = 8;   // PWM motor 1

int IN3 = 5;   // motor2Pin1
int IN4 = 38;  // motor2Pin2
int ENB = 39;  // PWM motor 2

int pwmChannelA = 2;
int pwmChannelB = 3;

void moveMotor(String dir, int spd) {
  if (dir == "F") {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else if (dir == "B") {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }

  ledcWrite(pwmChannelA, spd);
  ledcWrite(pwmChannelB, spd);
}

String lastCmd = "";
bool cmdUpdated = false;

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int len) {
  lastCmd = String((char*)data);
  lastCmd.trim();
  cmdUpdated = true;
}

void setup() {
  M5.begin();
  Serial.begin(115200);
  M5.Display.setCursor(0, 0);
  M5.Display.println("PWM Setup");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // PWM Setup แบบเดิม (2.x)
  ledcSetup(pwmChannelA, 5000, 8); // 5kHz, 8-bit resolution
  ledcAttachPin(ENA, pwmChannelA);

  ledcSetup(pwmChannelB, 5000, 8);
  ledcAttachPin(ENB, pwmChannelB);

  // Servo
  myServo.attach(2); // G2

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
  if (cmdUpdated) {
    M5.Display.clear();
    M5.Display.setCursor(0, 30);

    if (lastCmd.startsWith("L")) {
      int angle = lastCmd.substring(1).toInt();
      myServo.write(angle);
      M5.Display.printf("Servo: %d\n", angle);
    } else if (lastCmd.startsWith("F") || lastCmd.startsWith("B")) {
      String dir = lastCmd.substring(0, 1);
      int spd = lastCmd.substring(1).toInt();
      moveMotor(dir, spd);
      M5.Display.printf("Motor: %s %d\n", dir.c_str(), spd);
    } else if (lastCmd == "S") {
      moveMotor("S", 0);
      M5.Display.println("Motor: Stop");
    } else {
      M5.Display.println("Unknown cmd");
    }

    cmdUpdated = false;
  }

  delay(10);

}

// Callback when data is received
/*void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int len) {
  String cmd = String((char*)data);
  cmd.trim(); // Remove newline or space

  M5.Display.clear();
  M5.Display.setCursor(0, 0);

  if (cmd.startsWith("L")) {
    int angle = cmd.substring(1).toInt();
    myServo.write(angle);
    M5.Display.setCursor(0, 20);
    M5.Display.printf("Servo: %d\n", angle);
  } else if (cmd.startsWith("F") || cmd.startsWith("B")) {
    String dir = cmd.substring(0, 1);
    int spd = cmd.substring(1).toInt();
    moveMotor(dir, spd);
    M5.Display.setCursor(0, 40);
    M5.Display.printf("Motor: %s %d\n", dir.c_str(), spd);
  } else if (cmd == "S") {
    moveMotor("S", 0);
    M5.Display.setCursor(0, 40);
    M5.Display.println("Motor: Stop");
  } else {
    M5.Display.println("Unknown cmd");
  }
}*/


/*M5.Display.setCursor(0, 20);
  M5.Display.println("Forward");
  // Forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  for(int i = 0; i < 5; i++) {
    int spd = 50 + (i * 50); //
    M5.Display.setCursor(0, 20);
    M5.Display.printf("Forward %d", spd);
    ledcWrite(pwmChannelA, spd);
    ledcWrite(pwmChannelB, spd);
    delay(2000); // Wait for 2000ms for each speed
  }

  M5.Display.setCursor(0, 50);
  M5.Display.println("Backward");
  // Backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  for(int i = 0; i < 5; i++) {
    int spd = 50 + (i * 50); //
    M5.Display.setCursor(0, 50);
    M5.Display.printf("Backward %d", spd);
    ledcWrite(pwmChannelA, spd);
    ledcWrite(pwmChannelB, spd);
    delay(2000); // Wait for 2000ms for each speed
  }

  // Servo Test
  M5.Display.println("Servo test...");
  for (int angle = 0; angle <= 180; angle += 10) {
    myServo.write(angle);
    delay(100);
  }
  for (int angle = 180; angle >= 0; angle -= 10) {
    myServo.write(angle);
    delay(100);
  }

  M5.Display.clear();
  M5.Display.setCursor(0, 0);*/

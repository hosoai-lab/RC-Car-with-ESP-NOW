#include <M5AtomS3.h>

void setup() {
  M5.begin();
  Serial.begin(115200);

  ledcAttach(2, 5000, 8);  // G2 OK
  ledcWrite(2, 180);       // duty 180/255
  M5.Display.println("PWM OK on G2");

  ledcAttach(5, 5000, 8);  // G2 OK
  ledcWrite(5, 180);       // duty 180/255
  M5.Display.println("PWM OK on G5");
}

void loop() {}

/*#include <M5AtomS3.h>
#include <ESP32Servo.h>

Servo myServo;

// Motor 1 (Left or Front)
const int motor1In1 = 7;   // G7
const int motor1In2 = 6;   // G6
const int motor1PWM = 8;   // G8 (จริง ๆ คือ GPIO8)

// Motor 2 (Right or Rear)
const int motor2In1 = 5;   // G5
const int motor2In2 = 38;  // G38
const int motor2PWM = 39;  // G39

int speed = 150; // กำหนดความเร็ว 0-255

void setup() {
  M5.begin();
  Serial.begin(115200);
  M5.Display.begin();
  M5.Display.setTextSize(2);
  M5.Display.setCursor(0, 0);
  M5.Display.println("Starting test...2nd ver");

  // Servo
  myServo.attach(2); // G2

  // Motor direction pins
  pinMode(motor1In1, OUTPUT);
  pinMode(motor1In2, OUTPUT);
  pinMode(motor2In1, OUTPUT);
  pinMode(motor2In2, OUTPUT);

  // Attach PWM with new LEDC API (ESP32 3.x)
  ledcAttach(motor1PWM, 5000, 8); // freq 5kHz, resolution 8 bit
  ledcAttach(motor2PWM, 5000, 8);
}

void loop() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);

  // ทดสอบ Servo
  M5.Display.println("Servo test...");
  for (int angle = 0; angle <= 180; angle += 10) {
    myServo.write(angle);
    delay(100);
  }
  for (int angle = 180; angle >= 0; angle -= 10) {
    myServo.write(angle);
    delay(100);
  }

  // ทดสอบ มอเตอร์ เดินหน้า
  M5.Display.println("Forward...");
  moveMotor("F", speed);
  delay(2000);

  // ทดสอบ มอเตอร์ ถอยหลัง
  M5.Display.println("Backward...");
  moveMotor("B", speed);
  delay(2000);

  // หยุด
  M5.Display.println("Stop");
  moveMotor("S", 0);
  delay(1000);
}

void moveMotor(String dir, int spd) {
  if (dir == "F") {
    // Forward
    digitalWrite(motor1In1, HIGH);
    digitalWrite(motor1In2, LOW);
    digitalWrite(motor2In1, HIGH);
    digitalWrite(motor2In2, LOW);
  } else if (dir == "B") {
    // Backward
    digitalWrite(motor1In1, LOW);
    digitalWrite(motor1In2, HIGH);
    digitalWrite(motor2In1, LOW);
    digitalWrite(motor2In2, HIGH);
  } else {
    // Stop
    digitalWrite(motor1In1, LOW);
    digitalWrite(motor1In2, LOW);
    digitalWrite(motor2In1, LOW);
    digitalWrite(motor2In2, LOW);
  }

  // ส่ง PWM ด้วย pin จริง (ไม่ใช่ channel)
  ledcWrite(motor1PWM, spd);
  ledcWrite(motor2PWM, spd);
}*/

/*#include <M5AtomS3.h>
#include <ESP32Servo.h>
#include <M5Unified.h>

Servo myServo;

// Motor 1 (Left or Front)
int motor1Pin1 = 7; // G7
int motor1Pin2 = 6; // G6
int motor1PWM  = 8; // G8

// Motor 2 (Right or Rear)
int motor2Pin1 = 5;  // G5
int motor2Pin2 = 38; // G38
int motor2PWM  = 39; // G39

int spd = 150; // speed range: 0–255

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
  M5.Display.println("Ready!");
}

void loop() {
  // Test servo sweep
  for (int angle = 0; angle <= 180; angle += 10) {
    myServo.write(angle);
    M5.Display.setCursor(0, 20);
    M5.Display.printf("Servo: %d  \n", angle);
    delay(200);
  }
  for (int angle = 180; angle >= 0; angle -= 10) {
    myServo.write(angle);
    M5.Display.setCursor(0, 20);
    M5.Display.printf("Servo: %d  \n", angle);
    delay(200);
  }

  // Move Forward
  moveMotor("F", spd);
  M5.Display.setCursor(0, 40);
  M5.Display.print("Motor: Forward ");
  delay(2000);

  // Move Backward
  moveMotor("B", spd);
  M5.Display.setCursor(0, 40);
  M5.Display.print("Motor: Backward");
  delay(2000);

  // Stop
  moveMotor("S", 0);
  M5.Display.setCursor(0, 40);
  M5.Display.print("Motor: Stop    ");
  delay(2000);
}*/
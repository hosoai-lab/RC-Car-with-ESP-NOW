// ===== Sender Code (Serial -> ESP-NOW) =====
#include <M5AtomS3.h>
#include <WiFi.h>
#include <esp_now.h>
#include <M5Unified.h>

uint8_t receiverMac[] = {0x24, 0x58, 0x7C, 0x5C, 0x81, 0xA4};  // <-- Replace with actual MAC

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.print(status == ESP_NOW_SEND_SUCCESS ? "Data Sent!" : "Send Failed!");
}

void setup() {
  M5.begin();
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);
  M5.Display.setCursor(0, 0);
  M5.Display.print("Sender Ready..");
}

void loop() {
  /*if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    char msg[32];
    command.toCharArray(msg, sizeof(msg));
    esp_now_send(receiverMac, (uint8_t *)msg, strlen(msg) + 1);
    M5.Display.clear();
    M5.Display.setCursor(0, 0);
    M5.Display.print(msg);
  }*/
  
  static char buffer[32];
  static unsigned long lastSendTime = 0;

  if (Serial.available()) {
    size_t len = Serial.readBytesUntil('\n', buffer, sizeof(buffer) - 1);
    buffer[len] = '\0';  // Null-terminate

    esp_now_send(receiverMac, (uint8_t *)buffer, strlen(buffer) + 1);

    M5.Display.clear();
    M5.Display.setCursor(0, 0);
    M5.Display.printf("Sent: %s\n", buffer);

    lastSendTime = millis();
  }
 
}
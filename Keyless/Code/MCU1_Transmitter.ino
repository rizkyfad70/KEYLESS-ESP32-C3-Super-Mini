//ESP-NOW Protocol MCU1 
#include <esp_now.h>
#include <WiFi.h>

#define BUTTON_PIN 4
#define LED_PIN 8
bool toggleState = false;

uint8_t receiverMAC[] = {0x18, 0x8B, 0x0E, 0x90, 0xF0, 0x44}; // Ganti dengan MAC ESP MCU2

String key = "312";

typedef struct struct_message {
  char msg[32];
} struct_message;

struct_message outgoingMessage;

String encrypt(String input, String key) {
  int col = key.length();
  while (input.length() % col != 0) input += "X";
  int row = input.length() / col;

  char grid[row][col];
  int k = 0;
  for (int i = 0; i < row; i++)
    for (int j = 0; j < col; j++)
      grid[i][j] = input[k++];

  String output = "";
  for (int i = 0; i < col; i++) {
    int pos = key.indexOf('1' + i);
    for (int j = 0; j < row; j++) {
      output += grid[j][pos];
    }
  }
  return output;
}

void OnSend(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Status kirim pesan: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "GAGAL" : "SUKSES");
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init gagal!");
    return;
  }

  esp_now_register_send_cb(OnSend);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) == ESP_OK) {
    Serial.println("BLE ditambahkan.");
  } else {
    Serial.println("Gagal menambahkan peer.");
  }

  Serial.println("MCU1 siap.");
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(200); // debounce
    toggleState = !toggleState;

    String plainText = toggleState ? "UNLOCK" : "LOCKED";
    String encrypted = encrypt(plainText, key);
    encrypted.toCharArray(outgoingMessage.msg, 32);

    Serial.print("Tombol ditekan, pesan: ");
    Serial.println(plainText);
    Serial.print("Terenkripsi: ");
    Serial.println(encrypted);

    // Kirim data melalui ESP-NOW
    esp_now_send(receiverMAC, (uint8_t *)&outgoingMessage, sizeof(outgoingMessage));

    // Atur LED berdasarkan status
    digitalWrite(LED_PIN, toggleState ? HIGH : LOW);

    delay(500); // tunggu sebelum membaca tombol lagi
  }
}

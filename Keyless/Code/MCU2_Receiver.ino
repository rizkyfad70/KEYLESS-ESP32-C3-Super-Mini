//MCU2
#include <esp_now.h>
#include <WiFi.h>

#define BUZZER_PIN 3      
#define LED_PIN 2    
#define LED_OUT 4     
#define RELAY_1PIN 5  
#define RELAY_2PIN  6  

String key = "312";

typedef struct struct_message {
  char msg[32];
} struct_message;

struct_message incomingMessage;

String decrypt(String input, String key) {
  int col = key.length();
  int row = input.length() / col;
  char grid[row][col];
  int order[col];

  for (int i = 0; i < col; i++) {
    order[i] = key.indexOf('1' + i);
  }

  int k = 0;
  for (int i = 0; i < col; i++) {
    for (int j = 0; j < row; j++) {
      grid[j][order[i]] = input[k++];
    }
  }

  String output = "";
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      output += grid[i][j];
    }
  }

  return output;
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));
  String received = String(incomingMessage.msg);
  Serial.print("Terenkripsi diterima: ");
  Serial.println(received);

  String decrypted = decrypt(received, key);
  Serial.print("Setelah dekripsi: ");
  Serial.println(decrypted);

  if (decrypted == "UNLOCK") {
    digitalWrite(LED_PIN, HIGH);       // Nyalakan LED
    digitalWrite(LED_OUT, HIGH);
    digitalWrite(RELAY_1PIN, LOW);     // Aktifkan relay
    digitalWrite(RELAY_2PIN, LOW);
    digitalWrite(BUZZER_PIN, HIGH);    // Bunyi buzzer sekali
    delay(300);                        // Bunyi selama 500 ms
    digitalWrite(BUZZER_PIN, LOW);     // Matikan buzzer setelahnya
    delay(500);
    digitalWrite(BUZZER_PIN, HIGH);    // Bunyi buzzer sekali
    delay(400);                        // Bunyi selama 500 ms
    digitalWrite(BUZZER_PIN, LOW);     // Matikan buzzer setelahnya   
    Serial.println("🔓 Relay ON");
  } else if (decrypted == "LOCKED") {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(LED_OUT, LOW);        
    digitalWrite(RELAY_1PIN, HIGH);    
    digitalWrite(RELAY_2PIN, HIGH);  
    digitalWrite(BUZZER_PIN, HIGH);    
    delay(500);                        
    digitalWrite(BUZZER_PIN, LOW);   
    Serial.println("🔒 Relay OFF");
  } else {
    Serial.println("⚠️ Pesan tidak valid.");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_OUT, OUTPUT);
  pinMode(RELAY_1PIN, OUTPUT);
  pinMode(RELAY_2PIN, OUTPUT);

  digitalWrite(LED_OUT, LOW);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(RELAY_1PIN, HIGH);
  digitalWrite(RELAY_2PIN, HIGH);
    
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init gagal!");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("MCU2 siap menerima data.");
}

void loop() {
  // kosong, semua diinterupsi ESP-NOW
} 

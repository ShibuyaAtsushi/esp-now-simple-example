// #include <M5Unified.h>
// #include <WiFi.h>
// #include <esp_now.h>
// int count = 0;


// void onReceive(const uint8_t *mac, const uint8_t *incomingData, int len) { //*macは送信元のMACアドレス、incomingDataは送信されたデータ、lenはデータの長さ
//   if (incomingData[0] == 1) {
//     M5.Lcd.fillScreen(BLACK);
//     M5.Lcd.setCursor(0, 0);
//     M5.Lcd.setTextSize(4);
//     M5.Lcd.setTextColor(WHITE);
//     M5.Lcd.println("Push");
//     count = count+1;

//   }
// }

// void setup() {
//   Serial.begin(115200);
//   M5.begin();
//   M5.Lcd.fillScreen(BLACK);
//   M5.Lcd.setCursor(0, 0);
//   M5.Lcd.setTextSize(3);
//   M5.Lcd.setTextColor(WHITE);
//   WiFi.mode(WIFI_STA);
//   WiFi.disconnect();

//   if (esp_now_init() == ESP_OK) {
//     Serial.println("ESPNow Init Success");
//     M5.Lcd.println("ESPNow Init Success!");
//   } else {
//     Serial.println("ESPNow Init Failed");
//     delay(3000);
//     ESP.restart();
//   }

//   esp_now_register_recv_cb(onReceive);
// }

// void loop() {
//   // Serial.println("ESPNow Init Success");
//   M5.Lcd.print(count);
//   delay(1000);
// }


#include <M5Unified.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

int count = 0;

void onReceive(const uint8_t *mac, const uint8_t *incomingData, int len) {
  count++;
  Serial.print("Received data: ");
  for (int i = 0; i < len; i++) {
    Serial.print(incomingData[i]);
    Serial.print(" ");
  }
  Serial.println();
  
  if (incomingData[0] == 1) {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextSize(4);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.println("Push");
    count++;
  }
}

void setup() {
  Serial.begin(115200);
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(WHITE);
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  esp_wifi_set_channel(8, WIFI_SECOND_CHAN_NONE);//チャンネルを設定！！！超大事！！！

  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
    M5.Lcd.println("ESPNow Init Success!");
  } else {
    Serial.println("ESPNow Init Failed");
    delay(3000);
    ESP.restart();
  }

  esp_now_register_recv_cb(onReceive);
}

void loop() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.printf("Count: %d", count);
  delay(1000);
}



///////////////////////////////////////////////////////////////
////////////////////MACアドレス確認/////////////////////////////
///////////////////////////////////////////////////////////////
// #include <M5Unified.h>
// #include <WiFi.h>

// void setup() {
//   M5.begin();
//   Serial.begin(115200);
//   WiFi.mode(WIFI_STA);
//   WiFi.disconnect();
//   delay(100);
  

//   // MACアドレスを取得
//   uint8_t mac[6];
//   WiFi.macAddress(mac);
//   Serial.print("MAC Address: ");
//   for (int i = 0; i < 6; i++) {
//     Serial.printf("%02X", mac[i]);
//     if (i < 5) Serial.print(":");
//   }
//   Serial.println();
// }

// void loop() {
//   // 何もしない
// }
///////////////////////////////////////////////////////////////
////////////////////MACアドレス確認/////////////////////////////
///////////////////////////////////////////////////////////////

#include <M5AtomS3.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

// M5Core2のMACアドレスを設定
uint8_t receiverAddress[] = {0x3C, 0x61, 0x05, 0x0D, 0xB2, 0x54}; //  3C:61:05:0D:B2:54
uint8_t data = 0;

void setup() {
  
  USBSerial.begin(115200);
  AtomS3.begin(true);  // Init M5AtomS3Lite.
  M5.begin();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  

  if (esp_now_init() == ESP_OK) {
    USBSerial.println("ESPNow Init Success");
  } else {
    USBSerial.println("ESPNow Init Failed");
    delay(3000);
    ESP.restart();
    
  }
  

  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo)); // 構造体を初期化する
  memcpy(peerInfo.peer_addr, receiverAddress, 6); // MACアドレスを構造体にコピー（設定）する
  peerInfo.channel = 8; // チャンネルを設定 これとはべつに、WiFiのチャンネルも設定しないとエラーが出てしまいます
  peerInfo.encrypt = false; // 暗号化を無効
  esp_wifi_set_channel(8, WIFI_SECOND_CHAN_NONE); //WiFiのチャンネル設定


  if (esp_now_add_peer(&peerInfo) != ESP_OK) { //espnowのピアリストに追加する。成功するとESP_OKが返る
    USBSerial.println("Failed to add peer"); //失敗した場合の処理
    return;
  }
}

void loop() {
  M5.update();
  AtomS3.update();
  if (M5.BtnA.isPressed()) {
    data = 1; // ボタンが押されたことを示すデータ
    esp_now_send(receiverAddress, &data, sizeof(data));
    USBSerial.println("Button pressed, data sent:");
    USBSerial.println(data);
    delay(500);
  }
}





/**
 * @file Led.ino
 * @author SeanKwok (shaoxiang@m5stack.com)
 * @brief M5AtomS3 Lite LED Test
 * @version 0.1
 * @date 2023-12-13
 *
 *
 * @Hardwares: M5AtomS3
 * @Platform Version: Arduino M5Stack Board Manager v2.0.9
 * @Dependent Library:
 * M5GFX: https://github.com/m5stack/M5GFX
 * M5Unified: https://github.com/m5stack/M5Unified
 * M5AtomS3: https://github.com/m5stack/M5AtomS3
 * FastLED: https://github.com/FastLED/FastLED
 */


///////LED TEST/////////////////////////////////////////
// #include <M5AtomS3.h>

// void setup() {
//     AtomS3.begin(true);  // Init M5AtomS3Lite.
//     AtomS3.dis.setBrightness(100);
//     USBSerial.begin(115200);
// }

// void loop() {
//     // AtomS3.dis.drawpix(0xff0000);
//     // AtomS3.update();
//     // delay(50);
    
//     // AtomS3.dis.drawpix(0x0000ff);
//     // AtomS3.update();
//     // delay(50);
    
//     //  M5.update();
//   while (M5.BtnA.wasPressed()) {    //wasplessedは、ボタンが押された瞬間だけtrueを返す
//     USBSerial.println("Button pressed, data sent");
//     AtomS3.dis.drawpix(0x00ff00);
//     AtomS3.update();
//     delay(500);
//     // M5.update();
    
//     AtomS3.dis.drawpix(0x0000ff);
//     AtomS3.update();
//     delay(500);
//   }
//   AtomS3.dis.drawpix(0xff0000);
//     AtomS3.update();
// }
///////LED TEST/////////////////////////////////////////




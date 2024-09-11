//必要なものはこれだけ！
#include <M5unified.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

// 送信側・受信側で共通の構造体を定義する
typedef struct PacketData {
  uint8_t sensor1;
  uint8_t sensor2;
  uint8_t sensor3;
  uint8_t button_state;
  float accX;
  float accY;
  float accZ;
  float gyroX;
  float gyroY;
  float gyroZ;
  float temp;
} PacketData;

PacketData sendData; //送信データを格納するための構造体変数を作成

// 送信先のMACアドレスを設定 各自でMACアドレスを確認して、それを入れる
uint8_t receiverAddress[] = {0x3C, 0x61, 0x05, 0x0D, 0xB2, 0x54}; //  3C:61:05:0D:B2:54

  //このプログラム上で使う変数
  float accX;
  float accY;
  float accZ;
  float gyroX;
  float gyroY;
  float gyroZ;
  float temp;

void setup() {
  
  // USBSerial.begin(115200);   //M5AtomS3などで、Serial.beginが使えなかったらUSBSerial.beginを使うとうまくいく
  // Serial.begin(115200);   //シリアル通信する場合

  M5.begin(); // M5Stackを使う場合はこれを使う

  //ESP_NOWを使うにはこれが必要
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  //M5stackの画面に文字を表示させるための設定 M5を使わないなら不要
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextSize(4);
  M5.Lcd.setTextColor(WHITE);

  // ESP-NOWの初期化が成功していなければリスタート
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success (^_^)");
    M5.Lcd.println("ESPNow Init Success (^_^)");
  } else {
    Serial.println("ESPNow Init Failed (-_-)");
    M5.Lcd.println("ESPNow Init Failed (-_-)");
    delay(3000);
    ESP.restart();
  }
  
  // ESPNOWをするための一連の設定　MACアドレスの設定や使用する周波数帯（チャンネル）の設定
  esp_now_peer_info_t peerInfo; //peerInfoという構造体を作成 ESPNOWするために必要な設定が書かれた構造体
  memset(&peerInfo, 0, sizeof(peerInfo)); // 構造体を初期化する
  memcpy(peerInfo.peer_addr, receiverAddress, 6); // （peerInfo.peer_addrに、receiverAddressをコピーする。サイズは６。の意味）MACアドレスをpeer_addrという構造体メンバにコピー（設定）する。これで送信先のMACアドレスが設定される
  peerInfo.channel = 8; // チャンネルを設定 !!!これとはべつに、WiFiのチャンネルも設定しないとエラーが出ます!!!
  peerInfo.encrypt = false; // 暗号化を無効
  esp_wifi_set_channel(8, WIFI_SECOND_CHAN_NONE); //WiFiのチャンネル設定 !!!peerinfo.channelと合わせる!!!


  if (esp_now_add_peer(&peerInfo) != ESP_OK) { //「esp_now_add_peer」で、espnowのピアリストにpeerInfo構造体に書かれた設定を追加する。成功するとESP_OKが返るので、それがうまくいかなかったときの処理が書いてある
    Serial.println("Failed to add peer (;_;)");
    M5.Lcd.println("Failed to add peer (;_;)");
    return;
  }
}

void loop() {
  M5.update(); //M5の様々なセンサ値とかをここで取得（更新）している。
  // AtomS3.update();
  M5.Imu.getAccel(&accX, &accY, &accZ);// 加速度データの取得 m5stackに搭載されているIMUからデータを取得するための関数を使っている
  M5.Imu.getGyro(&gyroX, &gyroY, &gyroZ);// ジャイロデータの取得　これらgetAccelなどの関数は引数にポインタを渡して、そのポインタが指し示す変数にデータを格納するという使い方をするものだからこのように書いているだけ
  M5.Imu.getTemp(&temp);// 温度データの取得

  sendData.gyroX = gyroX; // 送信するための構造体変数sendDataに、取得した角速度データを格納　このように格納していって、最後にesp_now_sendで送信するだけ！
  sendData.gyroY = gyroY;
  sendData.gyroZ = gyroZ;
  sendData.button_state = 0;
  esp_now_send(receiverAddress, (uint8_t*)&sendData, sizeof(sendData)); // 受け取り側のMACアドレス（receiverAddress）と、送信するデータの（先頭の？）アドレス(sendData)、データのサイズを指定して送信する
}
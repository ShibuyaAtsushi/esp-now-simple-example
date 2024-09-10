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

PacketData packetdata1 = {1, 2, 3, 99};
PacketData receivedData = {1, 2, 3, 99, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

int count = 0;
float flag = 0;
int length = 0;

void onReceive(const uint8_t *mac, const uint8_t *incomingData, int len) { //*macは送信元のMACアドレス、incomingDataは送信されたデータ、lenはデータの長さ。この関数を、後でsetupで、データ受信時に自動で呼び出されるコールバック関数として登録することで機能する。
  //if (len == sizeof(receivedData)) {  // 受信データはPacketData型の構造体を想定しているので、そのサイズと一致するか確認
    // PacketData receivedData;  // 受信データを格納するための構造体のインスタンス（構造体変数）を作成
    // 受信したデータを構造体にコピー
    memcpy(&receivedData, incomingData, sizeof(PacketData)); //受信したデータを構造体にコピーする。第一引数にコピー先の構造体変数のアドレス、第二引数にコピー元のデータの!アドレス!、第三引数にコピーするデータのサイズを指定する。
    //↑構造体変数receivedDataのアドレスに、アドレスであるincomingdataが指し示すデータを、指定のサイズ分コピーする。これでincomingdataが指しているデータの実体をreceiveddataにコピーすることができた。
    //構造体変数receivedDataに、受信したデータが格納された。以降これをつかう。
    // Serial.print("ジュシンシマシタ：");
    flag = 1;
    count = count+1;
    length = len;
    // M5.Lcd.print("sensor1:");
    // M5.Lcd.print(receivedData.sensor1);
    // M5.Lcd.print("sensor2:");
    // M5.Lcd.print(receivedData.sensor2);
    // M5.Lcd.print("sensor3:");
    // M5.Lcd.print(receivedData.sensor3);
    // M5.Lcd.print("button_state:");
    // M5.Lcd.print(receivedData.button_state);
  //}
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

  esp_wifi_set_channel(8, WIFI_SECOND_CHAN_NONE);//ESPNOWで使用するWiFiのチャンネルを設定する。送信側の「peerInfo.channel = 8;」と合わせる必要がある（８は例）。（近いチャンネル（８だったら６や７）だと混信する可能性あり。）

  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
    M5.Lcd.println("ESPNow Init Success!");
  } else {
    Serial.println("ESPNow Init Failed");
    delay(3000);
    ESP.restart();
  }

  esp_now_register_recv_cb(onReceive); //受信したデータを処理する関数を登録する。これで、データを受信したときをトリガーに、割り込み関数のonReceive関数が呼ばれるようになる。ただ単にデータ受信時に呼び出されるコールバック関数を登録するのではなく、onReceiveに引数と同じの関数を登録するものであるので、引数は（macアドレス, 受診したデータを格納しているアドレスを代入するポインタ, 受診したデータの長さ（何バイトか））でなければならない。
}

void loop() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(WHITE, BLACK);
  while (1) {
  if (flag == 1) {
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("sensor1:");
    M5.Lcd.println(receivedData.accX);
    M5.Lcd.println("sensor2:");
    M5.Lcd.println(receivedData.accY);
    M5.Lcd.println("sensor3:");
    M5.Lcd.println(receivedData.accZ);
    M5.Lcd.println("button_state:");
    M5.Lcd.println(receivedData.button_state);
    // flag = 0;
  }
  if (flag == 0) {
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("Data Waiting...");
  }
  // M5.Lcd.printf("length: %d", length);
  // M5.Lcd.printf("sizeof(receivedData): %d", sizeof(receivedData));
  //受信時はフラグだけとか、アドレスだけとかの処理にする。多分今は重いんだと思う
}}



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

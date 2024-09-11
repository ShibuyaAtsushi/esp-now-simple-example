//必要なものはこれだけ！
#include <M5Unified.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

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

// 受信データを格納するための構造体変数を作成
PacketData receivedData; 

//受信時の割り込み関数を定義。ここはこのままで良い。（*macには送信元のMACアドレス、incomingDataには送信されたデータ、lenにはそのデータの長さzの値が勝手に入る。この関数を、後でsetup関数内でデータ受信時に自動で呼び出される割り込み関数として登録することで機能する。）
void onReceive(const uint8_t *mac, const uint8_t *incomingData, int len) { 
  if (len == sizeof(receivedData)) {  // 受信したデータはPacketData型の構造体を想定しているので、そのサイズと一致しているか念の為確認。
    memcpy(&receivedData, incomingData, sizeof(PacketData)); //受信したデータを構造体にコピーする。第一引数にコピー先の構造体変数のアドレス、第二引数にコピー元のデータのアドレス、第三引数にコピーするデータのサイズを指定する。
    //↑構造体変数receivedDataのアドレスに、受け取ったデータが格納されているアドレスが入っているポインタ「incomingdata」が指し示すデータを、指定のサイズ分コピーする。これでincomingdataが指しているデータの実体をreceivedDataにコピーすることができる。
    //構造体変数receivedDataに、受信したデータが格納された。以降これをつかう。
    //incomingDataはポインタである。
}
}

void setup() {
  // Serial.begin(115200);
  //M5stackを使う場合
  M5.begin();

  //ESP_NOWを使うにはこれが必要
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  esp_wifi_set_channel(8, WIFI_SECOND_CHAN_NONE);//ESPNOWで使用するWiFiのチャンネルを設定する。送信側の「peerInfo.channel = 8;」と合わせる必要がある（８は例）。（近いチャンネル（８だったら６や７）だと混信する可能性あり。）12チャンネルまで選べる

  if (esp_now_init() == ESP_OK) { //初期化がうまくいかなかったらリスタート。ここもそのままで良い。
    Serial.println("ESPNow Init Success!");
    M5.Lcd.println("ESPNow Init Success!");
  } else {
    Serial.println("ESPNow Init Failed!");
    M5.Lcd.println("ESPNow Init Failed!");
    delay(3000);
    ESP.restart();
  }

  esp_now_register_recv_cb(onReceive); //データ受信時の割り込み関数を登録する。これで、データを受信したときをトリガーに、割り込み関数のonReceive関数が呼ばれるようになる。ただ単にデータ受信時に呼び出されるコールバック関数を登録するのが「esp_now_register_recv_cb」の行うことではなく、上で定義したonReceiveと同じ引数をもつ関数を登録するものであるので、引数は（macアドレス, 受診したデータを格納しているアドレスを代入するポインタ, 受診したデータの長さ（何バイトか））でなければ登録がうまくいかない。

  //任意　ディスプレイつきM5製品の画面に文字を表示させるための設定
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(WHITE, BLACK);
}

void loop() {
  M5.Lcd.setCursor(0, 0); //M5の画面に表示される文字が毎回改行されて見えなくなるのを防ぐために、カーソルを毎回先頭に戻すことで同じ場所に上書き表示されるようにする
  M5.Lcd.println("Gyro_X:");
  M5.Lcd.println(receivedData.gyroX); //構造体変数receivedDataに、受け取ったデータをそのままコピーしたので、receivedData.sensor1のようにして構造体のメンバにアクセスすれば送信先と同じ変数名でそのまま使える！！
  M5.Lcd.println("Gyro_Y:");
  M5.Lcd.println(receivedData.gyroY);
  M5.Lcd.println("Gyro_Z:");
  M5.Lcd.println(receivedData.gyroZ);
  M5.Lcd.println("button_state:");
  M5.Lcd.println(receivedData.button_state);
}




///////////////////////////////////////////////////////////////
////////////////////MACアドレス確認用/////////////////////////////
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
////////////////////MACアドレス確認用/////////////////////////////
///////////////////////////////////////////////////////////////

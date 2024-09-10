#include <M5unified.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

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


// M5Core2のMACアドレスを設定
uint8_t receiverAddress[] = {0x3C, 0x61, 0x05, 0x0D, 0xB2, 0x54}; //  3C:61:05:0D:B2:54
uint8_t data = 0;
PacketData packetdata1 = {1, 2, 3, 99};
float accX = 0.0, accY = 0.0, accZ = 0.0;  // 加速度の値　すべての変数を0.0で初期化
float gyroX = 0.0, gyroY = 0.0, gyroZ = 0.0;  // ジャイロの値
float temp;  // 温度の値

void setup() {
  
  // USBSerial.begin(115200);   //Serial.beginが使えなかったら、USBSerial.beginを使うとうまくいく
  Serial.begin(115200);
  M5.begin();  // Initialize the M5Stack object
  WiFi.mode(WIFI_STA); // WiFiをSTAモードに設定
  WiFi.disconnect(); // 一旦WiFiの接続を切断　これで使うための準備が整う
  // IMUの初期化が成功しているかを確認
  if (!M5.Imu.isEnabled()) {
    Serial.println("IMU Initialization failed!");
    while (1);
  }

  Serial.println("IMU Initialized successfully");
  

  if (esp_now_init() == ESP_OK) {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextSize(4);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.println("ESPNow Init Success (^_^)");
  } else {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextSize(4);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.println("ESPNow Init Failed (-_-)");
    delay(3000);
    ESP.restart();
    
  }
  

  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo)); // 構造体を初期化する これをしないとうまく送受信できなかった
  memcpy(peerInfo.peer_addr, receiverAddress, 6); // （peerInfo.peer_addrに、receiverAddressをコピーする。サイズは６。の意味）MACアドレスをpeer_addrという構造体メンバにコピー（設定）する。これで送信先のMACアドレスが設定される
  peerInfo.channel = 8; // チャンネルを設定 !!!これとはべつに、WiFiのチャンネルも設定しないとエラーが出てしまいます!!!
  peerInfo.encrypt = false; // 暗号化を無効
  esp_wifi_set_channel(8, WIFI_SECOND_CHAN_NONE); //WiFiのチャンネル設定 !!!peerinfo.channelと合わせる!!!


  if (esp_now_add_peer(&peerInfo) != ESP_OK) { //espnowのピアリストに追加する。成功するとESP_OKが返る
    // Serial.println("Failed to add peer"); //失敗した場合の処理
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextSize(4);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.println("Failed to add peer (;_;)"); //失敗した場合の処理
    return;
  }
}

void loop() {
  M5.update(); //すべてのセンサ値とかをここで取得（更新）している。
  // AtomS3.update();
  M5.Imu.getAccel(&accX, &accY, &accZ);// 加速度データの取得
  M5.Imu.getGyro(&gyroX, &gyroY, &gyroZ);// ジャイロデータの取得
  M5.Imu.getTemp(&temp);// 温度データの取得
  if (M5.BtnA.wasPressed()) {
    data = 1; // ボタンが押されたことを示すデータ
    packetdata1.sensor1 = 1;
    packetdata1.sensor2 = 2;
    packetdata1.sensor3 = 3;
    packetdata1.button_state = 99;
    esp_now_send(receiverAddress, (uint8_t*)&packetdata1, sizeof(packetdata1)); //receiverAddress(送信先macアドレス)の機器に、packetdata1のアドレスが示すデータを送信する。サイズはsizeofで自動で取得し、packetdata1のアドレスの先頭からこの数値分のバイト数だけデータを送る。本来ならpacketdata1を示す型はPacketdataだけど、それだと構造体全体を1つの塊として扱うため、バイトごとに処理できないから、１バイトごとに読み込んでもらうためにuint8_t型にキャスト（型変換）している。構造体のアドレスを,「バイトの並びが格納された場所」として扱うためにキャストが必要。何バイトかで１つの構造体がまとまっているので、それを1バイトずつに細かく分解して、それを1バイトずつESPNOWで送る。
    // esp_err_tをつけると、その関数の実行結果に応じて、ESP_OKとかESP_FAILとかが返ってくるからIF分とか書きやすくなる
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextSize(3);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.println("button A was pressed, data sent:");
    M5.Lcd.println("sensor1:");
    M5.Lcd.println(packetdata1.sensor1);
    M5.Lcd.println("sensor2:");
    M5.Lcd.println(packetdata1.sensor2);
    M5.Lcd.println("sensor3:");
    M5.Lcd.println(packetdata1.sensor3);
    M5.Lcd.println("button_state:");
    M5.Lcd.println(packetdata1.button_state);
  }
  if (M5.BtnB.wasPressed()) {
    data = 1; // ボタンが押されたことを示すデータ
    packetdata1.sensor1 = 11;
    packetdata1.sensor2 = 22;
    packetdata1.sensor3 = 33;
    packetdata1.button_state = 99;
    esp_now_send(receiverAddress, (uint8_t*)&packetdata1, sizeof(packetdata1));
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextSize(3);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.println("button A was pressed, data sent:");
    M5.Lcd.println("sensor1:");
    M5.Lcd.println(packetdata1.sensor1);
    M5.Lcd.println("sensor2:");
    M5.Lcd.println(packetdata1.sensor2);
    M5.Lcd.println("sensor3:");
    M5.Lcd.println(packetdata1.sensor3);
    M5.Lcd.println("button_state:");
    M5.Lcd.println(packetdata1.button_state);
  }

    packetdata1.accX = accX;
    packetdata1.accY = accY;
    packetdata1.accZ = accZ;
    packetdata1.button_state = 0;
    esp_now_send(receiverAddress, (uint8_t*)&packetdata1, sizeof(packetdata1));
}


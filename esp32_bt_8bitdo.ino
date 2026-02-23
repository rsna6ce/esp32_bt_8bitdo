#include <Bluepad32.h>

// グローバル配列でコントローラを管理（最大4台）
ControllerPtr myControllers[BP32_MAX_GAMEPADS] = {nullptr};

#define LED_PIN 2

void onConnectedController(ControllerPtr ctl) {
  bool found = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      myControllers[i] = ctl;
      found = true;
      break;
    }
  }
  if (found) {
    Serial.println("=== コントローラ接続されました！ ===");
    Serial.print("モデル: ");
    Serial.println(ctl->getModelName());

    // MACアドレス出力（配列で取得）
    const uint8_t* addr = ctl->getProperties().btaddr;
    Serial.printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                  addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
  } else {
    Serial.println("接続スロット満杯！");
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      myControllers[i] = nullptr;
      break;
    }
  }
  Serial.println("=== コントローラ切断されました！ ===");
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  delay(1000);
  Serial.println("Bluepad32 v4.1.0 基本動作テスト開始");

  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.enableNewBluetoothConnections(true);
  Serial.println("スキャン開始... 8BitDo SN30 Pro を Y + Start 長押しで起動");
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  BP32.update();

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] != nullptr && myControllers[i]->isConnected()) {
      ControllerPtr ctl = myControllers[i];

      uint16_t buttons = ctl->buttons();          // フェイス + ショルダー (A/B/X/Y/L1/R1)
      uint8_t dpad = ctl->dpad();                 // D-Pad (ビットマスク: 1=上, 2=下, 4=左, 8=右)
      uint16_t misc = ctl->miscButtons();         // Misc (SELECT/START/HOME など)
      int l2 = ctl->brake();                     // L2トリガー (0~1023)
      int r2 = ctl->throttle();                     // R2トリガー (0~1023)

      // ジョイスティック値取得
      int lx = ctl->axisX();      // 左スティック X (左右)
      int ly = ctl->axisY();      // 左スティック Y (上下)
      int rx = ctl->axisRX();     // 右スティック X
      int ry = ctl->axisRY();     // 右スティック Y

      String output = "";

      // フェイスボタン (ABXY) → XInputモードで逆転しているのでラベル調整
      if (buttons & 0x0001) output += "B ";       // ログから A押下=0x1 → 実際はBボタン
      if (buttons & 0x0002) output += "A ";       // B押下=0x2 → 実際はAボタン
      if (buttons & 0x0004) output += "Y ";
      if (buttons & 0x0008) output += "X ";

      // D-Pad (十字キー)
      if (dpad & 0x01) output += "↑ ";
      if (dpad & 0x02) output += "↓ ";
      if (dpad & 0x04) output += "→ ";
      if (dpad & 0x08) output += "← ";

      // L1/R1 (ショルダー)
      if (buttons & 0x0010) output += "L1 ";
      if (buttons & 0x0020) output += "R1 ";

      // L2/R2 (アナログトリガー) → 押したら値が増える
      if (l2 > 50) output += "L2(" + String(l2) + ") ";
      if (r2 > 50) output += "R2(" + String(r2) + ") ";

      // Miscボタン (SELECT/START/HOME)
      if (misc & 0x01) output += "HOME ";   // 値は要確認（ログでmisc出力して調整）
      if (misc & 0x02) output += "SELECT ";
      if (misc & 0x04) output += "START ";

      // ジョイスティック（デッドゾーン ±30 以内は無視）
      if (abs(lx) > 30 || abs(ly) > 30) {
        output += "左スティック: X=" + String(lx) + " Y=" + String(ly) + " ";
      }
      if (abs(rx) > 30 || abs(ry) > 30) {
        output += "右スティック: X=" + String(rx) + " Y=" + String(ry) + " ";
}

      //Serial.printf("buttons:0x%04X dpad:0x%02X misc:0x%04X brake(L2?):%d throttle(R2?):%d\n",buttons, dpad, misc, l2, r2);

      if (output != "") {
        Serial.print("押された: ");
        Serial.println(output);
        digitalWrite(LED_PIN, HIGH);
      } else {
        digitalWrite(LED_PIN, LOW);
      }
    }
  }
  delay(50);
}
#include <M5Stack.h>

// モータ制御用PINアサイン
const uint8_t LEFT_MOTOR1 = 3;
const uint8_t LEFT_MOTOR2 = 1;
const uint8_t RIGHT_MOTOR1 = 16;
const uint8_t RIGHT_MOTOR2 = 17;

int status = 0;
String car_status = "Stop";

void setup() {
  // 初期化処理
  M5.begin();
  delay(500);

  // 文字の色とサイズ
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);

  // モータ制御用PINを出力モードに設定
  pinMode(LEFT_MOTOR1, OUTPUT);
  pinMode(LEFT_MOTOR2, OUTPUT);
  pinMode(RIGHT_MOTOR1, OUTPUT);
  pinMode(RIGHT_MOTOR2, OUTPUT);

  // 停止状態からスタート
  digitalWrite(LEFT_MOTOR1, LOW);
  digitalWrite(LEFT_MOTOR2, LOW);
  digitalWrite(RIGHT_MOTOR1, LOW);
  digitalWrite(RIGHT_MOTOR2, LOW);
}

void loop() {
  M5.update();  // ボタン操作の状況を読み込む関数(ボタン操作を行う際は必須)

  // Aボタンが押されたら左旋回
  if(M5.BtnA.wasPressed())
  {
    M5.Lcd.clear(); // 画面全体を消去
    car_status = "Turn Left";

    digitalWrite(LEFT_MOTOR1, HIGH);
    digitalWrite(LEFT_MOTOR2, LOW);
    digitalWrite(RIGHT_MOTOR1, LOW);
    digitalWrite(RIGHT_MOTOR2, HIGH);
  }

  // Bボタンが押されたら 停止⇛前進⇛停止⇛後退
  if(M5.BtnB.wasPressed())
  {
    M5.Lcd.clear(); // 画面全体を消去
    status = (status + 1) % 4;

    switch(status){
      case 0:   // 停止
      case 2:
        car_status = "Stop";
        digitalWrite(LEFT_MOTOR1, LOW);
        digitalWrite(LEFT_MOTOR2, LOW);
        digitalWrite(RIGHT_MOTOR1, LOW);
        digitalWrite(RIGHT_MOTOR2, LOW);
        break;
      case 1:   // 前進
        car_status = "Go Forward";
        digitalWrite(LEFT_MOTOR1, HIGH);
        digitalWrite(LEFT_MOTOR2, LOW);
        digitalWrite(RIGHT_MOTOR1, HIGH);
        digitalWrite(RIGHT_MOTOR2, LOW);
        break;
      case 3:   // 後退
        car_status = "Go BackForward";
        digitalWrite(LEFT_MOTOR1, LOW);
        digitalWrite(LEFT_MOTOR2, HIGH);
        digitalWrite(RIGHT_MOTOR1, LOW);
        digitalWrite(RIGHT_MOTOR2, HIGH);
        break;
      default:  // 停止
        car_status = "Stop";
        digitalWrite(LEFT_MOTOR1, LOW);
        digitalWrite(LEFT_MOTOR2, LOW);
        digitalWrite(RIGHT_MOTOR1, LOW);
        digitalWrite(RIGHT_MOTOR2, LOW);
    }

  }

  // Cボタンが押されたら右旋回
  if(M5.BtnC.wasPressed())
  {
    M5.Lcd.clear(); // 画面全体を消去
    car_status = "Turn Right";
    digitalWrite(LEFT_MOTOR1, LOW);
    digitalWrite(LEFT_MOTOR2, HIGH);
    digitalWrite(RIGHT_MOTOR1, HIGH);
    digitalWrite(RIGHT_MOTOR2, LOW);
  }
  delay(30);

  M5.Lcd.drawString("Car Status=" + car_status, 0, 40); // 信号の状態を表示
}
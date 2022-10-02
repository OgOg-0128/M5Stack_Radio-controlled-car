#include <M5Stack.h>
#include "Wiimote.h"

// Wiiリモコン
Wiimote wiimote;

//Wiiリモコンデータ
uint16_t prev_button = -1;
bool button_down  = false;
bool button_up    = false;
bool button_right = false;
bool button_left  = false;
bool button_plus  = false;
bool button_2     = false;
bool button_1     = false;
bool button_B     = false;
bool button_A     = false;
bool button_minus = false;
bool button_home  = false;

//ヌンチャクデータ
bool button_C     = false;
bool button_Z     = false;
int Nunchuk_sx;
int Nunchuk_sy; 

//バランスボードデータ
float wTopRight;
float wBottomRight;
float wTopLeft;
float wBottomLeft;
float wtotal;
//Wiiハンドル
uint16_t wiihandle;

// モータ制御用PINアサイン
const uint8_t LEFT_MOTOR1 = 2;
const uint8_t LEFT_MOTOR2 = 5;
const uint8_t RIGHT_MOTOR1 = 16;
const uint8_t RIGHT_MOTOR2 = 17;

int status = 0;
String car_status = "Stop";

void setup() {
  // 初期化処理
  Serial.begin(115200);
  Serial.println("\r\nWiimote Start");
  wiimote.init(wiimote_callback);
  M5.begin();
  
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

  M5.Lcd.drawString("Car Status=" + car_status, 0, 40); // 信号の状態を表示
}

void loop() {
  wiimote.handle();  
  M5.update();  // ボタン操作の状況を読み込む関数(ボタン操作を行う際は必須)

  uint16_t c_button = 
    button_up    << 0 | 
    button_down  << 1 | 
    button_left  << 2 | 
    button_right << 3 | 
    button_1     << 4;
  bool button = c_button != 0;

  if(button && prev_button != c_button){
    prev_button = c_button;
    M5.Lcd.clear(BLACK);
  
    if (button_up) {
      Serial.println("up button ON");
      car_status = "Go Forward";
      digitalWrite(LEFT_MOTOR1, HIGH);
      digitalWrite(LEFT_MOTOR2, LOW);
      digitalWrite(RIGHT_MOTOR1, HIGH);
      digitalWrite(RIGHT_MOTOR2, LOW);
    }
    if (button_down) {
      Serial.println("down button ON");

      car_status = "Go BackForward";
      digitalWrite(LEFT_MOTOR1, LOW);
      digitalWrite(LEFT_MOTOR2, HIGH);
      digitalWrite(RIGHT_MOTOR1, LOW);
      digitalWrite(RIGHT_MOTOR2, HIGH);
    }
    if (button_left) {
      Serial.println("left button ON");

      car_status = "Turn Left";
      digitalWrite(LEFT_MOTOR1, HIGH);
      digitalWrite(LEFT_MOTOR2, LOW);
      digitalWrite(RIGHT_MOTOR1, LOW);
      digitalWrite(RIGHT_MOTOR2, HIGH);
    }
    if (button_right) {
      Serial.println("right button ON");

      car_status = "Turn Right";
      digitalWrite(LEFT_MOTOR1, LOW);
      digitalWrite(LEFT_MOTOR2, HIGH);
      digitalWrite(RIGHT_MOTOR1, HIGH);
      digitalWrite(RIGHT_MOTOR2, LOW);
    }

    if (button_1) {
      Serial.println("1 button ON");

      car_status = "Stop";
      digitalWrite(LEFT_MOTOR1, LOW);
      digitalWrite(LEFT_MOTOR2, LOW);
      digitalWrite(RIGHT_MOTOR1, LOW);
      digitalWrite(RIGHT_MOTOR2, LOW);
    }

    M5.Lcd.drawString("Car Status=" + car_status, 0, 40); // 信号の状態を表示
  }
}

void wiimote_callback(wiimote_event_type_t event_type, uint16_t handle, uint8_t *data, size_t len) {
  static int connection_count = 0;
  if(event_type == WIIMOTE_EVENT_DATA){
    wiihandle=handle;
    if(data[1]==0x32){   // http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
      uint8_t* ext = data+4;

      //ヌンチャクデータ取得
      Nunchuk_sx=ext[0];
      Nunchuk_sy=ext[1];
      button_C     = (ext[5] & 0x02) != 0;
      button_Z     = (ext[5] & 0x01) != 0;      

    } else if (data[1]==0x34){ // https://wiibrew.org/wiki/Wii_Balance_Board#Data_Format

      //バランスボードデータ取得
      float weight[4];
      wiimote.get_balance_weight(data, weight);
      wTopRight   =weight[BALANCE_POSITION_TOP_RIGHT];
      wBottomRight=weight[BALANCE_POSITION_BOTTOM_RIGHT];
      wTopLeft    =weight[BALANCE_POSITION_TOP_LEFT];
      wBottomLeft =weight[BALANCE_POSITION_BOTTOM_LEFT];
      wtotal=wTopRight+wTopRight+wTopLeft+wBottomLeft;
      
    }
    //wiiリモコン釦データ取得
    button_down  = (data[2] & 0x01) != 0;
    button_up    = (data[2] & 0x02) != 0;
    button_right = (data[2] & 0x04) != 0;
    button_left  = (data[2] & 0x08) != 0;
    button_plus  = (data[2] & 0x10) != 0;
    button_2     = (data[3] & 0x01) != 0;
    button_1     = (data[3] & 0x02) != 0;
    button_B     = (data[3] & 0x04) != 0;
    button_A     = (data[3] & 0x08) != 0;
    button_minus = (data[3] & 0x10) != 0;
    button_home  = (data[3] & 0x80) != 0;

  }else if(event_type == WIIMOTE_EVENT_INITIALIZE){
    printf("  event_type=WIIMOTE_EVENT_INITIALIZE\n");
    wiimote.scan(true);
  }else if(event_type == WIIMOTE_EVENT_SCAN_START){
    printf("  event_type=WIIMOTE_EVENT_SCAN_START\n");
  }else if(event_type == WIIMOTE_EVENT_SCAN_STOP){
    printf("  event_type=WIIMOTE_EVENT_SCAN_STOP\n");
    if(connection_count==0){
      wiimote.scan(true);
    }
  }else if(event_type == WIIMOTE_EVENT_CONNECT){
    printf("  event_type=WIIMOTE_EVENT_CONNECT\n");
    wiimote.set_led(handle, 1<<connection_count);
    connection_count++;
  }else if(event_type == WIIMOTE_EVENT_DISCONNECT){
    printf("  event_type=WIIMOTE_EVENT_DISCONNECT\n");
    connection_count--;
    wiimote.scan(true);
  }else{
    printf("  event_type=%d\n", event_type);
  }
  delay(100);
}
//Wiiled.ino 2022/03/04

#include "Wiimote.h"
#define LED 2
Wiimote wiimote;
int led=1;

//Wiiリモコンデータ
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

void setup() {
  Serial.begin(115200);
  Serial.println("Wiimote Start");
  //pinMode(LED, OUTPUT);
  wiimote.init(wiimote_callback);
}

void loop() {
  wiimote.handle();
  if (button_A) {
    Serial.println("A button ON");
    /*
    if (digitalRead(LED)) {
      digitalWrite(LED, LOW); 
    } else {
      digitalWrite(LED, HIGH); 
    }
    */
    delay(300);
  }
  if (button_plus || button_minus || button_home) {
    if (button_plus) {
      Serial.println("PLUS button ON");
      led++;if (led>15) led=0;
    }
    if (button_minus) {
      Serial.println("MINUS button ON");
      led--;if (led<0) led=15;
    }
    if (button_home) {
      Serial.println("HOME button ON");
      led=0;
    }
    wiimote.set_led(wiihandle,led);
    delay(300);
  }
  delay(10);
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
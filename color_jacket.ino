#include <M5StickCPlus.h>
#include "BLEDevice.h"
#include <Adafruit_NeoPixel.h>

BLEScan* pBLEScan;

const char* down_uuid = "00030461-1efe-4adb-bc7e-9f7f8e27fdc1";
const char* stop_uuid = "22230461-1efe-4adb-bc7e-9f7f8e27fdc1";
const char* up_uuid = "11130461-1efe-4adb-bc7e-9f7f8e27fdc1";
const char* denger_uuid = "99930461-1efe-4adb-bc7e-9f7f8e27fdc1";

BLEAdvertisedDevice *device;
static String doConnect = "false";
static bool bleStart = true;

#define Neopixel_PIN 33 // M5Stack Grobe Pin
#define NUMPIXELS  360  // Enough for 29 LEDs
#define NUMPIXELLINES 20

int LED_PATTERN_LOVE_HEART[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 84, 85, 86, 87, 88, 89, 107, 108, 109, 116, 117, 121, 122, 133, 134, 143, 144, 156, 157, 164, 165, 166, 177, 178, 181, 182, 193, 194, 195, 202, 203, 215, 216, 225, 226, 237, 238, 242, 243, 250, 251, 252, 270, 271, 272, 273, 274, 275, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357};
int LED_PATTERN_DREAM[] = {82, 89, 90, 91, 92, 106, 107, 108, 109, 110, 112, 113, 114, 117, 122, 125, 127, 129, 132, 133, 144, 145, 146, 147, 148, 150, 152, 153, 154, 156, 157, 158, 159, 162, 165, 167, 169, 171, 175, 176, 177, 180, 181, 182, 184, 185, 188, 190, 192, 194, 197, 200, 201, 202, 203, 205, 206, 207, 209, 210, 211, 212, 213, 214, 219, 221, 222, 223, 224, 225, 230, 232, 234, 237, 242, 245, 246, 247, 249, 250, 251, 252, 267, 268, 269, 270, 277};
int LED_PATTERN_Z[] = {82, 83, 84, 97, 98, 99, 100, 101, 102, 112, 113, 114, 115, 116, 117, 122, 123, 124, 125, 126, 127, 137, 138, 139, 140, 141, 142, 149, 150, 151, 155, 156, 157, 162, 163, 164, 168, 169, 170, 177, 178, 179, 180, 181, 182, 186, 187, 188, 195, 196, 197, 202, 203, 204, 211, 212, 213, 217, 218, 219, 220, 221, 222, 223, 224, 225, 235, 236, 237, 242, 243, 244, 254, 255, 256, 257, 258, 259, 260, 261, 262, 275, 276, 277};
int LED_PATTERN_HEART[] = {84, 85, 86, 87, 88, 89, 107, 108, 109, 116, 117, 121, 122, 133, 134, 143, 144, 156, 157, 164, 165, 166, 177, 178, 181, 182, 193, 194, 195, 202, 203, 215, 216, 225, 226, 237, 238, 242, 243, 250, 251, 252, 270, 271, 272, 273, 274, 275};
int step_num = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, Neopixel_PIN, NEO_GRB + NEO_KHZ800);

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      //  Serial.print("Advertised Device found: ");
       if(advertisedDevice.haveServiceUUID()){
            //if(strncmp(advertisedDevice.getServiceUUID().toString().c_str(),uuid, 36) == 0){
            // Serial.print("Advertised Device found: ");
            // Serial.println(advertisedDevice.toString().c_str());
            if(strncmp(advertisedDevice.getServiceUUID().toString().c_str(),down_uuid, 36) == 0){
                Serial.println(advertisedDevice.getServiceUUID().toString().c_str());
                //M5.Lcd.printf("getUUID: %s\r\n", advertisedDevice.getServiceUUID().toString().c_str());
                device = new BLEAdvertisedDevice(advertisedDevice);
                doConnect = "down";
                pBLEScan->clearResults();  // delete results fromBLEScan buffer to release 
                BLEDevice::getScan()->stop();
            } else if(strncmp(advertisedDevice.getServiceUUID().toString().c_str(),stop_uuid, 36) == 0){
                Serial.println(advertisedDevice.getServiceUUID().toString().c_str());
                device = new BLEAdvertisedDevice(advertisedDevice);
                doConnect = "stop";
                pBLEScan->clearResults();  // delete results fromBLEScan buffer to release 
                BLEDevice::getScan()->stop();
            } else if(strncmp(advertisedDevice.getServiceUUID().toString().c_str(),up_uuid, 36) == 0){
                Serial.println(advertisedDevice.getServiceUUID().toString().c_str());
                device = new BLEAdvertisedDevice(advertisedDevice);
                doConnect = "up";
                pBLEScan->clearResults();  // delete results fromBLEScan buffer to release 
                BLEDevice::getScan()->stop();
            } else if(strncmp(advertisedDevice.getServiceUUID().toString().c_str(),denger_uuid, 36) == 0){
                Serial.println(advertisedDevice.getServiceUUID().toString().c_str());
                device = new BLEAdvertisedDevice(advertisedDevice);
                doConnect = "denger";
                pBLEScan->clearResults();  // delete results fromBLEScan buffer to release 
                BLEDevice::getScan()->stop();
            } else {
                doConnect = "false"; 
            }
        } else {
          doConnect = "false";
        }

    }
};

void Task1(void *pvParameters) {
  // loop()書くとBLEスキャン中M5.update()が実行されなくてボタンが取れないのでマルチスレッド化している
  while(1) {
    
    if(bleStart){
      pBLEScan->start(1, false);
      drawScreenHeader();
      pixels.clear();  
      lightPixcel(doConnect);
      drawScreen();   
    } else {
      pixels.clear();
      lightPixcel(doConnect);
    }   
  }
}

void drawScreenHeader() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(RED);
    M5.Lcd.print("BLE Scan!\n");
}

void stopScreenHeader(){
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(RED);
    M5.Lcd.print("BLE Stop!\n");
}

void drawScreen() {
    //drawScreenHeader();
    M5.Lcd.setCursor(0, 30);
    M5.Lcd.setTextSize(7);
    M5.Lcd.setTextColor(WHITE);
    //M5.Lcd.printf(" %2d",deviceNum);

    M5.Lcd.printf("%s", doConnect);

}




void setup() {
    M5.begin();
    M5.Lcd.setRotation(3);
    M5.Lcd.fillScreen(WHITE);
    M5.Lcd.setTextColor(BLACK, WHITE);

    M5.Lcd.println("Neopixel Example");
    
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();  //create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
    pBLEScan->setInterval(100); // スキャン間隔5秒
    pBLEScan->setWindow(99);  // less or equal setInterval value
    xTaskCreatePinnedToCore(Task1,"Task1", 4096, NULL, 3, NULL, 1);
    //内容は([タスク名], "[タスク名]", [スタックメモリサイズ(4096or8192)],
    //      NULL, [タスク優先順位](1-24,大きいほど優先順位が高い)],
    //      [宣言したタスクハンドルのポインタ(&thp[0])], [Core ID(0 or 1)]); 
   
    pixels.begin();
}

void setPattern(String pattern){
  Serial.println(pattern);
  int pattern_index = 0;
  if(pattern == "down"){
    int pixelHue = 0;
    for(int i=0; i<NUMPIXELS; i++){
      if((i<20 || i>339)&& i%10 == 0){
        pixelHue = step_num + (i * 65536L / NUMPIXELLINES);        
      }else if(i>=20 && i<=239 && i%20 == 0){
        pixelHue = step_num + (i * 65536L / NUMPIXELLINES); 
      }
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
    }
  } else if(pattern == "stop"){
    for(int i=0; i<NUMPIXELS; i++){
      if(i == LED_PATTERN_LOVE_HEART[pattern_index]){
        pixels.setPixelColor(i, pixels.Color(0, 150, 0));
        pattern_index++;
      }
    }
  } else if(pattern == "up"){
    for(int i=0; i<NUMPIXELS; i++){
      if(i == LED_PATTERN_Z[pattern_index]){
        pixels.setPixelColor(i, pixels.Color(0, 150, 0));
        pattern_index++;
      }
    }
  } else if(pattern == "denger"){
    for(int i=0; i<NUMPIXELS; i++){
      // if(i == LED_PATTERN_Z[pattern_index]){
        pixels.setPixelColor(i, pixels.Color(150, 0, 0));
        // pattern_index++;
      // }
    }
  } else {
    for(int i=0; i<NUMPIXELS; i++){
      //消灯
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));    
    }
  }
}

void lightPixcel(String onLight){
  // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
  pixels.setBrightness(100);
  setPattern(onLight);
  pixels.show();
  step_num += 2048;
  if (step_num >= 65536) {
    step_num = 0;
  }
}

void loop() {
  M5.update();
  
  // ホームボタンが現在押されているか？
  if (M5.BtnA.wasPressed() ) {
    bleStart = false;
    doConnect = "false";  
  } else if(M5.BtnB.wasPressed()){
    //右ボタンが現在押されているか？      
    bleStart = true;   
    doConnect = "false";  
  }
}

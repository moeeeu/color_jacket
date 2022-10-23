#include <M5StickCPlus.h>
#include "BLEDevice.h"
#include <Adafruit_NeoPixel.h>

BLEScan* pBLEScan;

const char* uuid = "00030461-1efe-4adb-bc7e-9f7f8e27fdc1";
BLEAdvertisedDevice *device;
static bool doConnect = false;
static bool bleStart = true;

#define Neopixel_PIN 33 // M5Stack Grobe Pin
#define NUMPIXELS  30  // Enough for 29 LEDs
int led_color[3][3] ={{10,0,0},{0,10,0},{0,0,10}};
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, Neopixel_PIN, NEO_GRB + NEO_KHZ800);

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
       Serial.print("Advertised Device found: ");
       if(advertisedDevice.haveServiceUUID()){
            //if(strncmp(advertisedDevice.getServiceUUID().toString().c_str(),uuid, 36) == 0){
            Serial.print("Advertised Device found: ");
            Serial.println(advertisedDevice.toString().c_str());
            if(strncmp(advertisedDevice.getServiceUUID().toString().c_str(),uuid, 36) == 0){
                //M5.Lcd.printf("getUUID: %s\r\n", advertisedDevice.getServiceUUID().toString().c_str());
                device = new BLEAdvertisedDevice(advertisedDevice);
                doConnect = true;
            } else {
                doConnect = false; 
            }
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
      pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory      
      lightPixcel(doConnect);
      drawScreen();   
    } else {
      pBLEScan->stop();
      stopScreenHeader();
      pixels.clear();
      lightPixcel(doConnect);
      delay(1000);
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

    M5.Lcd.printf("%s", doConnect ? "true" : "false");

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




void lightPixcel(bool onLight){
  // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
  pixels.setBrightness(100);
  if(onLight){
    for(int i=0; i<NUMPIXELS; i++){
        int rgb = i%3;
        pixels.setPixelColor(i, pixels.Color(led_color[rgb][0],led_color[rgb][1],led_color[rgb][2]));      
    }
  } else {
    for(int i=0; i<NUMPIXELS; i++){
      //消灯
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));    
    }
  }  
  pixels.show();
}


void loop() {
  M5.update();
  
  // ホームボタンが現在押されているか？
  if (M5.BtnA.wasPressed() ) {
    bleStart = false;
    doConnect = false;  
  } else if(M5.BtnB.wasPressed()){
    //右ボタンが現在押されているか？      
    bleStart = true;   
    doConnect = false;  
  }
}


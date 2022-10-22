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
            }
        }

    }
};


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
    pixels.begin();
}




void lightPixcel(bool onLight){
  // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
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
     M5.Lcd.println("BLE Stopped!");
     pBLEScan ->stop();  //create new scan
     bleStart = false;
     
  } else if(M5.BtnB.wasPressed()){
    //右ボタンが現在押されているか？  
    M5.Lcd.println("BLE Start!");
    bleStart = true;      
  }


  if(bleStart){
    pBLEScan->start(1, false);
    pixels.clear();

    if(doConnect == true){
      lightPixcel(doConnect);
      doConnect = false;
    } else {
      lightPixcel(doConnect);   
    }
    delay(1000); // Delay a second between loops.        
  }
  
}


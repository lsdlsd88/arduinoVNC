/*
 * VNC_ILI9341_touch.ino
 *
 *  Created on: 20.02.2016
 *
 * required librarys:
 *  - SPI (arduino core)
 *  - WiFi (arduino core)
 *  - Adafruit_GFX (https://github.com/adafruit/Adafruit-GFX-Library)
 *  - Adafruit_ILI9341 (https://github.com/Links2004/Adafruit_ILI9341)
 *  - arduinoVNC (https://github.com/Links2004/arduinoVNC)
 */


#include <bb_captouch.h>

// These defines are for a low cost ESP32 LCD board with the GT911 touch controller
#define TOUCH_SDA 33
#define TOUCH_SCL 32
#define TOUCH_INT 21
#define TOUCH_RST 25

BBCapTouch bbct;
const char *szNames[] = {"Unknown", "FT6x36", "GT911", "CST820"};

#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include <VNC_ILI9341.h>
#include <VNC.h>


// ILI9341
#define TFT_DC      (2)
#define TFT_CS      (15)
#define TFT_RESET   (-1)
#define TFT_BL      (27) //some CYD variants use pin 27


const char * vnc_ip = "192.168.1.12";
const uint16_t vnc_port = 5900;
const char * vnc_pass = "12345678";

const char* ssid = "your-ssid";
const char* password = "your-password";

SPIClass hspi = SPIClass(HSPI);
ILI9341VNC tft = ILI9341VNC(&hspi, TFT_DC, TFT_CS, TFT_RESET);
arduinoVNC vnc = arduinoVNC(&tft);

void TFTnoWifi(void) {
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, ((tft.getHeight() / 2) - (5 * 8)));
    tft.setTextColor(ILI9341_RED);
    tft.setTextSize(5);
    tft.println("NO WIFI!");
    tft.setTextSize(2);
    tft.println();
}


void TFTnoVNC(void) {
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, ((tft.getHeight() / 2) - (4 * 8)));
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(4);
    tft.println("connect VNC");
    tft.setTextSize(2);
    tft.println();
    tft.print(vnc_ip);
    tft.print(":");
    tft.println(vnc_port);
}

void setup(void) {
    pinMode(TFT_BL,OUTPUT);analogWrite(TFT_BL,200);
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    
  bbct.init(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);
  int iType = bbct.sensorType();
  Serial.printf("Sensor type = %s\n", szNames[iType]);
  

    Serial.println();
    Serial.println();
    Serial.println();

    // Init ILI9341
    tft.begin();
    delay(10);
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLUE);

#ifdef ESP8266
    // disable sleep mode for better data rate
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
#endif

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    TFTnoWifi();
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    TFTnoVNC();

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println(F("[SETUP] VNC..."));

    vnc.begin(vnc_ip, vnc_port);
    vnc.setPassword(vnc_pass); // optional
}

long lasttouch=0;

void loop() {
  
 int i;
 TOUCHINFO ti;
  uint16_t lx, ly;
    if(WiFi.status() != WL_CONNECTED) {
        vnc.reconnect();
        TFTnoWifi();
        delay(100);
    } else {
        if(vnc.connected()) {
          if ((millis()-lasttouch)>500){
  if (bbct.getSamples(&ti)) {
    for (int i=0; i<ti.count; i++){
      Serial.print("Touch ");Serial.print(i+1);Serial.print(": ");;
      Serial.print("  x: ");Serial.print(ti.x[i]);ly=240-ti.x[i];
      Serial.print("  y: ");Serial.print(ti.y[i]);lx=ti.y[i];
      Serial.print("  size: ");Serial.println(ti.area[i]);
      Serial.println(' ');
    } // for each touch point
      vnc.mouseEvent(lx, ly, 0b001);
      lasttouch=millis();
  delay(10);
  vnc.mouseEvent(lx, ly, 0b000);
  }
  // if touch event happened
        }
        }
        vnc.loop();
        if(!vnc.connected()) {
            TFTnoVNC();
            // some delay to not flood the server
            delay(5000);
        }
    }
}

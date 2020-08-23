//
//    FILE: oled.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: September 4, 2017
// Released to the public domain
//
//  https://github.com/squix78/esp8266-oled-ssd1306 
//  Find it in the Arduino library manager under "ESP8266 and ESP32 Oled Driver for SSD1306 display"
#include "SSD1306.h"
#ifdef ESP32
#define D3 14  //Data
#define D2 27  //Clock
#endif
SSD1306  display(0x3c, D3, D2);  //Data, Clock

void oled_setup() {
  display.init();
  display.clear();  
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0,13, "initialize OLED...");  
  display.display();
}

void oled_wifi_going(int cnt, char* ssid, char* password) {
  display.clear();  
  display.setFont(ArialMT_Plain_10);
  display.drawString(0,13, "waiting wifi..."); 
  display.drawString(0,24, String("ssid= ")+String(ssid));
  display.drawString(0,35, String("password= ")+ String(password)); 
  display.drawString(0,53, String(cnt));
  display.display();  
}

void oled_no_wifi() {
  display.clear();  
  display.setFont(ArialMT_Plain_10);
  display.drawString(0,0, "NO WIFI: skip wifi..."); 
  display.drawString(0,13, "waiting dust value..."); 
  display.drawString(0,24, String("check sensor wiring."));
  display.display();  
}

void oled_waiting_dust(int cnt) {
  display.clear();  
  display.setFont(ArialMT_Plain_10);
  display.drawString(0,0, "NO SENSOR: ...");   
  display.drawString(0,13, "check sensor wiring"); 
  display.drawString(0,53, String(cnt));
  display.display();  
}

bool m = false;
int v1[128], v2[128];

void oled_show(int pm25, int pm10, String msg) {
  display.clear();   
  display.setFont(ArialMT_Plain_24);
  display.drawString(0,13, String(pm25));
  display.drawString(48,13, String(pm10));
  display.setFont(ArialMT_Plain_10);
  display.drawString(0,0, "pm2.5");
  display.drawString(48,0, "pm10"); 
  if (msg) display.drawString(127-display.getStringWidth(msg), 0, msg);

  int vmax = 0;
  int vmin = 999;
  for (int i=0; i<128; i++) {
    if (i<127) { v1[i] = v1[i+1]; v2[i] = v2[i+1]; }
    else { v1[i] = pm25; v2[i] = pm10; }
    vmax = vmax>v2[i]?vmax:v2[i]; // assume that v2 > v1 always
    vmin = vmin<v1[i]?vmin:!v1[i]?vmin:v1[i];
  }

  const int YSPAN = 26;
  int range = vmax - vmin;
  float scale = range>YSPAN?float(YSPAN)/range:1.;
  int delta = vmax>YSPAN?vmin:0;

  for (int i=0; i< 128; i++) {
    if (v1[i] && v2[i]) {
      display.drawVerticalLine(i, 63-int(scale*(v2[i]-delta)), !int(scale*(v2[i]-v1[i]))?1:int(scale*(v2[i]-v1[i]))); 
    }
  }
  display.display();
}

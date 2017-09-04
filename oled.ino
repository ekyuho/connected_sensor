//
//    FILE: oled.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: September 4, 2017
// Released to the public domain
//
//  https://github.com/squix78/esp8266-oled-ssd1306 

#include "SSD1306.h"
SSD1306  display(0x3c, D3, D2);  //Data, Clock


void setup_oled() {
  display.init();
  display.clear();  
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0,13, "Initializing...");  
  display.display();
}

bool m = false;
int v1[128], v2[128];

void do_oled(int pm25, int pm10) {
  String m1;
  display.clear();  
  display.setFont(ArialMT_Plain_10);
  if (m) m1 = "*"; else m1 = " ";
  m = m?false:true;  
  display.drawString(123,0, m1);  
  display.drawString(0,0, "pm2.5");
  display.drawString(64,0, "pm10");  
  display.setFont(ArialMT_Plain_24);
  display.drawString(0,13, String(pm25));
  display.drawString(64,13, String(pm10));


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


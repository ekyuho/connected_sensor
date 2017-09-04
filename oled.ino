//  https://github.com/squix78/esp8266-oled-ssd1306 

#include "SSD1306.h"
SSD1306  display(0x3c, D3, D2);  //Data, Clock


void setup_oled() {
  display.init();
  display.clear();  
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0,16, "Initializing...");  
  display.display();
}

bool m = false;

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
  display.drawString(0,16, String(pm10));
  display.drawString(64,16, String(pm25));
  display.display();
}


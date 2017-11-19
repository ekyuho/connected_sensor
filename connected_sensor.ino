//
//    FILE: connected_sensor.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: September 4, 2017
// Last Modified: November 20, 2017
// Released to the public domain
//
#include "ThingSpeak.h"
ThingSpeak ts("0D0HNYXTWO10M27Q"); // with no key, no data will be stored.

#include "MagicEco.h"
MagicEco me("5a104e049d7c7482f830351f"); // with no key, no data will be stored.

#include "MyWifi.h"
MyWifi mywifi("cookie", "0317137263");

#include <SoftwareSerial.h>
SoftwareSerial dustport(D1, D0, false, 256);  //RX, TX

// At Oled.ino
//SSD1306  display(0x3c, D3, D5);  //Data, Clock

#include "Dust.h"
Dust dust;

#include "RunningMedian.h"
RunningMedian pm25s = RunningMedian(19);
RunningMedian pm10s = RunningMedian(19);

const int RATIO = 10;
const int INTERVAL = 60000;

void setup() {
  Serial.begin(115200);
  dustport.begin(9600);

  oled_setup();
  mywifi.connect_ap();
  if (!mywifi.connected) oled_no_wifi();

  Serial.println("\nDust Sensor Box V2.0, 2017/11/20 Kyuho Kim");
}

void got_dust(int pm25, int pm10) {
   pm25 /= RATIO;
   pm10 /= RATIO;
   //Serial.println(String(pm25) +","+ String(pm10)+ "  ");
   pm25s.add(pm25);
   pm10s.add(pm10);
   //Serial.println("pm25 size="+ String(pm25s.getSize()) +", count="+ String(pm25s.getCount()) +", median="+ String(pm25s.getMedian()));
   //Serial.println("pm10 size="+ String(pm10s.getSize()) +", count="+ String(pm10s.getCount()) +", median="+ String(pm10s.getMedian()));
   void oled_show(int, int, bool);   
   oled_show(pm25, pm10, mywifi.connected);
}

void do_interval() {
  if (!mywifi.connected) return;

  me.send(int(pm25s.getMedian()), int(pm10s.getMedian()));
  ts.send(int(pm25s.getMedian()), int(pm10s.getMedian()));
}

unsigned long mark = 0, sec_mark = 0;
boolean got_interval = false, got_sec = false;
unsigned long missings = 0;
  
void loop() {
  unsigned long current = millis();
  
  if (current > mark) {
    mark = current + INTERVAL;
    got_interval = true;
  }
  if (current > sec_mark) {
    sec_mark = current + 3000;
    got_sec = true;
    missings++;
  }
  while (dustport.available() > 0) {
    dust.do_char(dustport.read(), got_dust);
    missings = 0;
    yield();
  }
  if (got_interval) {
    got_interval = false;
    do_interval();
  }
  if (got_sec) {
    got_sec = false;
    if (missings > 3) oled_waiting_dust(missings);
  }
  yield();
}

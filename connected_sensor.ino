//
//    FILE: connected_sensor.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: September 4, 2017
// Last Modified: April 1, 2019
// Released to the public domain
//
#include "MyWifi.h"
MyWifi mywifi("KT_GiGA_2G_4A44", "3eb40ie803");

#include "ThingSpeak.h"
#define THINGSPEAKKEY "YourKeyPlease"
ThingSpeak ts(THINGSPEAKKEY); // with no key, no data will be stored.

#include "Sogang.h"
Sogang sg; 

#include <SoftwareSerial.h>
SoftwareSerial dustport(D1, D0, false, 256);  //RX, TX

// At Oled.ino
//SSD1306  display(0x3c, D3, D2);  //Data, Clock

#include "Dust.h"
Dust dust;

#include "RunningMedian.h"
RunningMedian pm25s = RunningMedian(19);
RunningMedian pm10s = RunningMedian(19);

const int RATIO = 1;
const int INTERVAL = 60000;
unsigned MYMIN = 0;
unsigned MYSEC = 0;

void setup() {
  Serial.begin(115200);
  dustport.begin(9600);
  oled_setup();

  Serial.println("\n\nDust Sensor Box V2.1, 2019/4/1 by Valve God, Kyuho Kim");
  Serial.print("\nConnect WiFi AP: ");
  mywifi.connect_ap();
  if (!mywifi.connected) oled_no_wifi();
  sg.setapikey(mywifi.macstring());

  Serial.println("Receiving Sensor Data: ");
}

void got_dust(int pm25, int pm10) {
   pm25 /= RATIO;
   pm10 /= RATIO;

   Serial.print(String(MYMIN)+"M "+ String(MYSEC) +"S, [pm25, pm10]=[");
   Serial.println(String(pm25) +", "+ String(pm10)+ "]");
   pm25s.add(pm25);
   pm10s.add(pm10);
   //Serial.println("pm25 size="+ String(pm25s.getSize()) +", count="+ String(pm25s.getCount()) +", median="+ String(pm25s.getMedian()));
   //Serial.println("pm10 size="+ String(pm10s.getSize()) +", count="+ String(pm10s.getCount()) +", median="+ String(pm10s.getMedian()));

   String msg = "";
   if (!mywifi.connected) msg = "NO WIFI";
   void oled_show(int, int, String);   
   oled_show(pm25, pm10, msg);
}

void do_interval() {
  if (!mywifi.connected) return;
  if (THINGSPEAKKEY != "YourKeyPlease") ts.send(int(pm25s.getMedian()), int(pm10s.getMedian()));
  sg.send(int(pm25s.getMedian()), int(pm10s.getMedian()));
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
    sec_mark = current + 1000;
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
    MYMIN = (current/1000) / 60;
    MYSEC = (current/1000) % 60;
    
    if (missings > 15) {
      oled_waiting_dust(missings);
      Serial.println(String(MYMIN)+"M "+ String(MYSEC) +"S: No data from dust sensor. check wiring.");
    }
  }
  yield();
}

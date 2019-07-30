//
//    FILE: connected_sensor.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: September 4, 2017
// Last Modified: July 27, 2019
// Released to the public domain
//
#include "MyWifi.h"
MyWifi mywifi;

#include "ThingSpeak.h"
#define THINGSPEAKKEY "YOUR_THINGSPEAK_KEY"
ThingSpeak ts(THINGSPEAKKEY); // with no key, no data will be stored.

#include "Sogang.h"
Sogang sg; 

#include <SoftwareSerial.h>
SoftwareSerial dustport(D4, D0, false, 256);  //RX, TX

#include "Dust.h"
Dust dust;

#include "RunningMedian.h"
RunningMedian pm25s = RunningMedian(19);
RunningMedian pm10s = RunningMedian(19);

// D4:RX, D3:Data, D2: CLOCK
const int INTERVAL = 60000;
String cmd = "";
int tick = 1;
unsigned long mark = 0, sec_mark = 0;
boolean got_interval = false, got_sec = false;
unsigned long missings = 0;

void got_dust(int pm25, int pm10) {
   Serial.printf("%02d: pm25,pm10=", tick++);
   Serial.println(String(pm25) +","+ String(pm10));
   pm25s.add(pm25);
   pm10s.add(pm10);

   String msg = "";
   if (!mywifi.connected) msg = "no WIFI";
   else if (!mywifi.tcp) msg = "no TCP";
   else if (!mywifi.ack) msg = "no ACK";
   else if (!sg.user) msg = "waiting";
   else msg = String(sg.user);
   void oled_show(int, int, String);   
   oled_show(pm25, pm10, msg);
}

void do_interval() {
  if (!mywifi.connect_ap()) return;

  ts.send(int(pm25s.getMedian()), int(pm10s.getMedian()));
  String r = sg.send(int(pm25s.getMedian()), int(pm10s.getMedian()));
  if (r == "no TCP") mywifi.tcp = false;
  else if (r == "no ACK") mywifi.ack = false;
  else mywifi.tcp = mywifi.ack = true;
}

void got_cmd() {
	if (cmd == "") return;
	
	if (mywifi.configure(cmd)) {
		Serial.println("Restart in 3 seconds...");
		delay(3000);
		ESP.restart();
	}
	cmd = "";
}

void setup() {
  Serial.begin(115200);
  dustport.begin(9600);
  
  Serial.println("\n\nDust Sensor Box V2.1, 2019/7/22 by Valve God, Kyuho Kim");  
  oled_setup();

  mywifi.begin();
  if (!mywifi.connect_ap()) oled_no_wifi();
  sg.setapikey(mywifi.macstring());
  Serial.print("my address=");
  Serial.println(WiFi.localIP());

  Serial.println("Receiving Sensor Data: ");
  mark = millis() + INTERVAL;
}
  
void loop() {
  unsigned long current = millis();
  
  if (current > mark) {
    mark = current + INTERVAL;
    got_interval = true;
    tick = 1;
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
    
    if (missings > 5) {
      oled_waiting_dust(missings);
      Serial.printf("No data from dust sensor. check wiring.\n");
    }
  }
  
  while (Serial.available()) {
	char c = Serial.read();
	if (c == '\n' || c == '\r') got_cmd();
	else cmd += String(c);
  }
  yield();
}

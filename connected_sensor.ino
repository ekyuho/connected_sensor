#include <SoftwareSerial.h>
SoftwareSerial dust(D1, D7, false, 256);

#include "RunningMedian.h"
RunningMedian pm25s = RunningMedian(19);
RunningMedian pm10s = RunningMedian(19);

const int INTERVAL = 60000;
String MYAPIKEY = "MVBATSXOGBR62YZX";
char* ssid = "cookie"; //"Seoul IOT_2(2.4G)";
char* password = "0317137263"; //"12345678";
const int RATIO = 10;

void setup() {
  Serial.begin(74880);
  dust.begin(9600);
  setup_oled();
  connect_ap(ssid, password);

  Serial.println("\nDust Sensor Box V1.0, 2017/9/4 Kyuho Kim");
}

void got_dust(int pm25, int pm10) {
   pm25 /= RATIO;
   pm10 /= RATIO;
   //Serial.println(String(pm25) +","+ String(pm10)+ "  ");
   pm25s.add(pm25);
   pm10s.add(pm10);
   //Serial.println("pm25 size="+ String(pm25s.getSize()) +", count="+ String(pm25s.getCount()) +", median="+ String(pm25s.getMedian()));
   //Serial.println("pm10 size="+ String(pm10s.getSize()) +", count="+ String(pm10s.getCount()) +", median="+ String(pm10s.getMedian()));
   do_oled(int(pm25s.getMedian()),int(pm10s.getMedian()));
}

void do_interval() {
  do_thingspeak(MYAPIKEY, int(pm25s.getMedian()), int(pm10s.getMedian()));
}

unsigned long mark = 0;
boolean got_interval = false;
  
void loop() {

  if (millis() > mark) {
    mark = millis() + INTERVAL;
    got_interval = true;
  }
  while (dust.available() > 0) {
    do_dust(dust.read(), got_dust);
    yield();
  }
  if (got_interval) {
    got_interval = false;
    do_interval();
  }
  yield();
}

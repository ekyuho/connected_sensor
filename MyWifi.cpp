//
//    FILE: wifi.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: September 4, 2017
// Released to the public domain
//
#include "MyWifi.h"
#include <ESP8266WiFi.h>

MyWifi::MyWifi(String _ssid, String _password) {
  _ssid.toCharArray(ssid, 32);
  _password.toCharArray(password, 32);
  connected = false;
}

MyWifi::MyWifi(String ssid) {
  MyWifi(ssid, String(""));
}

void MyWifi::connect_ap() {
  int count = 60;
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    void oled_wifi_going(int, char*, char*);
    oled_wifi_going(count, ssid, password);
    if (!count--) {
      Serial.print("\nNO WIFI");
      return;
    }
  }
  Serial.print("\n Got WiFi, IP address: ");
  Serial.println(WiFi.localIP()); 
  connected = true;
  return;
}

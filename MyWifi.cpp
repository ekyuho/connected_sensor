//
//    FILE: wifi.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: September 4, 2017
// Released to the public domain
//
#include "MyWifi.h"
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <EEPROM.h>
#include <assert.h>

void oled_wifi_going(int, char*, char*);
void oled_no_wifi(void);


MyWifi::MyWifi(void) {
  tcp = true;
  ack = true;
}

void MyWifi::print(Sogang *sg) {
  Serial.printf( "  ssid,pass=%s,%s (current)for %d@%s\n", ssid, password, sg->user, sg->apikey.c_str());
}

void MyWifi::begin(void) { 
	EEPROM.begin(64);
	String cmd = "";
	for (int i=0; i< 64; i++) {
		char c = EEPROM.read(i);
		if (!c) break;
		cmd += String(c);
	}
	
	if (!cmd.startsWith("ssid,pass=")) {
		cmd = "ssid,pass=cookie2,0317137263";
		MyWifi(cmd);
	}
	EEPROM.commit();

	parse(cmd);
}

void MyWifi::configure(String cmd) {
	parse(cmd);
	EEPROM.begin(64);
	for (int i=0; i<cmd.length(); i++)
		EEPROM.write(i, cmd.charAt(i));
	EEPROM.write(cmd.length(), 0);
	EEPROM.commit();
  ESP.restart();
}

void MyWifi::parse(String cmd) {
	String ssid_S = cmd.substring(10);
	int k = ssid_S.indexOf(",");
	String password_S = ssid_S.substring(k+1);
	ssid_S = ssid_S.substring(0,k);
	strcpy(ssid, ssid_S.c_str());
	strcpy(password, password_S.c_str());
}

void MyWifi::scan () {
    int n = WiFi.scanNetworks();
    Serial.println("Networks nearby");
    for (int i = 0; i < n; ++i) {
	  Serial.printf("%2d: %s (%d) %s\n", i+1, WiFi.SSID(i).c_str(), WiFi.RSSI(i), WiFi.BSSIDstr().c_str());
    }
}

bool MyWifi::isConnected(void) {
	return WiFi.isConnected();
}

bool MyWifi::connect_ap() {
  if (WiFi.status() == WL_CONNECTED) return "";

  scan();
  int count = 30;
  Serial.printf("\nconnecting to %s(%s)\n", ssid, password);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    oled_wifi_going(count, ssid, password);
    if (!count--) {
      Serial.println("\nNO WIFI");
	  oled_no_wifi();
      return false;
    }
  }
  Serial.println("............");
  return true;
}

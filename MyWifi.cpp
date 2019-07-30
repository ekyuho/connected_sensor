//
//    FILE: wifi.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: September 4, 2017
// Released to the public domain
//
#include "MyWifi.h"
#include <ESP8266WiFi.h>
#include <EEPROM.h>


MyWifi::MyWifi(void) {
	connected = false;
  tcp = true;
  ack = true;
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

bool MyWifi::configure(String cmd) {
	if (!cmd.startsWith("ssid,pass=")) {
		Serial.println("Command syntax:");
		Serial.println("  ssid,pass=yourssid,yourpass");
		Serial.printf( "  ssid,pass=%s,%s (current)\n", ssid, password);
		return false;
	}
	if (cmd.startsWith("restart"))
		return true;
	
	parse(cmd);
	
	EEPROM.begin(64);
	for (int i=0; i<cmd.length(); i++)
		EEPROM.write(i, cmd.charAt(i));
	EEPROM.write(cmd.length(), 0);
	EEPROM.commit();
	return true;
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
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
    }
}

String MyWifi::macstring(void) {
  return(_macstring);
}

bool MyWifi::connect_ap() {
  if (connected && WiFi.status() == WL_CONNECTED) return "";
	
  int count = 30;
  Serial.println();
  Serial.printf("connecting to %s(%s)\n", ssid, password);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    void oled_wifi_going(int, char*, char*);
    oled_wifi_going(count, ssid, password);
    if (!count--) {
      Serial.println("\nNO WIFI");
      scan();
      connected = false;
      return false;
    }
  }
  Serial.print("\nMy IP address: ");
  Serial.println(WiFi.localIP()); 
  Serial.print("My Mac address: ");
  byte mac[6]; 
  WiFi.macAddress(mac);
  char xx[13];
  for (int i=0; i<6; i++) sprintf(&xx[i*2], "%02x", mac[i]);
  xx[12] = 0;
  _macstring = String(xx);
  Serial.println(_macstring);
  
  connected = true;

  count = 10;
  Serial.println("stablizing network, waiting another 5 seconds ");
  while (count-- > 0) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  return true;
}

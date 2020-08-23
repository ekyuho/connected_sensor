//
//    FILE: server.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: November 20, 2017
// Released to the public domain
//
#include "Sogang.h"
#include <HTTPClient.h>

Sogang::Sogang(void) {
    _url = "http://h.damoa.io:8080/add?";  
	user = 0;
	apikey = "";
}

String Sogang::send(int pm25, int pm10) {
	HTTPClient http;

	byte mac[6];
	char macStr[18] = {0};
	if (apikey == "") {
		WiFi.macAddress(mac);
		sprintf(macStr, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		apikey = String(macStr);
	}

	String payload = "format=4&macapikey="+ apikey +"&type=D&value="+ String(pm25)+","+ String(pm10) +"&seq="+ String(_seq++);
	String s = _url + payload;

    Serial.println(s);
	http.begin(s);

	int httpCode = http.GET();
	String line;
	if(httpCode == HTTP_CODE_OK) {
		line = http.getString();
		Serial.println(line);
	}
	http.end();
	
	bool ack = false;
	if (line.startsWith("X-ACK:")) {
		Serial.println(line);
		ack = true;
		String u_s = line.substring(8, 11);
		String user_s = line.substring(12, 18);
		if (u_s != "\"u\"") {
			Serial.println("Format Error: "+ line);
		} else {
			user = atoi(user_s.c_str());
		}
	}


  if (!ack) return "no ACK";
  return "";
}

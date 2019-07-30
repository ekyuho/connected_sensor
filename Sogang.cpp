//
//    FILE: server.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: November 20, 2017
// Released to the public domain
//
#include "Sogang.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

Sogang::Sogang(void) {
    _host = "t.damoa.io";
    _url = "/add?";  
    _httpPort = 8080;
	user = 0;
}

void Sogang::setapikey(String apikey) {
    _apikey = apikey;
}

String Sogang::send(int pm25, int pm10) {
  WiFiClient client;

  if (!client.connect(_host, _httpPort)) {
    Serial.printf("opening TCP failed: %s:%d\n", _host, _httpPort);
    return("no TCP");
  }

  String payload = "format=4&macapikey="+ _apikey +"&type=D&value="+ String(pm25)+","+ String(pm10) +"&seq="+ String(_seq++);
  String getheader = "GET "+ String(_url) + String()+ "&"+ payload +" HTTP/1.1";
  client.println(getheader);
  client.println("User-Agent: ESP8266 Kyuho Kim");  
  client.println("Host: " + String(_host));  
  client.println("Connection: close");  
  client.println();

  Serial.println(getheader);
  int cnt = 1;
  bool ack = false;
  while (client.connected()||client.available()) {
    String line = client.readStringUntil('\n');
    Serial.printf("%d: %s\n", cnt++, line.c_str());
  	if (line.startsWith("X-ACK:")) {
      ack = true;
  		String u_s = line.substring(8, 11);
  		String user_s = line.substring(13, 19);
  		if (u_s != "\"u\"") {
  			Serial.println("Format Error: "+ line);
  		} else {
  			user = atoi(user_s.c_str());
  		}
  	}
  }
  client.stop();
  if (!ack) return "no ACK";
  return "";
}

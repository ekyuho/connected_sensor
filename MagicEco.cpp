//
//    FILE: server.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: November 20, 2017
// Released to the public domain
//
#include "MagicEco.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

MagicEco::MagicEco(String apikey) {
    _apikey = apikey;
    _host = "airstatus.magice.co";
    _url = "/api/update?api_key=";  
    _httpPort = 80;
}

boolean MagicEco::send(int pm25, int pm10) {
  return(send(pm25, pm10, float(0.0)));
}

boolean MagicEco::send(int pm25, int pm10, float temperature) { 
  WiFiClient client;

  if (_apikey == String("YOUR_MAGICECO_KEY")) {
    Serial.println("PLEASE SETUP YOUR OWN MAGIC ECO API KEY");
    return(false);
  }
  
  if (!client.connect(_host, _httpPort)) {
    Serial.print("connection failed: ");
    return(false);
  }

  String payload = "pm25="+ String(pm25) +"&pm10="+ String(pm10)+"&temperature="+ String(temperature);
  String getheader = "GET "+ String(_url) + String(_apikey)+ "&"+ payload +" HTTP/1.1";
  client.println(getheader);
  client.println("User-Agent: ESP8266 Kyuho Kim");  
  client.println("Host: " + String(_host));  
  client.println("Connection: close");  
  client.println();

  Serial.println(getheader);
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
  Serial.println("Done Magic.");
}



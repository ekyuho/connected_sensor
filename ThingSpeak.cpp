//
//    FILE: thingspeak.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: September 4, 2017
// Released to the public domain
//
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

ThingSpeak::ThingSpeak(String apikey) {
    _apikey = apikey;
    _host = "api.thingspeak.com";
    _url = "/update?api_key=";  
    _httpPort = 80;
    first = true;
}
boolean ThingSpeak::send(int pm25, int pm10) { 
  WiFiClient client;

  if (_apikey == String("YOUR_THINGSPEAK_KEY") || _apikey == "") {
    if (first) Serial.println("PLEASE SETUP YOUR OWN THINGSPEAK API KEY");
    first = false;
    return(false);
  }

  if (!client.connect(_host, _httpPort)) {
    Serial.print("connection failed: ");
    return(false);
  }

  String payload = "field1="+ String(pm25) +"&field2="+ String(pm10);
  String getheader = String("GET ")+ _url + _apikey + "&"+ payload +" HTTP/1.1";
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
  return(true);
}

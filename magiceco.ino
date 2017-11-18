//
//    FILE: server.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: November 20, 2017
// Released to the public domain
//
const char* host = "airstatus.magice.co";
String url = "/api/update?api_key=";  
const int httpPort = 80;

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

void do_magic(String APIKEY, int pm25, int pm10) {
  do_magic(APIKEY, pm25, pm10, 0.0)
}

void do_magic(String APIKEY, int pm25, int pm10, float temperature) { 
  WiFiClient client;

  if (APIKEY == String("")) return;
  if (!client.connect(host, httpPort)) {
    Serial.print("connection failed: ");
    return;
  }

  String payload = "pm25="+ String(pm25) +"&pm10="+ String(pm10)+"&temperature="+ String(temperature);
  String getheader = "GET "+ String(url) + String(APIKEY)+ "&"+ payload +" HTTP/1.1";
  client.println(getheader);
  client.println("User-Agent: ESP8266 Kyuho Kim");  
  client.println("Host: " + String(host));  
  client.println("Connection: close");  
  client.println();

  Serial.println(getheader);
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
  Serial.println("Send to Server.");
}

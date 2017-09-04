const char* host = "api.thingspeak.com";
String url = "/update?api_key=";  
const int httpPort = 80;

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

void do_thingspeak(String APIKEY, int pm25, int pm10) { 
  WiFiClient client;

  if (!client.connect(host, httpPort)) {
    Serial.print("connection failed: ");
    return;
  }

  String payload = "field1="+ String(pm25) +"&field2="+ String(pm10);
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
  Serial.println("Done Thingspeak.");
}



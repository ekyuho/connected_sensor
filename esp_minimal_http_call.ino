#include <ESP8266WiFi.h>
#include <WiFiClient.h>

void http_call(char* host, int port) { 
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.print("connection failed: ");
    return;
  }

  client.println("GET / HTTP/1.1"); 
  client.println("HOST: "+ String(host)); 
  client.println();

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin("cookie", "0317137263");
  Serial.println("Begin");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP()); 
  http_call("www.jejunu.ac.kr", 80);
}

void loop() {
}

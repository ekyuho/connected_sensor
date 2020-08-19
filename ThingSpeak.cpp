//
//    FILE: thingspeak.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: September 4, 2017
// Released to the public domain
//
#include "ThingSpeak.h"
#include <HTTPClient.h>

ThingSpeak::ThingSpeak(String apikey) {
    _apikey = apikey;
    _url = "http://api.thingspeak.com/update?api_key=";  
}

boolean ThingSpeak::send(int pm25, int pm10) { 
	HTTPClient http;

	if (_apikey == "") return(false);
 
    String s = _url+_apikey+"&field1="+String(pm25) +"&field2="+ String(pm10);
    Serial.println(s);
	http.begin(s);

	int httpCode = http.GET();
	if(httpCode == HTTP_CODE_OK) {
		String payload = http.getString();
		Serial.println(payload);
	}
	http.end();
	return(true);
}

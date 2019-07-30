#ifndef ThingSpeak_h
#define ThingSpeak_h

#include "Arduino.h"

class ThingSpeak 
{
  public:
	ThingSpeak(String apikey);
	boolean send(int pm25, int pm10);
  private:
	String _apikey = "";
	String _url;
	char* _host;
	int _httpPort;
  bool first;
};
#endif

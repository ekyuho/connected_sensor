//
//    FILE: wifi.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: September 4, 2017
// Released to the public domain
//
#ifndef MyWifi_h
#define MyWifi_h

#include "Arduino.h"

class MyWifi 
{
  public:
    MyWifi(void);
  	void begin(void);
  	bool configure(String);
    bool connect_ap(void);
    bool connected;
    bool tcp;
    bool ack;
    String macstring(void);
    char ssid[32];
    char password[32];
  private:
    String _macstring;
	void parse(String);
	void scan(void);
};
#endif

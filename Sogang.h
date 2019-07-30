//
//    FILE: server.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: November 20, 2017
// Released to the public domain
//
#ifndef Sogang_h
#define Sogang_h

#include "Arduino.h"

class Sogang
{
  public:
    Sogang(void);
    void setapikey(String);
	int user;
    String send(int, int); // type, value, seq
  private:
    String _apikey;
    String _url;
    char* _host;
    int _httpPort;   
    int _seq = 0;
};
#endif

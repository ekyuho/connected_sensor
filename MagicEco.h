//
//    FILE: server.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: November 20, 2017
// Released to the public domain
//
#ifndef MagicEco_h
#define MagicEco_h

#include "Arduino.h"

class MagicEco 
{
  public:
    MagicEco(String apikey);
	  boolean send(int, int, float);
    boolean send(int, int);
  private:
    String _apikey = "";
    String _url;
	  char* _host;
    int _httpPort;   
};
#endif

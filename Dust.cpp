//
//    FILE: dust.ino
//  AUTHOR: Kyuho Kim (ekyuho@gmail.com)
// CREATED: September 4, 2017
// Released to the public domain
//
#include "Dust.h"
//#define SDS011
#define PMSX003  

#ifdef SDS011
byte header[2] = { 0xAA, 0xC0 };
void Dust::do_char(char c, void (*function)(int, int)) {
    //Serial.print("stat="+ String(stat) +", "+ "cnt="+ String(cnt) +" ");
    //Serial.print(c, HEX);
    //Serial.println(" ");

    if (stat == 1) {
       if (c == header[0]) stat = 2;
    } else
    if (stat == 2) {
       if (c == header[1]) stat =3;
       else stat = 1;
    } else
    if (stat == 3) {
       buf[cnt++] = c;
       if (cnt == 7) stat = 4;
    } else
    if (stat == 4) {
       if (c != 0xAB) {
          Serial.printf("Eh? wrong tailer %02X", c);
          cnt = 0;
          stat = 1;
          return;
       }

       byte chksum = 0;
       for (int i=0; i<6; i++) chksum += buf[i];
       if (buf[6] - chksum) {
          Serial.printf("checksum err %02X != %02X\n", buf[6], chksum);
          cnt = 0;
          stat = 1;
          return;
       }
       cnt = 0;
       stat = 1;
       int pm25 = buf[0] + 256*buf[1];
       int pm10 = buf[2] + 256*buf[3];
       function(pm25/10, pm10/10);  //value too big
    }
}
#endif

#ifdef PMSX003
byte header[2] = { 0x42, 0x4d };
byte old_b=0, chksum=0, sum=0;
int bi=0, flen, pm25, pm10;

void Dust::do_char(char b, void (*function)(int, int)) {
    if (bi > 2 && old_b == header[0] && b == header[1]) {
      Serial.print("\n D Discard bi="+String(bi)+" ");
      bi = 1;
    }
    switch(bi) {
    case 0: 
      if (b == header[0]) {
        bi++; 
        //Serial.println();
        chksum = b;
      }
      break;
    case 1:
      if (b == header[1]) {
        bi++;
        chksum += b;
      } else bi = 0;
      break;
    case 2:
      chksum += b; bi++;
      flen = b*256;
      break;
    case 3:
      chksum += b; bi++;
      flen += b;
      break;
    case 4:
      chksum += b;
      pm25 = b*256; bi++;
      break;
    case 5:
      chksum += b;
      pm25 += b; bi++;
      break;
    case 8:
      chksum += b;
      pm10 = b*256; bi++;
      break;
    case 9:
      chksum += b;
      pm10 += b; bi++;
      break;
    default:
      if (bi == flen+2) {
        sum = b*256;
        bi++;
      } else if (bi == flen+3) {
        sum += b;
        if (chksum == sum) {
          function(pm25, pm10);
          bi = 0;
          old_b = b;
        } else {
          Serial.print(" Checksum Error: PMA3003 ");
          Serial.print(chksum, HEX);
          Serial.print("!=");
          Serial.print(sum, HEX);
          Serial.print("\n");
          bi = 0;
        }
      } else if (bi > flen+3) {
        bi = 0;
      } else {
        bi++;
        chksum += b;
      }
  }
  old_b = b;
}
#endif

Arduino Mega Server
version 0.14 (W5500 support)
2016, Hi-Lab.ru
  
License: Free, without any warranties
Home:    http://hi-lab.ru/arduino-mega-server (russian)
         http://hi-lab.ru/english/arduino-mega-server (english)
Email:   info@hi-lab.ru
  
IDE: Arduino 1.6.5 r2
     Processing 1.5.1

Pathes of project AMS:
------------------------------
\Sketches\ams\Arduino\arduino_mega_server\
\Sketches\ams\Arduino\libraries\
\Sketches\ams\Processing\arduino_serial_commander\

Arduino IDE settings:
---------------------
Sketches folder:
\Sketches\ams\Arduino

Loading sketches:
------------------  
Arduino Mega Server:      arduino_mega_server.ino
Arduino Serial Commander: arduino_serial_commander.pde
  
Quick start:
------------
1. Files from archive microSD.zip - to microSD card
2. Sketch "arduino_mega_server" - to Arduino Mega
3. Open on your browser address "192.168.1.37"
4. Enjoy and donate on page http://hi-lab.ru/arduino-mega-server/details/donate


Sketch settings:
----------------
  
W5100 (default)
#include <Ethernet.h>
#include <EthernetUdp.h>
//#include <Ethernet2.h>
//#include <EthernetUdp2.h>

W5500 (if Ethernet Shield 2 on W5500 chip)
//#include <Ethernet.h>
//#include <EthernetUdp.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h>
  
1. freeSockets = FREE_SOCKETS_ONE (default)
                 FREE_SOCKETS_ZERO (no network functions and fast interface)
2. modeNetWork = MODE_ONE (default)
                 MODE_MAJOR (tandem with MajorDoMo)
3. timeProvider = TIME_NETWORK (default)
                  TIME_RTC (hardware RTC)
                  TIME_NONE (no time synchronization)
4. //#define MAJORDOMO_FEATURE (default)
   #define MAJORDOMO_FEATURE (send data and synchronize time with MajorDoMo)

(on Ethernet page)
SELF_IP[] = {192, 168, 1, 37}; (default)
GATEWAY[] = {192, 168, 1, 1}; (default)

Tandem mode
SELF_IP[] = {192, 168, 2, 37}; (AMS)
192.168.2.8 (MajorDoMo)

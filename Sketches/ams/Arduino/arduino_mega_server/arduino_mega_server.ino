/*
  Arduino Mega Server
  version 0.14 (W5500 support)
  2016, Hi-Lab.ru
  
  License: Free, without any warranties
  Home:    http://hi-lab.ru/arduino-mega-server (russian)
           http://hi-lab.ru/english/arduino-mega-server (english)
  Email:   info@hi-lab.ru
  
  IDE: Arduino 1.6.5 r2
       Processing 1.5.1

  Sketch settings:
  ----------------
  
  (on this page)
  
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
  Arduino Mega Server:      arduino_mega_server_due.ino
  Arduino Serial Commander: arduino_serial_commander.pde
  
  Quick start:
  ------------
  1. Files from archive SD.zip - to microSD card
  2. Sketch "arduino_mega_server.ino" - to Arduino Mega
  3. Open on your browser address "192.168.1.37"
  4. Enjoy and donate on page
     http://hi-lab.ru/arduino-mega-server/details/donate
*/

// modules
#define ETHERNET_FEATURE
#define SERVER_FEATURE
#define RTC_FEATURE
#define SEND_FEATURE
//#define MAJORDOMO_FEATURE
#define UPLOAD_FEATURE
#define SD_INFO_FEATURE
//#define SD_FILES_FEATURE
//#define PING_FEATURE
//#define LAURENT_FEATURE
//#define PIRS_FEATURE
//#define CONTACTS_FEATURE
#define TEMP_FEATURE
//#define ELECTRO_FEATURE
#define LEDS_FEATURE
//#define KEYS_FEATURE
#define NOO_FEATURE

// debug
//#define ELECTRO_DEBUG
//#define SERVER_PROFILING
//#define EVENTS_CONTACTS
//#define EVENTS_PIRS
//#define TEMP_DEBUG

#include <avr/pgmspace.h>
#include <SPI.h>

// W5100
#include <Ethernet.h>
#include <EthernetUdp.h>

// W5500
//#include <Ethernet2.h>
//#include <EthernetUdp2.h>

#include <SD.h>
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>

char const SELF_NAME[] = "Mega";
char const VERSION[] = "0.14.2";

// mode net work
byte const MODE_ONE = 1;
byte const MODE_MAJOR = 2;
byte modeNetWork = MODE_ONE;

// time provider
byte const TIME_NONE = 0;
byte const TIME_NETWORK = 1;
byte const TIME_RTC = 2;
byte timeProvider = TIME_NETWORK;

// free sockets
byte const FREE_SOCKETS_ZERO = 0;
byte const FREE_SOCKETS_ONE = 1;
byte const FREE_SOCKETS_TWO = 2;
byte freeSockets = FREE_SOCKETS_ONE;

// mode work
byte const MODE_SERVER = 1;
byte const MODE_UPDATE = 2;
byte modeWork = MODE_SERVER;

// duration
time_t prevEventElectroCycle;
time_t prevEventMegaLive;
time_t prevEventRequest;

// modules
byte const MODUL_DISABLE = 0;
byte const MODUL_ENABLE = 1;
byte const MODUL_NOT_COMPILLED = 2;

byte modulEthernet = MODUL_NOT_COMPILLED;
byte modulRtc =      MODUL_NOT_COMPILLED;
byte modulSdCard =   MODUL_NOT_COMPILLED;
byte modulServer =   MODUL_NOT_COMPILLED;
byte modulSend =     MODUL_NOT_COMPILLED;
byte modulMajor =    MODUL_NOT_COMPILLED;
byte modulLaurent =  MODUL_NOT_COMPILLED;
byte modulPing =     MODUL_NOT_COMPILLED;
byte modulUpload =   MODUL_NOT_COMPILLED;
byte modulPirs =     MODUL_NOT_COMPILLED;
byte modulContacts = MODUL_NOT_COMPILLED;
byte modulTemp =     MODUL_NOT_COMPILLED;
byte modulElectro =  MODUL_NOT_COMPILLED;
byte modulLeds =     MODUL_NOT_COMPILLED; 
byte modulKeys =     MODUL_NOT_COMPILLED;
byte modulNoo =      MODUL_NOT_COMPILLED;

// design
byte const DEFAULT_DESIGN = 0;
byte const HOME_DESIGN = 1;
byte const MODERN_DESIGN = 2;
byte const HACKER_DESIGN = 3;
byte const PART1_DESIGN = 4;
byte const PART2_DESIGN = 5;
byte const PART3_DESIGN = 6;
byte const PART4_DESIGN = 7;
byte currentDesign = DEFAULT_DESIGN;

// pages
byte const UNKNOWN_PAGE = 0;
byte const THEMES_PAGE = 4;
byte const SETTINGS_PAGE = 5;
byte const PORTS_PAGE = 6;
byte const SUPPLY_PAGE = 7;
byte const ELECTRO_PAGE = 8;
byte currentPage = UNKNOWN_PAGE;

// strings
char buf[200];

// for name serial upload
char fn[20] = "";

// for sample Ajax
boolean LED_state[4] = {0};

// electro
boolean buttonElectro = 0;

// oscill
boolean oscill = false;

// cyclos
unsigned long cyclos = 0;

// timers
unsigned long timeSec; // time in seconds
unsigned long timer1s;  boolean cycle1s = false;
unsigned long timer4s;  boolean cycle4s = false;
unsigned long timer20s; boolean cycle20s = false;
unsigned long timer30s; boolean cycle30s = false;
unsigned long timer1m;  boolean cycle1m = false;
unsigned long timer3m;  boolean cycle3m = false;
unsigned long timer5m;  boolean cycle5m = false;
int startSendTime = 10;

byte const SD_CHIP_SELECT = 4;

#ifdef SD_INFO_FEATURE
  Sd2Card card;
  SdVolume volume;
  SdFile root;
#endif

#ifdef SD_FILES_FEATURE
  File sdRoot;
  File sdDir;
  File entry;
#endif

#ifdef LEDS_FEATURE
  // LED modes
  byte const LED_EMPTY = 0;
  byte const LED_PIR = 1;
  byte modeLED = LED_EMPTY;
#endif

// Serialprint
void StreamPrint_progmem(Print &out,PGM_P format,...) {
  // program memory version of printf - copy of format string and result share a buffer so as to avoid too much memory use
  char formatString[128], *ptr;
  strncpy_P(formatString, format, sizeof(formatString)); // copy in from program mem
  // null terminate - leave last char since we might need it in worst case for result's \0
  formatString[sizeof(formatString) - 2] = '\0'; 
  ptr = &formatString[strlen(formatString) + 1]; // our result buffer...
  va_list args;
  va_start(args, format);
  vsnprintf(ptr, sizeof(formatString) - 1 - strlen(formatString), formatString, args);
  va_end(args);
  formatString[sizeof(formatString) - 1] = '\0'; 
  out.print(ptr);
}

#define Serialprint(format, ...) StreamPrint_progmem(Serial,PSTR(format),##__VA_ARGS__)
#define Streamprint(stream,format, ...) StreamPrint_progmem(stream,PSTR(format),##__VA_ARGS__)

/* setup
-------------------------------------------------- */

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.print(SELF_NAME);
  Serial.println(F(" started..."));

  #ifdef ETHERNET_FEATURE
    ethernetInit();
  #endif

  sdCardInit();

  #ifdef RTC_FEATURE
    rtcInit();
  #endif

  #ifdef SERVER_FEATURE
    serverInit();
  #endif

  timersInit();
  
  #ifdef MAJORDOMO_FEATURE
    majordomoInit();
    majordomoMegaLive();
  #endif
  
  #ifdef LAURENT_FEATURE
    laurentInit();
  #endif
  
  #ifdef SD_INFO_FEATURE
    sdInfoInit();
  #endif

  #ifdef SD_FILES_FEATURE
    sdFilesInit();
  #endif

  #ifdef PING_FEATURE
    pingInit();
  #endif

  #ifdef UPLOAD_FEATURE
    uploadInit();
  #endif

  #ifdef PIRS_FEATURE
    pirsInit();
  #endif

  #ifdef CONTACTS_FEATURE
    contactsInit();
  #endif
 
  #ifdef TEMP_FEATURE
    tempInit();
  #endif
 
  #ifdef ELECTRO_FEATURE
    electroInit();
  #endif
  
  #ifdef KEYS_FEATURE
    keysInit();
  #endif
  
  #ifdef LEDS_FEATURE
    ledsInit();
  #endif

  #ifdef NOO_FEATURE
    nooInit();
  #endif

  timeStamp();
  Serialprint("GLOBAL Init DONE\n");
  Serial.println();
  timeStamp();
  Serialprint("AMS WORK\n");
} // setup

/* loop
-------------------------------------------------- */

void loop() {
  #ifdef UPLOAD_FEATURE
    uploadWorks();
  #endif

  if (modeWork == MODE_SERVER) {
    #ifdef RTC_FEATURE
      rtcWorks();
    #endif     
    
    //prof works
    profStart();
    timersWorks();

    #ifdef SERVER_FEATURE
      serverWorks();
    #endif 

    #ifdef KEYS_FEATURE
      keysWorks();
    #endif  
  
    #ifdef PIRS_FEATURE
      pirsWorks();
    #endif   
  
    #ifdef CONTACTS_FEATURE
      contactsWorks();
    #endif   

    #ifdef TEMP_FEATURE
      if (cycle5m || (timeSec < startSendTime)) {
        tempWorks();
      }
    #endif 
  
    #ifdef ELECTRO_FEATURE
      if (modulElectro == 1) {
        if (cycle20s || (timeSec < startSendTime)) {
          electroWorks();
          freqWorks();
        }
        // network oscilloscope
        if (cycle1s && oscill) {
          freqWorks();
        }
      }
    #endif
  
    #ifdef MAJORDOMO_FEATURE
      majordomoMegaLive();
    #endif

    #ifdef SD_FILES_FEATURE
      if (cycle30s) {
        //printSdContent(sdRoot, 0);
        //cardInfo();
        //Serial.println("Root SD card:");
        //printDirectory(sdDir);
        //Serial.println("/js/");
        //sdDir = SD.open("/js/");
        //printDirectory(sdDir);
      }
    #endif

    #ifdef PING_FEATURE
      if (cycle30s) {
        pingWorks();
      }
    #endif
    
    #ifdef LAURENT_FEATURE
      if (cycle3m) {
        sprintf(buf, "$KE");
        sendLaurentRequest();
      }
    #endif

    cyclos++;

    cyclosInSecWork();
    
    // end loop works
    eraseCyclos();
    profCalc();

    cyclosDelayWork();

  } // if (modeWork == MODE_SERVER)

} // loop


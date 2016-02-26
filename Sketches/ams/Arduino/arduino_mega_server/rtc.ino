/*
  Modul RTC
  part of Arduino Mega Server project
*/

// NTP settings

#ifdef MAJORDOMO_FEATURE
  byte TIME_IP[] = {MAJOR_IP[0], MAJOR_IP[1], MAJOR_IP[2], MAJOR_IP[3]};
  unsigned int TIME_PORT = 8888;  // local port to listen for UDP packets
  const int timeZone = 6;
#else
  byte TIME_IP[] = {129, 6, 15, 30}; // time-c.nist.gov
  unsigned int TIME_PORT = 123;  // alt port
  const int timeZone = 6;
#endif

IPAddress timeServer(TIME_IP);

EthernetUDP Udp;

time_t prevDisplay = 0; // when the digital clock was displayed

void rtcInit() {
  Serialprint("Modul RTC...\n");
  Udp.begin(TIME_PORT);
  if (timeProvider == TIME_NETWORK) {
    Serialprint(" Waiting for NTP sync...\n");
    setSyncProvider(getNtpTime);
  }
  else if (timeProvider == TIME_RTC) {
    Serialprint(" Waiting for RTC sync...\n");
    setSyncProvider(RTC.get); // get time from RTC
  } 
  else {
    Serialprint(" RTC service stopped\n");
  }
  modulRtc = MODUL_ENABLE;
  initDone();
}

void rtcWorks() {
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { // update the display only if time has changed
      setLifer();
      prevDisplay = now();
    }
  }
}

void rtcSync() {
  setSyncProvider(getNtpTime);
  Serialprint("...getNtpTime... \n");
  if (timeStatus() != timeNotSet) {
    Serialprint("...set!... \n");
    time_t t = getNtpTime();
    RTC.set(t);
    setSyncProvider(RTC.get);
  }
}

void printDigits(int digits) {
  if(digits < 10) {
    Serial.print('0');
  }
  Serial.print(digits);
}

void serialRTC() {
  Serial.print(year()); 
  Serial.print("-");
  printDigits(month());
  Serial.print("-");
  printDigits(day());
  Serial.print(" ");
  printDigits(hour());
  Serial.print(":");
  printDigits(minute());
  Serial.print(":");
  printDigits(second());
  //Serial.print(" ");
  //Serial.print(cyclos);
}

void timeStamp() {
  serialRTC();
  Serial.print(" ");
}

void printRTC(){
  serialRTC();
  Serial.println();
}

// NTP code

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

#ifdef RTC_FEATURE

time_t getNtpTime() {
  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serialprint("Transmit NTP request (");
  printIp(TIME_IP);
  Serialprint(":");
  Serial.print(TIME_PORT);
  Serialprint(")\n");
  sendNTPpacket(timeServer);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serialprint("Receive NTP response\n");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serialprint("No NTP response\n");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:                 
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

#endif // RTC_FEATURE

// Duration

void showDuration(time_t duration) {
  // prints the duration in days, hours, minutes and seconds
  timeStamp();
  Serialprint("(duration ");
  if(duration >= SECS_PER_DAY){
     Serial.print(duration / SECS_PER_DAY);
     Serialprint(" day "); 
     duration = duration % SECS_PER_DAY;     
  }
  if(duration >= SECS_PER_HOUR){
     Serial.print(duration / SECS_PER_HOUR);
     Serialprint(" hour "); 
     duration = duration % SECS_PER_HOUR;     
  }
  if(duration >= SECS_PER_MIN){
     Serial.print(duration / SECS_PER_MIN);
     Serialprint(" min "); 
     duration = duration % SECS_PER_MIN;     
  }
  Serial.print(duration);
  Serialprint(" sec)\n");   
}

void checkEvent(time_t* prevEvent) {
  time_t duration = 0;
  time_t timeNow = now();
  
  if (*prevEvent > 0) {
    duration = timeNow - *prevEvent;
  }     
  if (duration > 0) {
    showDuration(duration);
  }  
  *prevEvent = timeNow;
}

// Uptime

char uptimeString[14];

/* ----------------------------------------------------------
  Function makeUptimeString(char z[], char nop[])
    convert system millis() to formstted string days hh.mm.ss
    Used globsl variable char uptimeString[14];
    Auto reset ~50 days
    TODO: save to EEPROM x50 days periods
------------------------------------------------------------- */

void makeUptimeString(char zero[], char nop[]) {
  char* zh;
  char* zm;
  char* zs;
  
  unsigned long totalSeconds = millis() / 1000;
  unsigned long days = totalSeconds / 86400;
  unsigned long tsHours = totalSeconds - days * 86400;
  unsigned long hours = tsHours / 3600;
  unsigned long tsMinutes = tsHours - hours * 3600;
  unsigned long minutes = tsMinutes / 60;
  unsigned long seconds = tsMinutes - minutes * 60;

  if (hours   < 10) {zh = zero;} else {zh = nop;}
  if (minutes < 10) {zm = zero;} else {zm = nop;}
  if (seconds < 10) {zs = zero;} else {zs = nop;}

  sprintf(uptimeString, "%d %s%d:%s%d:%s%d", (int)days, zh, (int)hours, zm, (int)minutes, zs, (int)seconds);
}

// Lifer

byte lifer;

void setLifer() {
  lifer++;
  if (lifer > 6) {
    lifer = 0;
  }
}


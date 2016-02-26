/*
  Modul nooLite
  part of Arduino Mega Server project
*/

#ifdef NOO_FEATURE

byte const PIN_TX = 18; // TX PIN (to RX noolite)
byte const PIN_RX = 19; // RX PIN (to TX noolite)

byte const NOO_CHANNEL_1 = 0; // channel (address) 0...31 (MT1132)
byte const NOO_CHANNEL_2 = 1;
byte const NOO_CHANNEL_3 = 2;
byte const NOO_CHANNEL_4 = 3;
byte const NOO_CHANNEL_5 = 4;

void nooInit() {
  Serial1.begin(9600);
  modulNoo = MODUL_ENABLE;
  started("nooLite");
}

void nooWork() {

}

void nooSendCommand(byte channel, byte command, byte data, byte format) {
  byte buf[12];

  for (byte i = 0; i < 12; i++) {
    buf[i] = 0;
  }

  buf[0] = 85;
  buf[1] = B01010000;
  buf[2] = command;
  buf[3] = format;
  buf[5] = channel;
  buf[6] = data;

  int checkSum = 0;
  for (byte i = 0; i < 10; i++) {
    checkSum += buf[i];
  }

  buf[10] = lowByte(checkSum);
  buf[11] = 170;

  for (byte i = 0; i < (12); i++) {
    Serial1.write(buf[i]);
  }
  /*
  Serial.println(">");
  for (byte i = 0; i < (12); i++) {
    Serial.print(buf[i]);
  }
  Serial.println();
  */
}

//                                     command data format
void nooBind   (byte ch) {nooSendCommand(ch, 15, 0, 0);}
void nooUnbind (byte ch) {nooSendCommand(ch,  9, 0, 0);}

void nooOn     (byte ch) {nooSendCommand(ch,  2, 0, 0);}
void nooOff    (byte ch) {nooSendCommand(ch,  0, 0, 0);}
void nooTrigger(byte ch) {nooSendCommand(ch,  4, 0, 0);}
void nooCancel (byte ch) {nooSendCommand(ch, 10, 0, 0);}

void nooUp     (byte ch) {nooSendCommand(ch,  3, 0, 0);}
void nooDown   (byte ch) {nooSendCommand(ch,  1, 0, 0);}
void nooRevers (byte ch) {nooSendCommand(ch,  5, 0, 0);}

void nooValue  (byte ch, byte v) {nooSendCommand(ch,  6, v, 1);}

#endif // NOO_FEATURE

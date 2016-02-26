/* ----------------------------------------------
  Modul Send HTTP request for Due
  part of Arduino Mega Server project
------------------------------------------------- */

#ifdef SEND_FEATURE

// target net settings
byte TARGET_IP[] = {192, 168, 2, 8};
int TARGET_PORT = 80;

// EthernetClient object
EthernetClient tclient;

void sendInit() {
  initStart("Send");
  modulSend = MODUL_ENABLE;
  initDone();
}

void sendHttpRequest(byte ip[], int port, EthernetClient cl) {
  if (cl.connect(ip, port)) { 
    timeStamp();
    Serial.print("Host ");
    printIp(ip);
    Serial.print(" ");
    Serial.println(buf);
    cl.println(buf);
    cl.print("Host: ");
    sendIpClient(SELF_IP, cl);
    cl.println();
    cl.println(); 
    delay(100);
    cl.stop();
  } else {
      timeStamp();
      Serial.print("Host ");
      printIp(ip);
      Serial.print(" not connected (");
      Serial.print(buf);
      Serial.println(")");
    }
}

void sendRequest(byte ip[], int port, char object[], int value, EthernetClient cl) {
  sprintf(buf, "GET /?%s=%d", object, value); 
  sendHttpRequest(ip, port, cl);
}

#endif // SEND_FEATURE

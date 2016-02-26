/*
  Modul Pages Maker
  part of Arduino Mega Server project
*/

File tempFile;

void openBlockStyle(EthernetClient cl) {
  cl.println();
  cl.println("<style>");
}

void closeBlockStyle(EthernetClient cl) {
  cl.println("</style>");
  cl.println();
}

void openStyle(char style[], EthernetClient cl) {
  cl.print("  .");
  cl.print(style);
  cl.println(" {");
}

void closeStyle(EthernetClient cl) {
  cl.println("  }");
}

void insertIpAttr(char first[], char last[], EthernetClient cl) {
  cl.print("    ");
  cl.print(first);
  sendIpClient(SELF_IP, cl);
  cl.println(last);
}

void addStyleHome(EthernetClient cl) {
  openBlockStyle(cl);
    openStyle("home", cl);
      insertIpAttr("background: url(//", "/home.jpg) no-repeat;", cl);
    closeStyle(cl);
  closeBlockStyle(cl);
}

void addStyleModern(EthernetClient cl) {
  openBlockStyle(cl);
    openStyle("modern", cl);
      insertIpAttr("background: url(//", "/modern.jpg) no-repeat;", cl);
    closeStyle(cl);
  closeBlockStyle(cl);
}

void insertBlock(uint8_t typeOperation, EthernetClient cl) {
  if (typeOperation == TYPE_LINKS) {
    if (modeNetWork == MODE_ONE) {
      switch (currentDesign) {
        case HOME_DESIGN:   tempFile = SD.open("_one_hm.htm"); addStyleHome(cl); break;
        case MODERN_DESIGN: tempFile = SD.open("_one_md.htm"); addStyleModern(cl); break;
        case HACKER_DESIGN: tempFile = SD.open("_one_hk.htm"); break;
        case PART1_DESIGN:  tempFile = SD.open("_one_p1.htm"); break;
        case PART2_DESIGN:  tempFile = SD.open("_one_p2.htm"); break;
        case PART3_DESIGN:  tempFile = SD.open("_one_p3.htm"); break;
        case PART4_DESIGN:  tempFile = SD.open("_one_p4.htm"); break;
        default: tempFile = SD.open("_one.htm");
      }
    }
    else if (modeNetWork == MODE_MAJOR) {
      switch (currentDesign) {
        case HOME_DESIGN:   tempFile = SD.open("_maj_hm.htm"); addStyleHome(cl); break;
        case MODERN_DESIGN: tempFile = SD.open("_maj_md.htm"); addStyleModern(cl); break;
        case HACKER_DESIGN: tempFile = SD.open("_maj_hk.htm"); break;
        case PART1_DESIGN:  tempFile = SD.open("_maj_p1.htm"); break;
        case PART2_DESIGN:  tempFile = SD.open("_maj_p2.htm"); break;
        case PART3_DESIGN:  tempFile = SD.open("_maj_p3.htm"); break;
        case PART4_DESIGN:  tempFile = SD.open("_maj_p4.htm"); break;
        default: tempFile = SD.open("_major.htm");
      }
    }
  }
  else if (typeOperation == SCRIPTS_JS) {
    if (modeNetWork == MODE_ONE) {
      tempFile = SD.open("scripts.js");
    }
  }
  else if (typeOperation == HEADER) {
    switch (currentDesign) {
      case HOME_DESIGN:   tempFile = SD.open("_head_hm.htm"); break;
      case MODERN_DESIGN: tempFile = SD.open("_head_md.htm"); break;
      case HACKER_DESIGN: tempFile = SD.open("_head_hk.htm"); break;
      case PART1_DESIGN:  tempFile = SD.open("_head_p1.htm"); break;
      case PART2_DESIGN:  tempFile = SD.open("_head_p2.htm"); break;
      case PART3_DESIGN:  tempFile = SD.open("_head_p3.htm"); break;
      case PART4_DESIGN:  tempFile = SD.open("_head_p4.htm"); break;
      default: tempFile = SD.open("_header.htm");
    }
  }  
  else if (typeOperation == DASH) {
    switch (currentDesign) {
      case HOME_DESIGN:   tempFile = SD.open("_dash_hm.htm"); break;
      case MODERN_DESIGN: tempFile = SD.open("_dash_md.htm"); break;
      case HACKER_DESIGN: tempFile = SD.open("_dash_hk.htm"); break;
      case PART1_DESIGN:  tempFile = SD.open("_dash_p1.htm"); break;
      case PART2_DESIGN:  tempFile = SD.open("_dash_p2.htm"); break;
      case PART3_DESIGN:  tempFile = SD.open("_dash_p3.htm"); break;
      case PART4_DESIGN:  tempFile = SD.open("_dash_p4.htm"); break;
      default: tempFile = SD.open("_dash.htm");
    }    
  }
  else if (typeOperation == MENU) {
    switch (currentDesign) {
      case HOME_DESIGN:   tempFile = SD.open("_menu_hm.htm"); break;
      case MODERN_DESIGN: tempFile = SD.open("_menu_md.htm"); break;
      case HACKER_DESIGN: tempFile = SD.open("_menu_hk.htm"); break;
      case PART1_DESIGN:  tempFile = SD.open("_menu_p1.htm"); break;
      case PART2_DESIGN:  tempFile = SD.open("_menu_p2.htm"); break;
      case PART3_DESIGN:  tempFile = SD.open("_menu_p3.htm"); break;
      case PART4_DESIGN:  tempFile = SD.open("_menu_p4.htm"); break;
      default: tempFile = SD.open("_menu.htm");
    }    
  }
  else if (typeOperation == BANNER_BOTTOM) {
    long rnd = random(1, 15);
    if (rnd == 5) {
      tempFile = SD.open("_banner.htm");
    }
  }  
  else if (typeOperation == FOOTER) {
    switch (currentDesign) {
      case HOME_DESIGN:   tempFile = SD.open("_foot_hm.htm"); break;
      case MODERN_DESIGN: tempFile = SD.open("_foot_md.htm"); break;
      case HACKER_DESIGN: tempFile = SD.open("_foot_hk.htm"); break;
      case PART1_DESIGN:  tempFile = SD.open("_foot_p1.htm"); break;
      case PART2_DESIGN:  tempFile = SD.open("_foot_p2.htm"); break;
      case PART3_DESIGN:  tempFile = SD.open("_foot_p3.htm"); break;
      case PART4_DESIGN:  tempFile = SD.open("_foot_p4.htm"); break;
      default: tempFile = SD.open("_footer.htm");
    }    
  }
  else if (typeOperation == FLOTR2_JS) {
    tempFile = SD.open("flotr2.js");
  }
  else if (typeOperation == SELF_ADDRESS) {
    sendIpClient(SELF_IP, cl);
  }
  else if (typeOperation == SAMPLE2_ONE) {
    if (modeNetWork == MODE_ONE) {
      tempFile = SD.open("sample2.js");
    }
  }  
  else if (typeOperation == SAMPLE2_MAJOR) {
    if (modeNetWork == MODE_MAJOR) {
      tempFile = SD.open("_sample2.htm");
    }
  }
  
  if (tempFile) {
    while(tempFile.available()) {
      rsize2 = tempFile.read(buff2, MAX_BUFFER_SIZE2);
      cl.write(buff2, rsize2);
    }
    tempFile.close();
  }
}

// readSerial for reading messeges received from Python
// ============================================================



struct COMMUNICATION readSerial(struct COMMUNICATION communication,
                                int maxEmptyRead = 300,
                                int wait = 10,
                                int minWait = 1) {
                                  
  int n         = -1;
  bool ping     = false;
  bool stop     = false;
  int emptyRead = 0;
  int    incomingByte = 0;
  char   serialData[2000];
  String msgHeader = "";


  while (!stop) {
    incomingByte = Serial.read();

    // stop iteration limit
    if (emptyRead > maxEmptyRead) {
      stop = true;
    }

    // '*' Ping Detection: clean the serial, reply to the ping, stop the loop
    // ----------------------------------------------------------------------
    if (incomingByte == 42) {
      while(Serial.available() > 0) {
        incomingByte = Serial.read();
      }
      Serial.println("*");
      delay(wait);
      incomingByte = 0;
      ping = true;
      stop = true;
      communication.ping = true;
    }
    
    // if '<':'60' end of data detection
    // ---------------------------------
    else if (incomingByte == 60) {
      stop = true;
    }
    
    // Check whether byte was not read
    else if (incomingByte >= 0) {
      n = n+1;
      serialData[n] = incomingByte;
      delay(minWait);
      emptyRead = 0;
    } else {
      delay(minWait);
      emptyRead = emptyRead + 1;
    }
  }
  //Serial.println(serialData);

  

  // if not a ping, process the received data
  // ========================================
  if (!ping) {
    char* ptr = strtok(serialData, "$");
    msgHeader = String(ptr);

    if (DEBUG) {
      Serial.print("HEADER: ");
      Serial.println(msgHeader);
    }
    
    if (msgHeader == "@LOG") {
      LOG = true;
      Serial.println("LOGED");
    } else if (msgHeader == "@BOO") {
      communication.booted = true;
      Serial.println("BOOTED");
    } else if (msgHeader == "@BYE") {
      communication.shutdown = true;
      Serial.println("SHUTDOWN");
    } else if (msgHeader == "@MSF") {
      MSF[99];
    } else if (msgHeader == "@REV") {
      REV[99];
    } else if (msgHeader == "@PRS") {
      PRS[99];
    } else if (msgHeader == "@PLY") {
      PLY[99];
    } else if (msgHeader == "@PL0") {
      playStatus = false;
      refreshDisplay = true;
    } else if (msgHeader == "@ERR") {
      ERR[3];
      errScreen      = true;
      refreshDisplay = true;
    }
    
    // break the message
    ptr = strtok(NULL, "$");
    
    if (msgHeader == "@MSF") MSF[0] = "";
    else if (msgHeader == "@REV") REV[0] = "";
    else if (msgHeader == "@PRS") PRS[0] = "";
    else if (msgHeader == "@PLY") PLY[0] = "";
    else if (msgHeader == "@ERR") ERR[0] = "";
    else if (msgHeader == "@WAT") waitScreen = true;

    // LOOP and update the array
    // ============================================
    byte i = 1;
    while (ptr) {
      if (msgHeader == "@MSF") {
        MSF[i] = ptr;
        MsfArrayLength = i;  //update the MSFlength
        //Serial.println(MSF[i]);
      }
      else if (msgHeader == "@PRS") {
        PRS[i] = ptr;
        PrsArrayLength = i;  //update the preset length
      }
      else if (msgHeader == "@REV") {
        REV[i] = ptr;
        RevArrayLength = i;  //update the preset length
      } 
      else if (msgHeader == "@PLY") {
        PLY[i] = ptr;
        PlyArrayLength = i;  
      } 
      else if (msgHeader == "@WAT") {
        waitingTime = int(ptr); 
      } 
      else if (msgHeader == "@ERR") {
        ERR[i] = ptr; 
      }
      ptr = strtok(NULL, "$");
      i++;
    }

    // update and return the struct
    // ==================================================
    communication.header = msgHeader;
  }



  // Clean the serialData for each read
  short acidClean = 0;
  memset(serialData, acidClean, 2000);
  return communication;
}

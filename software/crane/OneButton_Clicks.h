// Functions for the rotatory shift encoder
// ============================================================

int getEncoderTurn(void) {
  static int oldA = HIGH; //set the oldA as HIGH
  static int oldB = HIGH; //set the oldB as HIGH
  int result = 0;
  int newA = digitalRead(clkPin);//read the value of clkPin to newA (direction forward)
  int newB = digitalRead(dtPin); //read the value of dtPin to newB (direction backward)

  /*
   * the shift encoder has multiple bouncing, with some backward bouncing 
   * happenning even 25ms after the movement. Therefore, I apply the the 
   * delay twice to make sure the device does not read the second bounce 
   * as a new movement
   * 
   * In each single movement, there are 3 changing states. newA and newB change
   * based on directions. 
   * 
   * States
   * ------
   * 
   * 1. half movement. if backward, newB turns 0. if Forward, newA turns 0
   * 2. complete move, with a falling click in the shift encoder, where both
   *    newA and newB become 0
   * 3. stabilization, where both become 1.
  */
  
  if (newA != oldA || newB != oldB) //if the value of clkPin or the dtPin has changed
  {
    // something has changed
    delay(3); //remeasure after a quick debouncing
    int newA = digitalRead(clkPin);//read the value of clkPin to newA
    int newB = digitalRead(dtPin); //read the value of dtPin to newB
    
    if (newA != oldA || newB != oldB) {

      /*
      Serial.print("newA:");
      Serial.print(newA);
      Serial.print(" ");
      Serial.print("newB:");
      Serial.println(newB);

      // delay on stabilizations (both are zero)
      if (newA == newB && newA == LOW) {
        //delay(5);
      }
      // delay on stabilizations (both are one)
      if (newA == newB && newA == HIGH) {
        delay(50);
      }
      */
    
      if (oldA == HIGH && newA == LOW){
        result = (oldB * 2 - 1);
      }
      oldA = newA;
      oldB = newB;
      //delay(25);
    }
  }
  return result; 
}


void singleclick(){                                 // what happens when the button is clicked
 //digitalWrite(12,HIGH);                            // light the red LED
 //delay(250);                                       // wait one second
 //digitalWrite(12,LOW);                              // turn off the gren led
 SerialUSB.println("sClick!");
 sClick = true;
}

void doubleclick() {                                // what happens when button is double-clicked
 //digitalWrite(11,HIGH);                             // light the green LED
 //delay(250);                                       // wait one second
 //digitalWrite(11,LOW);                              // turn off green LED

 SerialUSB.println("dClick");
 dClick = true;
}


// longClick is similar to longPressStop1
/*
void longclick(){                                   // what happens when buton is long-pressed
 //digitalWrite(13,HIGH);                            // light the blue LED
 //delay(250);                                       // wait one second
 //digitalWrite(13,LOW);                              // turn off the blue LED
 
 pClick = true;
}
*/

// This function will be called once, when the button1 is pressed for a long time.
void longPressStart1() {
  SerialUSB.println(">FN:");
} // longPressStart1


// longPress1
void longPress1() {
  //Serial.println(">FN:");
} 


// This function will be called once, when the button1 is released after beeing pressed for a long time.
void longPressStop1() {
  SerialUSB.println(">ND:");
} // longPressStop1










// Functions for the 5-button sets, 3-button sets, and a single button
// ============================================================
int singleButton(int PIN, bool DEBUG=false) {
  // AVOID NOISE BY RESAMPLING
  //delay(5);
  int ARRAY[451];
  // this button reads around 720 when it is pressed
  if (analogRead(PIN) > 100) {
    for (int i = 0; i < 451; i++) {
      //buttonValue = (analogRead(PIN)+buttonValue)/2;
      //delay(1);
      ARRAY[i] = analogRead(PIN);
    }
  
    int median = QuickMedian<int>::GetMedian(ARRAY, 451); //array, its length
    if (median>=350) {
      if (DEBUG) SerialUSB.println(ARRAY[25]);
      dClick = true;
    }
  }
}


// USED FOR THE ROTATION MENU
int fiveButton(int PIN, bool DEBUG=false) {
  int ARRAY[451];
  int result = 0;
  

  // on default this button combo would read around 720 with a 3.3v. So I read for less than 700 to initiate
  if (analogRead(PIN) < 700) {
    
    for (int i = 0; i < 451; i++) {
      ARRAY[i] = analogRead(PIN);
    }

    int median = QuickMedian<int>::GetMedian(ARRAY, 451); //array, its length
    
    if (median<=13) {
      if (DEBUG) SerialUSB.println("LEFT0");
      left0 = HIGH;
    }
    else if (median>=14 && median<=30){
      if (DEBUG) SerialUSB.println("UP0");
      up0 = HIGH;
      result = -1;
    }
    else if (median>=45 && median<=75){
      if (DEBUG) SerialUSB.println("DOWN0");
      down0 = HIGH;
      result = 1;
    }
    else if (median>=95 && median<=120){
      if (DEBUG) SerialUSB.println("RIGHT0");
      right0 = HIGH;
    }
    else if (median>=200 && median<=300) {
      if (DEBUG) SerialUSB.println("sClick!");
      sClick = true;
    } 
  }

  return result; 
}

void threeButton1(int PIN, bool DEBUG=false) {
  int ARRAY[451];
  if (analogRead(PIN) < 700) {
    for (int i = 0; i < 451; i++) {
      ARRAY[i] = analogRead(PIN);
    }
    int median = QuickMedian<int>::GetMedian(ARRAY, 451); //array, its length
    if (median<=10) {
    if (DEBUG) SerialUSB.println("instrumentButton");
      instrumentButton = HIGH;
    }
    else if (median>=95 && median<=120){
      if (DEBUG) SerialUSB.println("reverbButton");
      reverbButton = HIGH;
    }
    else if (median>=200 && median<=300) {
      if (DEBUG) SerialUSB.println("tuningButton");
      tuningButton = HIGH;
    }
  }
}

void threeButton2(int PIN, bool DEBUG=false) {
  int ARRAY[451];
  if (analogRead(PIN) < 700) {
    for (int i = 0; i < 451; i++) {
      ARRAY[i] = analogRead(PIN);
    }

    int median = QuickMedian<int>::GetMedian(ARRAY, 451); //array, its length


    // RECORD BUTTON
    // -------------
    if (median<=10) {
      if (DEBUG) SerialUSB.println("recordButton");

      // only activate this option if the piano is not already recording
      if (!recordStatus) {
        recordButton = HIGH;
        recordStatus = true;

        // jump to the record button in the menu (which is number 3)
        menu.page    = 3;
        menu.item    = 0;
        menu.refresh = false;
        itemsLoaded  = false;
        menu = displayMenu(menu=menu,
                        rotation=rotation,
                        sClick=sClick,
                        dClick=dClick,
                        pClick=pClick,
                        massage=massage,
                        refreshMENU=false);

        // CODE FOR 'RECORDING' File IS '>RC:'
        COMMAND = String(">RC:") + recordStatus + String("\r\n");
        //Serial.print(">RC:");
        //Serial.print(recordStatus);
        SerialUSB.println(COMMAND);
        //Serial.print("\n");
        
        commandSent = true;
        commandConfirmed = false;
        delay(10);
      }
    }

    // PLAY / PAUSE BUTTON
    // -------------------
    else if (median>=95 && median<=120){
      if (DEBUG) SerialUSB.println("play/pause Button");
      

      // PAUSE BUTTON
      if (recordStatus) {
        recordButton = LOW;
        recordStatus = false;
        pauseButton  = HIGH;

        // jump to the record button in the menu (which is number 3)
        menu.page    = 3;
        menu.item    = 0;
        //menu.refresh = false;
        itemsLoaded  = false;
        menu = displayMenu(menu=menu,
                        rotation=rotation,
                        sClick=sClick,
                        dClick=dClick,
                        pClick=pClick,
                        massage=massage,
                        refreshMENU=false);

        // CODE FOR 'PAUSING' File IS '>RC:0'
        COMMAND = String(">RC:") + recordStatus + String("\r\n");
        SerialUSB.println(COMMAND);
        commandSent = true;
        commandConfirmed = false;
        
        //Serial.print(">RC:");
        //Serial.println(recordStatus);
        //Serial.print("\n");
        delay(10);
      }

      // OTHERWISE ACTIVATE THE PLAY BUTTON, IF NOTHING IS BEING PLAYED NOW
      else {
        playButton = HIGH;
      }
      
    }
    else if (median>=200 && median<=300) {
      if (DEBUG) SerialUSB.println("settingsButton");
      settingsButton = HIGH;
      delay(250);
    }
  }
}

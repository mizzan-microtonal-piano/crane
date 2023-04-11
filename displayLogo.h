// logo
// ============================================================

// COLOR PICKER: <http://www.barth-dev.de/online/rgb565-color-picker/>

// Interesting stuff
// =====================
//lcdTestPattern();

#define boxShadow  0xB080 
#define boxBody    RED    
#define textShadow 0xF2C8 
#define textBody   BLACK

// STYLE 3 --------------

#define textShadow3 0x124A //0x3495
#define textBody3   0x01C8 //0x132F
#define boxShadow3B 0x0062
#define boxShadow3  0x00E4 //0x0126
#define boxBody3    0x0062 //0x0126

//STYLE 4 -------------------------
float style4Colors[] = {0x2B0E,0x4BB1,0x4BB1,0x4370,0x4370,0x3B2F,0x3B2F,0x3AEE,0x3AEE,0x32AC,0x32AC,0x328B,0x328B,0x328B,0x328B,0x2A2A,0x2A2A,0x21E9,0x21E9,0x21C8,0x21C8,0x01C8};
float style4SHadows[]= {0x5412,0x22CD,0x22AC,0x22AC,0x226B,0x226B,0x1A2A,0x1A2A,0x19E9,0x19E9,0x11C8,0x11C8,0x11A7,0x11A7,0x11A7,0x11A7,0x1167,0x1167,0x1146,0x1146,0x0904,0x0106};                       
int   style4pause[]  = {2000,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};

//SCREEN.setFont(&FreeMono12pt7b);
//SCREEN.setFont(&Picopixel);


// START DISPLAY
// ======================================================================
void displayStart(String title="mizzan\n", int X=17, int Y=130, String subtitle="booting...\n", int x=100, int y=200, int duration=9000) {
  //TITLE
  SCREEN.fillScreen(BLACK); 
  SCREEN.setFont(&UnifrakturMaguntia_Regular44pt7b);
  SCREEN.setTextSize(1);
  SCREEN.setTextColor(style4Colors[1]);
  SCREEN.setCursor(X,Y);             // Start at top-left corner
  SCREEN.print(title); 

  //SUBTITLE
  SCREEN.setFont();
  SCREEN.setTextSize(2); 
  SCREEN.setTextColor(style4Colors[1]);
  SCREEN.setCursor(x,y);
  SCREEN.print(subtitle); 
  delay(duration);
}




// WAITING SCREEN
// ======================================================================
void processScreen(float wait=3000,
                   String subtitle="processing...\n", int x=88, int y=200) {

  //> when the `wait` variable was int, for values above 30000 it was not being passed properly inside the function and sometimes the value was -80 instead! 
  //>  changing the variable to float solved the bug

  SCREEN.fillScreen(BLACK); 
  SCREEN.setTextSize(3);

  int X1 = 40;
  int X2 = 240;
      
  SCREEN.fillRoundRect(X1-1, 109, X2+1, 22, 0, 0x0062);

  //SUBTITLE
  SCREEN.setFont();
  SCREEN.setTextSize(2); 
  SCREEN.setTextColor(style4Colors[1]);
  SCREEN.setCursor(x,y);
  SCREEN.print(subtitle); 

  int Len = (sizeof(style4pause)/ sizeof(style4pause[0]));
  int steps = int(310/Len) + 2;
  int j = 0; 
  for (int i = 0; i < (X2); i++) {
    //if (changeColor) {
    //  if (i % steps == 0) j = j+1;  // update color for every 10 iterations
    //  SCREEN.fillRoundRect(5, 110, i, 20, 0, style4Colors[Len-j]); //0x0062
    //} 
    SCREEN.fillRoundRect(X1, 110, i, 20, 0, style4Colors[1]); //0x0062

    //there is a lot of killed time in between waiting s, that's taken by the monitor!
    // therefore, assuming linear waiting time, this procedure can be slowed down
    //// int waitingTime = round(wait/240) - 10 ; //deactivated for crane board due to unknown waiting time
    //// delay(waitingTime); 

  }
}



// ERROR DISPLAY
// ======================================================================
void displayErr(int style=1, String num="0", int duration=3000) {
  
  if (style == 1) {
    // FRAME
    SCREEN.fillScreen(BLACK); 
    SCREEN.drawRect(0, 0, 320, 240, 0x3495);   //color 5             //x,y,width,hight,color
    SCREEN.drawRect(1, 1, 319, 239, 0x3495);   //color 5             //x,y,width,hight,color

    // title
    //SCREEN.fillRoundRect(2, 2, 166, 35, 2, 0x3495);
    SCREEN.fillRoundRect(4, 4, 313, 35, 2, 0x3495);
    ////SCREEN.fillRoundRect(0, 0, 320, 35, 2, 0x3495);
    
    SCREEN.setTextColor(BLACK, WHITE);               // Draw 'inverse' text
    SCREEN.setTextSize(3);                           // Draw 3X-scale text
    SCREEN.setCursor(7,10);                          // Start at top-left corner
    SCREEN.setTextColor(BLACK, 0x3495);              // Draw 'inverse' text
    String title = "Error " + num; 
    SCREEN.print(title);

    // Error message
    SCREEN.setTextSize(2); 
    SCREEN.setTextColor(0x3495);

    // print the error message
    errors(num);
    
    delay(duration);
  }
}






// LOGO DISPLAY 
// ======================================================================
void displayLogo(bool Loop=true, int style=2, String txt="quartone\n", int duration=9999) {
  // `txt` represents `F("text...\n")` 

  if (style == 1) {
    SCREEN.fillScreen(BLACK); 
    SCREEN.fillRoundRect(0, 53, 320, 27, 5, 0x132F); //x1,y1,x2,y2, cornerSoftness
    SCREEN.setTextColor(BLACK, WHITE);               // Draw 'inverse' text
    SCREEN.setTextSize(3);                           // Draw 3X-scale text
    SCREEN.setCursor(1,55);                          // Start at top-left corner
    SCREEN.setTextColor(BLACK, 0x3495);              // Draw 'inverse' text
    SCREEN.print(txt);
    SCREEN.setTextSize(1); 
    SCREEN.setCursor(50,84);
    SCREEN.setTextColor(0x3495);
    SCREEN.print(F("QT-122\n")); 
  }

  if (style == 2) {

    SCREEN.fillScreen(BLACK); 
    SCREEN.fillRoundRect(0, 45, 128, 48, 2, boxShadow);
    SCREEN.fillRoundRect(2, 47, 126, 44, 2, boxBody);
    SCREEN.setCursor(1,55);             // Start at top-left corner
    SCREEN.setTextColor(textShadow);
    SCREEN.setTextSize(3);
    SCREEN.print(txt); 
    SCREEN.setCursor(2,55);             // Start at top-left corner
    SCREEN.setTextColor(textBody);
    SCREEN.print(txt); 
  }

  if (style == 3) {
    SCREEN.fillScreen(BLACK); 
  }

  if (style == 4) {
    SCREEN.fillScreen(BLACK); 
    SCREEN.setTextSize(3);
    SCREEN.fillRoundRect(0, 53, 128, 5, 0, 0x00104); //x1,y1,x2,y2, cornerSoftness
    SCREEN.fillRoundRect(0, 55, 127, 10, 2, 0x00E4); //0x00E4
    SCREEN.fillRoundRect(0, 65, 127, 12, 2, 0x00A4); //0x00A4
    SCREEN.fillRoundRect(0, 77, 128, 1, 0, 0x0062);

    // run a loop based on the length of the color and delay, which should be of equal size
    int Len = (sizeof(style4pause)/ sizeof(style4pause[0]));
    int dur = 0;
    for (int i = 0; i < Len; i++) {
      //SCREEN.setFont(&FreeMono12pt7b);
      //SCREEN.setFont(&Picopixel);
      if (dur <= duration) {
        SCREEN.setCursor(1,55);             // Start at top-left corner
        SCREEN.setTextColor(style4SHadows[i]);  //0x01C8
        SCREEN.print(txt); 
        SCREEN.setCursor(2,54);             // Start at top-left corner
        SCREEN.setTextColor(style4Colors[i]);  //0x0127
        SCREEN.print(txt); 
        delay(int(style4pause[i]));
      } else {
        // turn the screen off when the duration is passed
        SCREEN.fillScreen(BLACK); 
      }
      dur = dur + int(style4pause[i]);
    }
  }
}

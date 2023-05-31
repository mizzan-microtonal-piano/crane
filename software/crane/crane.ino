

/*
 This program is written for Adafruit_ST7789 library, which powers the
 2" LCD screen. 

 All rights reserved
 
 Haghish UG
 2019
 */

/*
 *  warning
 *  
 *  in Feb 2019, the PLY[], MSF[] variables had empty entries which was ONLY 
 *  due to limited memory of Arduino and reducing their size from 99 to 32
 *  solved the issue! does it mean I should add memory card to arduino?
 */


/* 
REQUIRED UPDATES 
============================================================

1. sClick, dClick, and pClick should be returned by the func
   instead of a global variable
*/

/*
 * Integer equivalent of characters
 * ================================
 * <https://en.wikipedia.org/wiki/ASCII>
 * !: 33
 * #: 35
 * $: 36
 * %: 37
 * *: 42  #used for sending and receiving a ping
 * <: 60
 * >: 62
 * @: 64
 * |: 124
 */

// EXTERNAL LIBRARIES
// ============================================================

//#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

#include <Adafruit_GFX.h>           // Adafruit GFX library v. 1.7.5
//#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <gfxfont.h>                // Adafruit GFX library v. 1.7.5
#include <Adafruit_ILI9341.h>      // Hardware-specific library for ST7789
//#include <EEPROM.h>                 // for writing/reading configs


#include <Wire.h>
////#include <OneButton.h>              // OneButton (reading the push buttons) v. 1.4.0
////#include "QuickMedianLib.h"         // version 1.1.1 for measuring median 
#include "buttonScan.h"             // code for scanning Emon's key matrix

//void buttonHandler(); // use this to define a function that is defined at the buttom of the file

// Libraries written by Mizzan
// ------------------------------------------------------------

// Font
// ------------------------------------------------------------
#include "UnifrakturMaguntia_Regular44pt7b.h"

//fonts from Adafruit_SSD1306 examined for the Logo (currently are not used)
//#include <Fonts/FreeMonoBoldOblique12pt7b.h>
//#include <Fonts/Picopixel.h>
//#include <Fonts/FreeMono12pt7b.h>

// ON WINDOWS <UnifrakturMaguntia_Regular44pt7b.h> this did not specify the path correctly!
//#include <UnifrakturMaguntia_Regular44pt7b.h>
//#include <C:\Users\mizzan\Documents\GitHub\PianoMenu19\Arduino\006_Menu-LCD-320x240/UnifrakturMaguntia_Regular44pt7b.h>


// Adafruit TFT screen
// ------------------------------------------------------------
#define TFT_DC PC_4
#define TFT_CS PA_4
#define TFT_BL PB_0
#define TFT_MOSI PA_7
#define TFT_CLK PA_5
#define TFT_RST PC_5
#define TFT_MISO PA_6

//SPISettings mySetting(40000000, MSBFIRST , SPI_MODE0);

// GLOBAL VALIABLES
// ============================================================
//int memory;
//float pitch_memory = 64; 
int massage = 0;

// rotatory setting
// ============================================================
/*
const int clkPin= 2;  //the clk attach to pin 2
const int dtPin= 3;   //the dt pin attach to pin 3
const int swPin= 4;   //the sw pin attach to pin 4
*/

// define single click, double click, and long-push click
// ============================================================
int sClick   = false;
int dClick   = false;
int pClick   = false;
int refreshMENU = false;

////OneButton button1(4, true);    //attach a button on pin A1 to the library
int encoderVal = 1;
int rotation = 0;

// button sets setting
// ============================================================
int back = LOW;
int up0 = LOW;
int down0 = LOW;
int left0 = LOW;
int right0 = LOW;
int out0 = LOW;

int instrumentButton = LOW;
int reverbButton = LOW;
int tuningButton = LOW;
int recordButton = LOW;
int pauseButton = LOW;
int playButton = LOW;
int settingsButton = LOW;

// configuration banks
// ============================================================
//int eeAddress = 0;   //Location we want the data to be put.

// Display
// ============================================================
bool shutdown       = false;
bool refreshDisplay = false;
bool waitScreen     = false;
bool errScreen      = false;

// NOTE: the screen suffers from flickering. Try to avoid rebuilding the whole
// screen after each move, and only update the items. To do so, I define
// two identifiers to specify whether pages or items (2nd level) is booted
boolean pagesLoaded = false;
boolean itemsLoaded = false;
// to force reboot the screen, specify them both to be 'false' right before calling 
// the screen


// COMMUNICATION Global variables received from SERVER
// ============================================================
int TIME              = 0;
String MSF[99];
String PRS[32];
String REV[32];
String PLY[99];
String ERR[3];
bool LOG = false;         //boolean used for displaying the logoScreen
String errorMsg;
int MsfArrayLength    = 0;
int PrsArrayLength    = 0;
int RevArrayLength    = 0;
int PlyArrayLength    = 0;
int waitingTime       = 0;
String COMMAND;
bool commandSent      = false;
bool commandConfirmed = false;
bool commandWaiting   = false;
int  commandWaitingTime= 0;
//int RevArrayLength = (sizeof(REV)/ sizeof(REV[0])) - 1; // BECAUSE IT IS RECEIVED FROM PYTHON AS WELL

// MAIN SCREEN ITEMS
// ============================================================
boolean reverbStatus = false;
boolean recordStatus = false;
boolean playStatus   = false;

// COMMUNICATION STRUCT
// ============================================================
struct COMMUNICATION {
  boolean ping;
  boolean booted;
  boolean shutdown;
  String header;
  String message;
};

// initialize the struct
struct COMMUNICATION communication;

// System Debug Mode: Activating extended serial messages
// ============================================================
bool DEBUG = false;

// Screen dimensions
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT  240 // Change this to 96 for 1.27" OLED.

// You can use any (4 or) 5 pins 
//#define SCLK_PIN 52
//#define MOSI_PIN 51

// Define the button status array externally defined in buttonScan
// ============================================================
extern int buttonPressed[12];
extern int buttonPressedPrevious[12];
extern int buttonEdge[12];
extern int buttonFlag[12];
extern int buttonHandled[12];
extern int buttonHandle[12];

// Color definitions
// ------------------------------
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

// Haghish Colors
#define skyBlue         BLUE+0x0f00+1*128
#define lightBlue       BLUE+0x0f00+10*128
#define coolNight       0x2966
#define darkNight       0x18E4 
#define borderLine      0x3A09
#define borderLine2     0xFFFF
#define selectedBox     0xC659 
#define selectedBoxGold 0xD676
#define unselectedText  0xBDF9 
#define selectedText    0xEFDF  //0xF7FF 
#define darkText        0x4A49
#define TWILIGHT        0x8431 
#define WARNING         0xC289

// declare the display before sourcing the menu
// ------------------------------------------------------------
// This screen definition is MUCH faster
// Adafruit_ILI9341 SCREEN = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_MISO);
Adafruit_ILI9341 SCREEN = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

//TFT SCREEN = TFT(CS_PIN, DC_PIN, RST_PIN);

// local functions (prior to setup)
// ============================================================

// THE ORDER OF THESE HEADERS MATTER ???
#include "errors.h"            // reading errors for displayLogo
#include "displayLogo.h"       // BOOT Logo
#include "readSerial.h"        // reading functions for serial communication
#include "MENU.h"              // reading functions for MENU
////#include "OneButton_Clicks.h"  // reading functions for rotatory shift encoder

char buff[100];

// ------------------------------------------------------------
// SETUP()
// ============================================================
void setup() {

  SerialUSB.begin(115200); //default was 11
  SerialUSB.println("Mizzan Microtonal Piano");
  
  buttonScanInit();


  /* WHAT THE FUCK AM I DOING HER?
  //set clkPin,dePin,swPin as INPUT
  pinMode(clkPin, INPUT);
  pinMode(dtPin, INPUT);
  pinMode(swPin, INPUT);
  digitalWrite(swPin, HIGH);
  */

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

/***
  button1.attachDoubleClick(doubleclick);           
  button1.attachClick(singleclick);                
  button1.attachLongPressStart(longPressStart1); 
  button1.attachDuringLongPress(longPress1);
  button1.attachLongPressStop(longPressStop1);
***/

  // initiate the display
  // --------------------
  SCREEN.begin();

  /***
  SPSR |= (1<<SPI2X);

  sprintf(buff, "SPCR : %X", SPCR );
  Serial.println(buff);
  sprintf(buff, "SPSR : %X", SPSR );
  Serial.println(buff);
  sprintf(buff, "SPDR : %X", SPDR );
  Serial.println(buff);

  //SPI.begin();
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
  //SPSR |= (1<<SPI2X);
  ***/

  // read diagnostics (optional but can help debug problems)
  /*
  uint8_t x = SCREEN.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); 
  Serial.println(x, HEX);
  x = SCREEN.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); 
  Serial.println(x, HEX);
  x = SCREEN.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); 
  Serial.println(x, HEX);
  x = SCREEN.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); 
  Serial.println(x, HEX);
  x = SCREEN.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); 
  Serial.println(x, HEX); 
  */

  

  
  //SCREEN.init(SCREEN_HEIGHT, SCREEN_WIDTH);           // Init ST7789 240x320
//  SCREEN.setRotation(1);           // specify the orientation direction of text
//  SCREEN.fillScreen(BLACK);

  SCREEN.setRotation(3);
  /***
  SCREEN.fillScreen(BLACK);
  SCREEN.setTextColor(ILI9341_BLUE); 
  SCREEN.setTextSize(2);
  SCREEN.setCursor(0, 0);
  SCREEN.println("my foonting turlingdromes.");
  delay(1000);
  ***/

  
  // display the "booting..." logo
  // -----------------------------
  displayStart(" mizzan\n", 17, 130,    //title
               "booting...\n",100, 200, //subtitle
               1000);                   //duration

  // =====================================
  // RUN A LOOP UNTIL YOU HEAR FROM PYTHON
  // =====================================
  communication.ping = false;
  communication.booted = true; //true that if you want to test it without the python SERVER
  while (!communication.booted) {
    if(SerialUSB.available() > 0) {
      communication = readSerial(communication=communication);
    }  
    else {
      delay(50);
    }

    if (waitScreen & waitingTime > 0) {
      processScreen(waitingTime);
      waitScreen = false;
      waitingTime= 0;
    }

    // display the "loading instruments..." logo
    // -----------------------------------------
    if (LOG) {
      displayStart(" mizzan\n", 17, 130,    //title
               "instruments...\n",90, 200,  //subtitle
               3000);                       //duration
      LOG = false;
    }
  }

  communication.ping = false;
  

  

  // Apply initial process screen until the instrument is loaded
  // -----------------------------------------------------------
  processScreen(1000);   //the waiting time reduced from 3000 because crane v. 1.0 is too slow
  
  // load the menucommunication
  // --------------------------
  displayMenu(menu=menu,
              rotation=0,
              sClick=sClick,
              dClick=dClick,
              pClick=pClick,
              massage=massage,
              refreshMENU=refreshDisplay);

}

// ------------------------------------------------------------
// LOOP()
// ============================================================
void loop() {

  TIME = millis();
  

  // TEMPORARILY ACTIVATE THIS IF YOU WANT TO TEST THE BOARD WITHOUT PYTHON
  // EVERY COMMAND MUST BE CONFIRMED BY PYTHON. FOR NOW, DISABLE THIS FEATURE FOR TESTING
  //// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  //COMMAND = "";
  //// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  // display error messages, but if a sever error has occured, show the 20 seconds recovery time
  // -------------------------------------------------------------------------------------------
  if (errScreen) {
    if (ERR[1].toInt() < 990) {
      displayErr(1, ERR[1], 5000);
    }
    else {
      processScreen(20000);
    }
    memset(ERR, 0, sizeof(ERR));       //erase the scalars of the string array
    errScreen = false;
    refreshDisplay = true;             // refresh the menu
    pagesLoaded = false;
    itemsLoaded = false;
  }

 


  /*
  // initialize button1
  button1.tick(); 

  // initialize the button sets
  singleButton(A0, false);
  threeButton1(A2, false);
  threeButton2(A3, false);
  */

  // SCAN THE Buttons
  // ------------------------------------------------------------
  buttonScan();
  buttonEdgeScan();
  buttonHandler();
  buttonClearFlag();

  encoderVal = encoderVal + rotation;

  // Getting the navigation direction
  // ------------------------------------------------------------
  
  ////int rotation = getEncoderTurn();//   ////to be added for sound mixing ???
  /*
  int rotation = fiveButton(A1, false);
  encoderVal = encoderVal + rotation;
  */

  if (instrumentButton == HIGH) {
    refreshDisplay = true;
    //sClick = true;
    menu.page = 1;
    menu.item = selectedInstrument;
    memorizeMenuPage= 0;
    memorizeMenuItem= 0;
    instrumentButton = LOW;
  }
  else if (reverbButton == HIGH) {
    refreshDisplay = true;
    //sClick = true;
    menu.page = 4;
    menu.item = selectedREV;
    memorizeMenuPage= 3;
    memorizeMenuItem= 0;
    reverbButton = LOW;
  }
  else if (tuningButton == HIGH) {
    refreshDisplay = true;
    //sClick = true;
    menu.page = 2;
    menu.item = selectedMSF;
    memorizeMenuPage= 1;
    memorizeMenuItem= 0;
    tuningButton = LOW;
  }
  else if (recordButton == HIGH) {
    if (DEBUG) SerialUSB.println("RECORD");
    menu.page = 3;
    menu.item = 0;
    memorizeMenuPage= 2;
    memorizeMenuItem= 0;
    recordButton = LOW;
    refreshDisplay = true;
    if (!recordStatus) {
      recordStatus = true;
      dClick = true;
    }
  }
  else if (playButton == HIGH) {
    if (recordStatus) {
      if (DEBUG) SerialUSB.println("PAUSE");
      refreshDisplay = true;
      menu.page = 3;
      menu.item = 0;
      //memorizeMenuPage= 2;
      //memorizeMenuItem= 0;
      recordStatus = false;
      dClick = true;
    }
    else {
      if (DEBUG) SerialUSB.println("SONGS");
      sClick = true;
      menu.page = 5;
      menu.item = 0;
      memorizeMenuPage= 4;
      memorizeMenuItem= 0;
    }
    playButton = LOW;
  }
  else if (settingsButton == HIGH) {
    refreshDisplay = true;
    sClick = true;
    menu.page = 6;
    memorizeMenuPage= 5;
    memorizeMenuItem= 0;
    settingsButton = LOW;
  }

  // ============================================================
  // MENU
  // reload the menu if there has been an action
  // ============================================================
  if (rotation != 0 | sClick | dClick | pClick) {
    menu = displayMenu(menu=menu,
                      rotation=rotation,
                      sClick=sClick,
                      dClick=dClick,
                      pClick=pClick,
                      massage=massage,
                      refreshMENU=refreshDisplay);

    // check the size of the menu for writing! max is 255 bytes
    //EEPROM.write(0, menu); //writes a number, but not a struct
    // EEPROM.put(eeAddress, menu);
    //Serial.print(sizeof(menu)); Serial.println(" bytes!");
    
    // reset the buttons
    sClick = false;
    dClick = false;
    pClick = false;
    rotation = 0;
    refreshDisplay = menu.refresh;
  }

  // if a refresh is requested, flush the screen
  if (refreshDisplay | globalMenuRefresh) {
    //delay(1000);
    
    menu = displayMenu(menu=menu,
                      rotation=rotation,
                      sClick=sClick,
                      dClick=dClick,
                      pClick=pClick,
                      massage=massage,
                      refreshMENU=refreshDisplay);
                      
    refreshDisplay = false;
    menu.refresh = false;
    globalMenuRefresh = false;

    //channelOne = menu.val_page1[0];
    //channelTwo = menu.val_page1[1];
  }

  // Serial Communication
  // ============================================================
  if(SerialUSB.available() > 0) {
    communication = readSerial(communication=communication);
    
    if (waitScreen & waitingTime > 0) {
      processScreen(waitingTime);
      waitScreen = false;
      waitingTime= 0;
      refreshDisplay = true;
    }

    // reload the menu system
    if (refreshDisplay) {
      pagesLoaded = false;
      itemsLoaded = false;
      displayMenu(menu=menu,
                  rotation=0,
                  sClick=sClick,
                  dClick=dClick,
                  pClick=pClick,
                  massage=massage,
                  refreshMENU=refreshDisplay);
      refreshDisplay = false;
    }
  }

  // Listen for command confirmations
  // --------------------------------
  if (communication.ping) {
    SerialUSB.println("PING");

    // clear the command history
    commandSent        = false;
    commandConfirmed   = true;
    communication.ping = false;
    COMMAND            = "";
  }
  
  // otherwise repeat the command
  // ----------------------------
  if (commandSent & !commandConfirmed) {
    if (!commandWaiting) {
      commandWaiting = true;
      commandWaitingTime = TIME;
    }

    // repost the command every 1000ms
    else if ((TIME - commandWaitingTime) >= 1000 & sizeof(COMMAND) > 1) {
      SerialUSB.println(COMMAND);
      //Serial.print("\n");
      //Serial.print(millis());
      //Serial.print("   ");
      delay(5);
      //Serial.println(commandWaitingTime);
      commandWaitingTime = TIME;
      commandWaiting = false;
      delay(5);
    }
  }

  // Goodbye Logo
  // ============================================================
  if(communication.shutdown) {
    displayStart(" mizzan\n", 17, 130,       //title
                 "please wait...\n",90, 200, //subtitle
                 120000);  
  } 

  //delay(25);
}

// ------------------------------------------------------------
// Once the buttons are identidied, remove serial.print and then move it to buttonScan code file
// ============================================================
void buttonHandler(){

  //an example of multiple button pressed (in this case when button 2 and 3 are pressed the backlight toggles)
  /*
  else if(buttonFlag[2] == 1   && buttonFlag[3] == 1){
    if(buttonEdge[2] == 0  &&  buttonEdge[3] == 0){
      BL_Status = !BL_Status;
      digitalWrite(TFT_BL,BL_Status);
    }
    buttonHandle[2] = 1; //clear the flag
    buttonHandle[3] = 1; //clear the flag
  }
  */

  //example of a single button pressed (in this case when button 0 is pressed, the serial prints the rise/fall state of the button)
  if(buttonFlag[0] == 1 ){
      if(buttonEdge[0] == 0 ){
        if (DEBUG) SerialUSB.println("FALL on 0");
      }else{
        if (DEBUG) SerialUSB.println("RISE on 0");
        rotation = rotation - 1;
        SerialUSB.println(rotation);
      }
    buttonHandle[0] = 1; //clear the flag
  }

  else if(buttonFlag[1] == 1 ){
      if (buttonEdge[1] == 0 ){
        SerialUSB.println("FALL on 1");
      } else {
        SerialUSB.println("RISE on 1");
      }
    buttonHandle[1] = 1; //clear the flag
  }

  else if(buttonFlag[2] == 1 ){
    if(buttonEdge[2] == 0 ){
      if (DEBUG) SerialUSB.println("FALL on 2");
    }else{
      if (DEBUG) SerialUSB.println("RISE on 2");
    }
    buttonHandle[2] = 1; //clear the flag
  }

  else if(buttonFlag[3] == 1 ){
    if(buttonEdge[3] == 0 ){
      if (DEBUG) Serial.println("FALL on 3");
    }else{
      if (DEBUG) Serial.println("RISE on 3");
      rotation = rotation + 1;
      SerialUSB.println(rotation);
    }
    buttonHandle[3] = 1; //clear the flag
  }


  else if(buttonFlag[4] == 1 ){
    if(buttonEdge[4] == 0 ){
      if (DEBUG) SerialUSB.println("FALL on 4");
    }else{
      if (DEBUG) SerialUSB.println("button 4: dClick");
      dClick = true;
    }
    buttonHandle[4] = 1; //clear the flag
  }


  else if(buttonFlag[5] == 1 ){
    if(buttonEdge[5] == 0 ){
      if (DEBUG) SerialUSB.println("FALL on 5");
    }else{
      if (DEBUG) SerialUSB.println("button 5: sClick");
      sClick = true;
    }
    buttonHandle[5] = 1; //clear the flag
  }

  else if(buttonFlag[6] == 1 ){
    if(buttonEdge[6] == 0 ){
      if (DEBUG) SerialUSB.println("FALL on 6");
    }else{
      if (DEBUG) SerialUSB.println("RISE on 6");
      tuningButton = HIGH;
    }
    buttonHandle[6] = 1; //clear the flag
  }

  else if(buttonFlag[7] == 1 ){
    if(buttonEdge[7] == 0 ){
      if (DEBUG) SerialUSB.println("FALL on 7");
    }else{
      if (DEBUG) SerialUSB.println("RISE on 7");
      reverbButton = HIGH;
    }
    buttonHandle[7] = 1; //clear the flag
  }

  // THE NAME IS INCORRECT
  else if(buttonFlag[8] == 1 ){
    if(buttonEdge[8] == 0 ){
      if (DEBUG) SerialUSB.println("FALL on 8");
    }else{
      if (DEBUG) SerialUSB.println("RISE on 8");
      instrumentButton = HIGH;
    }
    buttonHandle[8] = 1; //clear the flag
  }

  // RECORD BUTTON
  // ----------------------------------------
  else if(buttonFlag[9] == 1 ){
    if(buttonEdge[9] == 0 ){
      if (DEBUG) SerialUSB.println("FALL on 9");
    }else{
      if (DEBUG) SerialUSB.println("RISE on 9");
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
        //SerialUSB.print(">RC:");
        //SerialUSB.print(recordStatus);
        SerialUSB.println(COMMAND);
        //SerialUSB.print("\n");
        
        commandSent = true;
        commandConfirmed = false;
        delay(10);
      }
    }
    buttonHandle[9] = 1; //clear the flag
  }

  // PLAY / PAUSE BUTTON
  // ----------------------------------------
  else if(buttonFlag[10] == 1 ){
    if(buttonEdge[10] == 0 ){
      if (DEBUG) SerialUSB.println("FALL on 10");
    } else {
      if (DEBUG) SerialUSB.println("RISE on 10");
      // PAUSE BUTTON
      if (recordStatus) {
        recordButton = LOW;
        recordStatus = LOW;
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
        
        //SerialUSB.print(">RC:");
        //SerialUSB.println(recordStatus);
        //SerialUSB.print("\n");
        delay(10);
      }
      // OTHERWISE ACTIVATE THE PLAY BUTTON, IF NOTHING IS BEING PLAYED NOW
      else {
        playButton = HIGH;
      }
    }
    buttonHandle[10] = 1; //clear the flag
  }

  else if(buttonFlag[11] == 1 ){
    if(buttonEdge[11] == 0 ){
      if (DEBUG) SerialUSB.println("FALL on 11");
    }else{
      if (DEBUG) SerialUSB.println("RISE on 11");
      settingsButton = HIGH;
      delay(50);
    }
    buttonHandle[11] = 1; //clear the flag
  }
  
}

// I have defined a MENU struct and a menu object is passed to
// this function, updated, and returned to the loop.
//#############################################################


// ------------------------------------------------------------
// Display settings
// ============================================================
/* The display should be dividable into specific number of rows/columns;
    Currently, the number of rows is 6. Reduce the screen hight to make
    sure there is no remaining.
*/

/* WARNING
    The white color is somehow defective in the library. Unusual behavior// ------------------------------------------------------------
// PAGE 1: Instrument Screen
// ============================================================
    appear when the background is white. Especially when text is written,
    the background text appears to be whiter than white! This is why, it is adviced
    to avoid using the white color.
*/



int screenWidth  = 320;
int screenHeight = 240; //avoid unbalanced lines
int numberRows   = 6;     // how many rows to draw for the main menu?
int rowHeight    = screenHeight / numberRows;



int memorizeMenuPage = 0;
int memorizeMenuItem = 0;



// define 3 background colors. One for the selected page, as well as
// one for odd numbers and one for even numbers
static const uint16_t PROGMEM colors[] = {darkNight,
                                          coolNight,
                                          selectedBoxGold,
                                          selectedBox,
                                          WHITE,
                                          BLACK,
                                          WARNING
                                         };

static const uint16_t PROGMEM textColors[] = {unselectedText,
                                              unselectedText,
                                              BLACK,
                                              darkText,
                                              TWILIGHT,
                                              selectedText
                                             };


// ------------------------------------------------------------
// define the MENU stuct
// ============================================================
struct MENU {
  int bank = 0; //bank address on the memory (for system setting)
  int page = 1;
  int item = 0;

  // the page1 has 2 items, both integers. REMEMBER, the first location
  // of the array is '0' not '1'
  int val_page1[2] = {25, 0};  // preset values for item2 & item1
  int val_page1_max[2] = {100, 100};  // define the maximum values for page1
  int val_page1_min[2] = { -100, -100}; // define the minimum values for page1
  boolean refresh = false; //refresh the display after execution
};

// initialize the menu
struct MENU menu;


// ------------------------------------------------------------
// define the GLOBAL tuning stuct
// ============================================================
struct GLOBAL {
  int key1 = 0; //bank address on the memory (for system setting)
  int key2 = 25;
  int key3 = 0;
  int currentKeybed = 0;

  boolean refresh = false; //refresh the tuning
};

// initialize the global tuning struct
struct GLOBAL global;


// ------------------------------------------------------------
// MENU SETTINGS
// ============================================================
String menuPages[] = {"Instrument",
                      "Tuning",
                      "Record",
                      "Reverb",
                      "Songs",
                      "Settings"
                     };



String menuInstruments[] = {"",
                            "Grand Piano",
                            "Lutheal Piano",
                            "Fortepiano",
                            "Upright Lite",
                            "John Cage",
                            "Muted Grand Piano",
                            "Lute-Harpsichord",
                            "Rollig Orphica",
                            "Rhodes Mark I (line)",
                            "Rhodes Mark I (amp)",
                            "Wurlitzer",
                            "Celesta",
                            "Dulcitone",
                            "Prepared Pianet",
                            "Harp",
                            "Philicorda",
                            "Electric Guitar",
                            /*"Accoustic Guitar",
                            
                            "Harpsichord II",
                            "Harpsichord III",
                            "Church Organ",
                            "Marimba",                  
                            "upright Bass"*/
                           };

int instrumentsNum = (sizeof(menuInstruments)/ sizeof(menuInstruments[0])) - 1;

String menuInstrumentsBrief[] = {"",
                                 "grand piano",
                                 "lutheal",
                                 "fortepiano",
                                 "up. piano",
                                 "j. cage",
                                 "muted grand",
                                 "lute-harpsi.",
                                 "orphica",
                                 "mark I line",
                                 "mark I amp",
                                 "wurlitzer",
                                 "celesta",
                                 "dulcitone",
                                 "pianet",
                                 "harp",
                                 "philicorda"
                                 "lutheal",
                                 "e. guitar"
                                };

String menuRecord[] = {"",
                       "start",
                       "stop",
                       "",
                       "",
                       ""};


String menuSettings[] = {"",
                       "def. inst.",
                       "def. tun.",
                       "polyphony",   
                       "Gain",  
                       "audio rec.", 
                       "audio fmt.",
                       //"aud. quality",
                       ""};


int RecArrayLength = (sizeof(menuRecord)/ sizeof(menuRecord[0])) - 1;

int SetArrayLength = (sizeof(menuSettings)/ sizeof(menuSettings[0])) - 1;


// PAGES DEFAULT VALUES
// ===================================
int currentInstrument = 0;
int currentMSF = 0;
int currentRec = 0;
int currentREV = 0;
int currentPLY = 0;
int currentSET = 0;
int selectedInstrument = 1; //on the start, pick the first item
int selectedMSF = 1;  
int selectedREV = 1;
int selectedPLY = 1;
int selectedSET = 1;


int menuPagesNum = 6;
int menuPageMax = 6;     // number of the last menu page
const int page_max_items[5] = {6, 6, 16, 16, 6}; //number of items per page
const int page_min_items[5] = {1, 1, 1, 1, 1}; //smallest value. it should be 1 for all, but not currently!
bool actionButton = false;
bool submenuButton = false;
bool updateItemValue = false; // update an item value in the menu
bool bankMenu = false;
int  bankItem = 1;
bool securityScreenMenu = false;
bool securityScreenAnswer = false;
bool securityScreenAction = false;
bool globalMenuRefresh = false;


void doneScreen() {
  SCREEN.fillScreen(BLACK);
  SCREEN.setCursor(0, 0);
  SCREEN.setTextSize(2);
  SCREEN.setTextColor(BLACK, WHITE);
  SCREEN.print(" Done!                       ");
  SCREEN.setTextSize(2);
  SCREEN.setTextColor(WHITE, BLACK);
  delay(1000);
}

void echo (String txt) {
  SCREEN.fillScreen(BLACK);
  SCREEN.setCursor(0, 0);
  SCREEN.setTextColor(WHITE, BLACK);
  SCREEN.setTextSize(2);
  SCREEN.print(txt);
  SCREEN.setTextSize(2);
}

void header (String txt, int bgcolor, int txtcolor) {
  SCREEN.fillScreen(pgm_read_word(&colors[0]));
  SCREEN.fillRect(0, 0, screenWidth, (screenHeight / numberRows),
                   pgm_read_word(&colors[bgcolor]));
  SCREEN.setCursor(3, 7);
  SCREEN.setTextColor(pgm_read_word(&textColors[txtcolor]));

  // print the header
  SCREEN.setTextSize(3);
  SCREEN.print(menuPages[memorizeMenuPage]);
  SCREEN.setTextSize(2);
}


// ------------------------------------------------------------
// PAGE 1: Instrument Screen
// ============================================================
int instrumentScreen(int menuitem,
                     int currentInstrument,
                     int selectedInstrument,
                     int itemsLoaded) {
  int startPoint = 1;
  bool refresh = false;

  // RULES
  // -----
  
  // on the first boot of the screen, pick the selected instrument
  if (!itemsLoaded) currentInstrument = 0;

  // rules for refreshing the screen
  if (menuitem > 5) {
    startPoint = menuitem - 4;
    refresh = true;
  }
  else if (menuitem <= 5 & currentInstrument > 5) refresh = true;

  int j = 0;
  for (int i = startPoint; i < startPoint + 5; i++) {
    j = j + 1;
    int startRect = (screenHeight * j / numberRows);
    SCREEN.setTextColor(pgm_read_word(&textColors[1]));

    if (refresh) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[0]));
    }

    if (currentInstrument == i) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[0]));
    }
    else if (menuitem == i) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[1]));
    }
    SCREEN.setCursor(3, startRect + 10);

    if (selectedInstrument == i) {
      SCREEN.setTextColor(pgm_read_word(&textColors[5]));
    }
    SCREEN.print(menuInstruments[i]);
  }
  return menuitem;
}

// ------------------------------------------------------------
// PAGE 2: Loading MSF tuning files
// ============================================================
int msfScreen(int menuitem,
              int currentMSF,
              int selectedMSF,
              int itemsLoaded) {

  int startPoint = 1;
  bool refresh = false;
  

  // RULES
  // -----

  // on the first boot of the screen, pick the selected instrument
  if (!itemsLoaded) {
    currentMSF = 0;
    menuitem = selectedMSF;
  }

  // rules for refreshing the screen
  if (menuitem > 5) {
    startPoint = menuitem - 4;
    refresh = true;
  }
  else if (menuitem <= 5 & currentMSF > 5) refresh = true;

  int j = 0;
  for (int i = startPoint; i < startPoint + 5; i++) {
    j = j + 1;
    int startRect = (screenHeight * j / numberRows);
    SCREEN.setTextColor(pgm_read_word(&textColors[1]));

    if (refresh) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[0]));
    }

    if (currentMSF == i) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[0]));
    }
    else if (menuitem == i) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[1]));
    }
    if (selectedMSF == i) {
      SCREEN.setTextColor(pgm_read_word(&textColors[5]));
    }
    SCREEN.setCursor(3, startRect + 10);
    SCREEN.print(MSF[i]);
  }
  return menuitem;
}

// ------------------------------------------------------------
// PAGE 3: Recording Audio and MIDI
// ============================================================

/*
- define a variable stating whether the instrument is currently recording 
- if currently the instrument is recording, jump on 'stop'
*/
int recScreen(int menuitem,   // item in the list defined for the subscreen
             int currentRec,  // current item menu which the scroller has stopped on
             bool recordStatus,
             int itemsLoaded) 
             {
      
  int startPoint = 1;
  bool refresh = false;

  

  // RULES
  // -----

  if (!itemsLoaded)  currentRec = 0;


  // rules for refreshing the screen
  if (menuitem > 5) {
    startPoint = menuitem - 4;
    refresh = true;
  }
  else if (menuitem <= 5 & currentRec > 5) refresh = true;

  // include globab refresh
  if (refreshDisplay) {
    if (DEBUG) Serial.println("OK REFRESH REC SCREEN");
    refresh = true;
    refreshDisplay = false;
  }

  int j = 0;
  for (int i = startPoint; i < startPoint + 5; i++) {
    j = j + 1;
    int startRect = (screenHeight * j / numberRows);
    SCREEN.setTextColor(pgm_read_word(&textColors[1]));

    if (refresh) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[0]));
    }

    if (currentRec == i) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[0]));
    }
    else if (menuitem == i) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[1]));
    }
    SCREEN.setCursor(3, startRect + 10);

    // if the system is recording, highlight the first row, which is for record button!
    if (recordStatus & i == 1) {
      //SCREEN.setTextColor(pgm_read_word(&textColors[5]));
    }
    SCREEN.print(menuRecord[i]);
  }
  return menuitem;
}

// ------------------------------------------------------------
// PAGE 4: Reverb presets
// ============================================================
int revScreen(int menuitem,
              int currentREV,
              int selectedREV,
              int itemsLoaded) {

  int startPoint = 1;
  bool refresh = false;
  

  // RULES
  // -----
  

  // on the first boot of the screen, pick the selected instrument
  if (!itemsLoaded) {
    currentREV = 0;
    menuitem = selectedREV;
  }

  // rules for refreshing the screen
  if (menuitem > 5) {
    startPoint = menuitem - 4;
    refresh = true;
  }
  else if (menuitem <= 5 & currentREV > 5) refresh = true;

  int j = 0;
  for (int i = startPoint; i < startPoint + 5; i++) {
    j = j + 1;
    int startRect = (screenHeight * j / numberRows);
    SCREEN.setTextColor(pgm_read_word(&textColors[1]));

    if (refresh) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[0]));
    }

    if (currentREV == i) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[0]));
    }
    else if (menuitem == i) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[1]));
    }
    if (selectedREV == i) {
      SCREEN.setTextColor(pgm_read_word(&textColors[5]));
    }
    SCREEN.setCursor(3, startRect + 10);
    SCREEN.print(REV[i]);
  }
  return menuitem;
}

// ------------------------------------------------------------
// PAGE 5: Loading MIDI files
// ============================================================
int plyScreen(int menuitem,
              int currentPLY,
              int selectedPLY,
              int itemsLoaded) {

  int startPoint = 1;
  bool refresh = false;
  

  // RULES
  // -----

  // on the first boot of the screen, pick the selected instrument
  if (!itemsLoaded) {
    currentPLY = 0;
    menuitem = selectedPLY;
  }

  // rules for refreshing the screen
  if (menuitem > 5) {
    startPoint = menuitem - 4;
    refresh = true;
  }
  else if (menuitem <= 5 & currentPLY > 5) refresh = true;

  // include globab refresh
  if (refreshDisplay) {
    if (DEBUG) Serial.println("OK REFRESH PLY SCREEN");
    refresh = true;
    refreshDisplay = false;
  }

  int j = 0;
  for (int i = startPoint; i < startPoint + 5; i++) {
    j = j + 1;
    int startRect = (screenHeight * j / numberRows);
    SCREEN.setTextColor(pgm_read_word(&textColors[1]));

    if (refresh) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[0]));
    }

    if (currentPLY == i) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[0]));
    }
    else if (menuitem == i) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[1]));
    }
    if (selectedPLY == i) {
      SCREEN.setTextColor(pgm_read_word(&textColors[5]));
    }
    SCREEN.setCursor(3, startRect + 10);
    SCREEN.print(PLY[i]);
  }
  return menuitem;
}

// ------------------------------------------------------------
// PAGE 6: Settings screen
// ============================================================
int setScreen(int menuitem,
              int currentSET,
              int selectedSET,
              int itemsLoaded) {

  int startPoint = 1;
  bool refresh = false;
  

  // RULES
  // -----

  // on the first boot of the screen, pick the selected instrument
  if (!itemsLoaded) {
    currentSET = 0;
    menuitem = selectedSET;
  }

  // rules for refreshing the screen
  if (menuitem > 5) {
    startPoint = menuitem - 4;
    refresh = true;
  }
  else if (menuitem <= 5 & currentSET > 5) refresh = true;

  int j = 0;
  for (int i = startPoint; i < startPoint + 5; i++) {
    j = j + 1;
    int startRect = (screenHeight * j / numberRows);
    SCREEN.setTextColor(pgm_read_word(&textColors[1]));

    if (refresh) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[0]));
    }

    if (currentSET == i) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[0]));
    }
    else if (menuitem == i) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[1]));
    }
    if (selectedSET == i) {
      SCREEN.setTextColor(pgm_read_word(&textColors[5]));
    }
    SCREEN.setCursor(3, startRect + 10);
    SCREEN.print(menuSettings[i]);
  }
  return menuitem;
}

/*
// global tuning submenu
String pianoName[] = {""};

struct GLOBAL globalTuning(struct GLOBAL global,
                           int menuitem) {


  int j = 0;
  for (int i = 1; i < 4; i++) {
    j = j+1;
    int startRect = (screenHeight * j / numberRows);

    if (global.currentKeybed == i) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                 pgm_read_word(&colors[0]));
    }
    else if (menuitem == i) {
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                 pgm_read_word(&colors[1]));
    }

    SCREEN.setTextColor(WHITE);
    SCREEN.setCursor(3, startRect+5);
    SCREEN.print(pianoName[i]);
  }

  // update current keybed
  global.currentKeybed = menu.item;

  // return the construct
  return global;
}
*/

/*
void eraseMemory() {
  for (int i = 0; i < 1000; ++i) {
    EEPROM.write(i, -1);
  }
}


void securityScreen(boolean securityScreenAnswer) {
  SCREEN.fillScreen(BLACK);
  SCREEN.setCursor(0,0);
  SCREEN.setTextColor(WHITE, BLACK);
  SCREEN.print(" Erasing the memory:\n\n");
  if (!securityScreenAction) {
    if (!securityScreenAnswer) {
      SCREEN.setTextColor(BLACK, WHITE);
      SCREEN.print(" No               \n");
      SCREEN.setTextColor(WHITE, BLACK);
      SCREEN.print(" Yes              \n");
    } else {
      SCREEN.print(" No               \n");
      SCREEN.setTextColor(BLACK, WHITE);
      SCREEN.print(" Yes              \n");
      SCREEN.setTextColor(WHITE, BLACK);
    }
  }
  else {
    if (securityScreenAnswer) {
      echo("Erasing...");
      eraseMemory();
    }
    else {
      SCREEN.fillScreen(BLACK);
      SCREEN.setCursor(0,0);
      //SCREEN.display();
    }
    submenuButton = false; //move up
    securityScreenMenu = false;
    securityScreenAnswer = false;
    securityScreenAction = false;
    menu.refresh = true;
    globalMenuRefresh = true;
  }
}
*/

// ------------------------------------------------------------
// define MENU function
// ============================================================

/*
  Description
  -----------

  there are three types of screens in the menu. each of these
  types is treated seperately as a submenu. Currently the followings
  are implemented:

  we currently have 3-level menu:
  1. navigation menu (menu.item == 0)
  2. submenu (menu.item >= 1)
    3.a securityScreen, ascing for approval of the action
    3.b doneScreen

  Rules
  =====


*/


struct MENU displayMenu(struct MENU menu,
                        int rotation,
                        boolean sClick,
                        boolean dClick,
                        boolean pClick,
                        int massage,
                        boolean refreshMENU) {

  int16_t i; //???

  /*
    MAIN NAVIGATION SCREEN
    ======================

    The main menu navigation should know the maximum number of pages.
    If the rotation number exceed the limit, it starts over at the top or buttom.
  */
  if (menu.item == 0 & rotation != 0) {
    menu.page = menu.page + rotation;

    // add the menu.page limits
    if (menu.page > menuPageMax) menu.page = 1;
    else if (menu.page < 1) menu.page = menuPageMax;

    if (DEBUG) {
      Serial.print("RULE 4 >   page: ");
      Serial.print(menu.page);
      Serial.print("   >   item: ");
      Serial.println(menu.item);
    }
  }

  /*
    SUBMENU NAVIGATION SCREEN
    =========================

    The item menu navigation is more specialized, showing behavior
    based on the selected page. For selecting instrument, the menu
    should wrap if the value is les than the minimum or maximum number
    of the instruments. For the other pages, there is no wrapping need.
    Finally, for the *reset* menu, the security page should be loaded.
  */

  else {
    // Navigating from menu.page to menu.item
    if (sClick & menu.item == 0) {
      if (menu.page == 1) menu.item = selectedInstrument;
      else if (menu.page == 2) {
        if (selectedMSF == 0) menu.item = 1;
        else menu.item = selectedMSF;
      }
      else if (menu.page == 4) {
        if (selectedREV == 0) menu.item = 1;
        else menu.item = selectedREV;
      }
      else if (menu.page == 5) {
        if (selectedPLY == 0) menu.item = 1;
        else menu.item = selectedPLY;
      }
      else if (menu.page == 6) {
        if (selectedSET == 0) menu.item = 1;
        else menu.item = selectedSET;
      }
      else menu.item = menu.item + 1;
      sClick = false;

      if (DEBUG) {
        Serial.print("sClick-Rule >   page: ");
        Serial.print(menu.page);
        Serial.print("   >   item: ");
        Serial.println(menu.item);
      }
    }

    // back to page.menu
    if (dClick) {
      menu.item = 0;
      bankMenu = false;
      submenuButton = false;

      if (DEBUG) {
        Serial.print("dClick-Rule > page: ");
        Serial.print(menu.page);
        Serial.print("   >   item: ");
        Serial.println(menu.item);
      }
    }

    // PG1: Instrument page rules
    // --------------------------
    if (menu.page == 1) {

      //if sClick, specify the selected instrument
      if (sClick) {
        selectedInstrument = menu.item;
        menu.item = 0;
        itemsLoaded = false;
        
        // CODE FOR INSTRUMENTS IS '>IN:'
        COMMAND = String(">IN:") + selectedInstrument + String("\r\n");
        Serial.println(COMMAND);
        commandSent = true;
        commandConfirmed = false;
        delay(250);
        
        //Serial.print(">IN:");
        //Serial.println(selectedInstrument);
        //Serial.print("\n");
      }

      // else, navigate only
      else if (itemsLoaded & rotation != 0) {
        menu.item = menu.item + rotation;

        // apply the limits & fix the 0 value
        if (menu.item < 1) menu.item = instrumentsNum;
        else if (menu.item > instrumentsNum) menu.item = 1;

        if (DEBUG) {
          Serial.print("Page1-Rule > page: ");
          Serial.print(menu.page);
          Serial.print(" > item: ");
          Serial.println(menu.item);
        }
      }
    }

    // Tuning page rules
    // -----------------
    if (menu.page == 2) {
      if (sClick) {
        selectedMSF = menu.item;
        menu.item = 0;
        itemsLoaded = false;
        
        // CODE FOR SPECIFYING MSF FILE '>MT:'
        COMMAND = String(">MT:") + selectedMSF + String("\r\n");
        Serial.println(COMMAND);
        commandSent = true;
        commandConfirmed = false;
        delay(250);
        
        //Serial.print(">MT:");
        //Serial.println(selectedMSF);
        //Serial.print("\n");
        delay(10);
      }

      // else, navigate only
      else if (itemsLoaded & rotation != 0) {
        menu.item = menu.item + rotation;

        // apply the limits & fix the 0 value
        if (menu.item < 1) menu.item = MsfArrayLength;
        else if (menu.item > MsfArrayLength) menu.item = 1;
        if (DEBUG) {
          Serial.print("Page2-Rule > page: ");
          Serial.print(menu.page);
          Serial.print(" > item: ");
          Serial.println(menu.item);
        }
      }
    } 

      /*
        // update values & apply the minimum/maximum item value limits
        if (menu.item != 0 & rotation != 0 & !updateItemValue) {
        menu.item = menu.item + rotation;
        if (menu.item > page_max_items[menu.page-1]) menu.item = page_min_items[menu.page-1];
        else if (menu.item < page_min_items[menu.page-1]) menu.item = page_max_items[menu.page-1];

        //Serial.print("RULE 1 >   page: ");
        //Serial.print(menu.page);
        //Serial.print("item > ");
        //Serial.println(menu.item);
        }

        // 3) When single-click is made, move the navigation to the items
        //    alternatively, if the navigation is already on an item,
        //    execute it or move to its subnavigation
        if (sClick) {
        //Serial.println("RULE 3 > ");
        if (menu.item == 0) menu.item = menu.item + 1;

        // define actionButton and submenuButton
        else if (menu.item > 0) {
          if (submenuButton) {
            submenuButton= false;
            updateItemValue=false;
            //Serial.print("RULE 3 >   actionButton & updateItemValue DEctivated for menu.item ");
          }
          else {
            submenuButton = true; //??? do all want submenuButton?
            //Serial.print("RULE 3 >   actionButton activated for menu.item ");
          }
          //Serial.println(menu.item);
        }
        }

        // 6) Rules for updating the menu.item.values
        //-------------------------------------------------------------
        if (menu.page == 2 & submenuButton) {
        updateItemValue = true;
        menu.val_page1[menu.item-1] = menu.val_page1[menu.item-1] + rotation;
        if (menu.val_page1[menu.item-1] > menu.val_page1_max[menu.item-1]) {
          menu.val_page1[menu.item-1] = menu.val_page1_max[menu.item-1];
        } else if (menu.val_page1[menu.item-1] < menu.val_page1_min[menu.item-1]) {
          menu.val_page1[menu.item-1] = menu.val_page1_min[menu.item-1];
        }

        //Serial.print("RULE 6 > val_page1[0]:  ");
        //Serial.print(menu.val_page1[0]);
        //Serial.print("  val_page1[1] > ");
        //Serial.println(menu.val_page1[1]);
        }

        // 4) When double-click is made, move to the menu
        if (dClick) {
        menu.item = 0;
        submenuButton = false;
        //Serial.print("RULE 4 >   page: ");
        //Serial.println(menu.page);
        }

        // 5) add the menu.item limits based on page
        //-------------------------------------------------------------
        if (menu.item != 0) {
        if (menu.item > page_max_items[menu.page-1]) menu.item = page_min_items[menu.page-1];
        else if (menu.item < page_min_items[menu.page-1]) menu.item = page_max_items[menu.page-1];
        //Serial.print("RULE 5 > menu.item:");
        //Serial.println(menu.item);
        }

      */


    // PAGE3 
    // -----------------
    else if (menu.page == 3) {
      if (sClick) {
        
        if (menu.item == 1) recordStatus = true;                  // update recording status
        else if (menu.item == 2) recordStatus = false;            // update recording status
        
        menu.item = 0;
        itemsLoaded = false;
        

        // CODE FOR 'RECORDING' File IS '>RC:'
        COMMAND = String(">RC:") + recordStatus + String("\r\n");
        Serial.println(COMMAND);
        commandSent = true;
        commandConfirmed = false;
        
        //Serial.print(">RC:");
        //Serial.println(recordStatus);
        //Serial.print("\n");
        delay(10);
      }

      // else, navigate only
      else if (itemsLoaded & rotation != 0) {
        menu.item = menu.item + rotation;

        // apply the limits & fix the 0 value
        if (menu.item < 1) menu.item = RecArrayLength;
        else if (menu.item > RecArrayLength) menu.item = 1;
        if (DEBUG) {
          Serial.print("Page2-Rule > page: ");
          Serial.print(menu.page);
          Serial.print(" > item: ");
          Serial.println(menu.item);
        }
      }
    }

    // PAGE4
    // -----------------
    else if (menu.page == 4) {
      if (sClick) {
        selectedREV = menu.item;
        menu.item = 0;
        itemsLoaded = false;
        if (selectedREV != 1) reverbStatus = true;                  // update recording status
        else if (selectedREV == 1) reverbStatus = false;            // update recording status

        if (DEBUG) {
          Serial.print("reverbStatus");
          Serial.println(reverbStatus);
        }
        
        // CODE FOR 'RECORDING' File IS '>RV:'
        COMMAND = String(">RV:") + selectedREV + String("\r\n");
        Serial.println(COMMAND);
        commandSent = true;
        commandConfirmed = false;
        
        //Serial.print(">RV:");
        //Serial.println(selectedREV);
        //Serial.print("\n");
        delay(10);
      }

      // else, navigate only
      else if (itemsLoaded & rotation != 0) {
        menu.item = menu.item + rotation;

        // apply the limits & fix the 0 value
        if (menu.item < 1) menu.item = RevArrayLength;
        else if (menu.item > RevArrayLength) menu.item = 1;
        if (DEBUG) {
          Serial.print("Page4-Rule > page: ");
          Serial.print(menu.page);
          Serial.print(" > item: ");
          Serial.println(menu.item);
        }
      }
    }

    // PLAY page rules
    // -----------------
    if (menu.page == 5) {
      if (sClick) {
       

        if (!playStatus) {
          if (menu.item > 0) playStatus = true;
          selectedPLY = menu.item;
        } else {
          if (selectedPLY == menu.item) {
            selectedPLY = 1;
            playStatus  = false;
          }
          else {
            selectedPLY = menu.item;
            playStatus = true;

          }
        }
        
        menu.item = 0;
        itemsLoaded = false;
        
        // CODE FOR 'RECORDING' File IS '>PL:'
        COMMAND = String(">PL:") + selectedPLY + String("\r\n");
        Serial.println(COMMAND);
        commandSent = true;
        commandConfirmed = false;
        //Serial.print(">PL:");
        //Serial.println(selectedPLY);
        //Serial.print("\n");
        delay(10);
      }

      // else, navigate only
      else if (itemsLoaded & rotation != 0) {
        menu.item = menu.item + rotation;

        // apply the limits & fix the 0 value
        if (menu.item < 1) menu.item = PlyArrayLength;
        else if (menu.item > PlyArrayLength) menu.item = 1;

        if (DEBUG) Serial.println(PlyArrayLength);
        
        if (DEBUG) {
          Serial.print("Page5-Rule > page: ");
          Serial.print(menu.page);
          Serial.print(" > item: ");
          Serial.println(menu.item);
        }
      }
    } 

    // ##########################################################
    // PAGE SIX (SETTINGS)
    // ##########################################################
    if (menu.page == 6) {
      if (sClick & menu.item == 0) {
        menu.item = menu.item + 1;
        sClick = false;
      }
      else if (menu.item != 0) {

        // navigate in the main items and apply limits
        //-------------------------------------------------------------
        if (rotation != 0 & !bankMenu) {
          menu.item = menu.item + rotation;
          if (menu.item > 5) menu.item = 5;
          else if (menu.item < 1) menu.item = 1;
        }

        if (rotation != 0 & bankMenu) {
          //Serial.println("bankMenu is true!");
        }

        if (sClick) {
          if (menu.item == 0) menu.item = menu.item + 1;
          if (menu.item == 3 | menu.item == 4) {
            if (!bankMenu) {
              bankMenu = true;
              bankItem = 1;
            } else {
              actionButton = true;
            }
          }
          if (menu.item == 5) {
            if (submenuButton) submenuButton = false;
            else submenuButton = true;
          }
        }

      }



      // 4) When double-click is made, move to the menu
      if (dClick) {
        menu.item = 0;
        bankMenu = false;
        submenuButton = false;
      }


      /*
        if (menu.item != 0) {
        if (menu.item > page_max_items[menu.page-1]) menu.item = page_min_items[menu.page-1];
        else if (menu.item < page_min_items[menu.page-1]) menu.item = page_max_items[menu.page-1];
        Serial.println("RULE 5 > ");
        }
      */
    }

    if (bankMenu & rotation != 0) {
      // update values & apply the minimum/maximum item value limits
      bankItem = bankItem + rotation;
      if (bankItem > 16) bankItem = 1;
      else if (bankItem < 1) bankItem = 16;
    }
  }


  // ##########################################################
  // PAGE SIX (SHUTDOWN)
  // ##########################################################
  /*
  if (menu.page == 6) {
    if (sClick) {
      //doneScreen();
      displayLogo(true, 4, "  Bye  \n");
      Serial.println(">OS: shutdown -P now");
      sClick = false;
      //byeLogo();
      //delay(99999);
    }
  }
  */



  // ============================================================
  // UPDATING THE SCREEN
  // ============================================================

  // if menu.item == 0, only navigate among the pages
  // Otherwise, only update the subset of the screen that has changed

  if (menu.item == 0) {

    // reset global variables used for item menu
    itemsLoaded = false;

    if (!pagesLoaded) {
      for (int i = 0; i < menuPagesNum; i++) {
        int startRect = screenHeight * i / numberRows;
        int rectCol = 0; //define the color for Odd numbers and change it otherwise
        if (memorizeMenuPage == i) rectCol = 2;
        else if ( (i & 0x01) == 0) rectCol = 1;   //TRUE, if number is "even" (i.e. the least significant bit is zero)

        // background color
        SCREEN.fillRect(0, startRect, SCREEN_WIDTH, (screenHeight / numberRows),
                         pgm_read_word(&colors[rectCol]));

        // print the text and selectedBox border based on rectCol
        SCREEN.setCursor(5, startRect + 10);
        SCREEN.setTextColor(pgm_read_word(&textColors[rectCol]));
        SCREEN.setTextSize(2);
        SCREEN.print(menuPages[i]);

        // draw the boarder for the selected page
        //(X1,Y1)  ,  (X1,Y2),  Direction and width
        if (rectCol == 2) {
          SCREEN.drawRect(0, startRect, 320, rowHeight, borderLine); //defined borderLine color
        }
      }
      pagesLoaded = true;
    }
    else {
      // update the previously memorized page
      int startRect = screenHeight * memorizeMenuPage / numberRows;
      int rectCol = 0; //define the color for Odd numbers and change it otherwise
      if ((memorizeMenuPage & 0x01) == 0) rectCol = 1;
      SCREEN.fillRect(0, startRect, screenWidth, (screenHeight / numberRows),
                       pgm_read_word(&colors[rectCol]));
      SCREEN.setCursor(3, startRect + 10);
      SCREEN.setTextColor(pgm_read_word(&textColors[1]));
      SCREEN.print(menuPages[memorizeMenuPage]);

      // update the new selected page & memorize menu page
      // ----------------------------------------
      memorizeMenuPage = menu.page - 1;
      startRect = screenHeight * memorizeMenuPage / numberRows;
      SCREEN.fillRect(0, startRect, screenWidth, rowHeight,
                       pgm_read_word(&colors[2]));
      SCREEN.drawRect(0, startRect, screenWidth, rowHeight, borderLine); //defined borderLine color
      SCREEN.setCursor(3, startRect + 10);
      SCREEN.setTextColor(pgm_read_word(&textColors[2]));
      SCREEN.print(menuPages[memorizeMenuPage]);
    }
  }

  // ------------------------------------------------------------
  // Submenu
  // ============================================================
  else {

    // specify that the pagemenu is false
    pagesLoaded = false;

    // if the item.menu is loaded for the first time, boot it all.
    // otherwise, just update the screen to avoid flickering
    if (!itemsLoaded) {
      if (menu.page != 6) header(menuPages[memorizeMenuPage], 2, 2);
      else header(menuPages[memorizeMenuPage], 6, 2);

      // LOADING MENU ITEMS FOR EACH PAGE
      // ================================

      // PAGE1: INSTRUMENTS First Load
      // -----------------------------
      if (menu.page == 1) {
        currentInstrument = instrumentScreen(menu.item,
                                             currentInstrument,
                                             selectedInstrument,
                                             itemsLoaded);
      }

      // PAGE2: MSF list
      // -----------------------------
      else if (menu.page == 2) {
        currentMSF = msfScreen(menu.item,
                               currentMSF,
                               selectedMSF,
                               itemsLoaded);
      }

      // PAGE3: RECORD PAGE
      // -----------------------------
      else if (menu.page == 3) {
        currentRec = recScreen(menu.item,
                 currentRec,
                 recordStatus,
                 itemsLoaded);
      }

      // PAGE4: REV list
      // -----------------------------
      else if (menu.page == 4) {
        currentREV = revScreen(menu.item,
                               currentREV,
                               selectedREV,
                               itemsLoaded);
      }
      else if (menu.page == 5) {
        currentPLY = plyScreen(menu.item,
                               currentPLY,
                               selectedPLY,
                               itemsLoaded);
      }
      else if (menu.page == 6) {
        currentSET = setScreen(menu.item,
                               currentSET,
                               selectedSET,
                               itemsLoaded);
      }

      //memorizeMenuItem = menu.item;
      itemsLoaded = true;
    }
    else {

      // PAGE1: INSTRUMENTS First Load
      // -----------------------------
      if (menu.page == 1) {
        currentInstrument = instrumentScreen(menu.item,
                                             currentInstrument,
                                             selectedInstrument,
                                             itemsLoaded);
      }
      else if (menu.page == 2) {
        currentMSF = msfScreen(menu.item,
                               currentMSF,
                               selectedMSF,
                               itemsLoaded);
      }
      else if (menu.page == 3) {
        currentRec = recScreen(menu.item,
                 currentRec,
                 recordStatus,
                 itemsLoaded);
      }
      else if (menu.page == 4) {
        currentREV = revScreen(menu.item,
                               currentREV,
                               selectedREV,
                               itemsLoaded);
      }
      else if (menu.page == 5) {
        currentPLY = plyScreen(menu.item,
                               currentPLY,
                               selectedPLY,
                               itemsLoaded);
      }
      else if (menu.page == 6) {
        currentSET = setScreen(menu.item,
                               currentSET,
                               selectedSET,
                               itemsLoaded);
      }
    }
  }

  // MAIN PAGE
  // =========================================================
  if (menu.item == 0) {

    String abbTune = "";
    String addReverb= "";
    String recDot = "";
    
    //SCREEN.fillRect(72, 0, screenWidth, screenHeight, RED);
    for (int i = 0; i < 5; i++) {
      
      int startRect = screenHeight * i / numberRows;
      int rectCol = 4; //define the color for Odd numbers and change it otherwise
      if (memorizeMenuPage == i) rectCol = 3;
      else if ( (i & 0x01) == 0) rectCol = 4;   //TRUE, if number is "even" (i.e. the least significant bit is zero)

      // print the text and selectedBox border based on rectCol
      SCREEN.setCursor(180, startRect + 10);
      SCREEN.setTextColor(pgm_read_word(&textColors[rectCol]));
      SCREEN.setTextSize(2);

      abbTune   = MSF[selectedMSF];
      abbTune   = abbTune.substring(0,9);
      addReverb = REV[selectedREV];
      addReverb = addReverb.substring(0,9);
      
      if (i == 0) SCREEN.print(menuInstrumentsBrief[selectedInstrument]);
      if (i == 1) SCREEN.print(abbTune);
      if (i == 2) {
        if (recordStatus) {
          //recDot = "on";
         SCREEN.fillCircle(103, 100, 6, RED); //0x0062
        }
        //else recDot = "off"; 
        //SCREEN.print(recDot);
      }
      if (i == 3) SCREEN.print(addReverb);
      if (i == 4) {
        if (playStatus) {
          SCREEN.fillTriangle(100, 174, 100, 188, 110, 181, GREEN);
        }
      }

      // draw the boarder for the selected page
      //(X1,Y1)  ,  (X1,Y2),  Direction and width
      if (rectCol == 2) {
        SCREEN.drawRect(0, startRect,   320, rowHeight, borderLine); //defined borderLine color
      }
    }
  }
  // ------------------------------------------------------------
  // securityScreenMenu ???
  // ============================================================


  // return the construct
  return menu;
}

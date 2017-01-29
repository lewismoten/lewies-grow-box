/*
 * Lewie's Grow Box
 * 
 * Create an automated growbox to assist with indoor gardening
 */
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define MODE_NORMAL 0
#define MODE_MENU 1
#define MODE_TIMEOUT 10 * 1000 // 10 seconds

byte MODE = MODE_NORMAL;
unsigned long timeout = 0;

// Clk, Din, DC, CE, RST
Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 10, 11, 12);
//Adafruit_PCD8544 display = Adafruit_PCD8544(13,11,5,4,3); // optimal for hardware?

#include "keypad.h"
#include "mode_menu.h"
#include "mode_normal.h"

void setup() {

  display.begin();
  display.setContrast(50);

  // Show splash screen
  display.display();
  delay(500);
  display.clearDisplay();
  
  // This fixes the clock and syncs it up
  //ClockSetup();
  KeypadState keypadState = keypadGetState();
  modeNormalStart(keypadState);
}


void loop() {

  KeypadState keypadState = keypadGetState();

  if(keypadState.state == KEYPAD_STATE_DOWN || keypadState.state == KEYPAD_STATE_HOLD) {
    // extend timeout any time input key is pressed
    timeout = millis() + MODE_TIMEOUT;
  }

  if(MODE == MODE_NORMAL && keypadState.state == KEYPAD_STATE_DOWN) {
    // Any time a key is pressed during the normal mode, jump to the menu
    MODE = MODE_MENU;
    modeMenuStart(keypadState);
    return;
  } else if(MODE != MODE_NORMAL && millis() > timeout) {
    modeNormalStart(keypadState);
    MODE = MODE_NORMAL;
    return;
  }

  bool retainMode = false;
  if(MODE == MODE_NORMAL) {
    retainMode = normalLoop(keypadState);
  } else if(MODE == MODE_MENU) {
    retainMode = menuLoop(keypadState);
  }

  if(!retainMode && MODE != MODE_NORMAL) {
    MODE = MODE_NORMAL;
    modeNormalStart(keypadState);
  }
}

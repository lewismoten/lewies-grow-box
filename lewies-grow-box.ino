/*
 * Lewie's Grow Box
 * 
 * Create an automated growbox to assist with indoor gardening
 */

#define MODE_NORMAL 0
#define MODE_MENU 1
#define MODE_TIMEOUT 10 * 1000 // 10 seconds

#include "lcd.h";
#include "keypad.h";
#include "mode_normal.h";
#include "mode_menu.h";

byte MODE = MODE_NORMAL;
unsigned long timeout = 0;

void setup() {

  LcdInitialize();

  // I think this is changing analog pins to "virtual" digital pins
  // ie - A5 becomes pins 14-21
  DDRC |= _BV(2) | _BV(3);
  PORTC |= _BV(3);

  // This fixes the clock and syncs it up
  //ClockSetup();
 
}


void loop() {

  KeypadState keypadState = keypadGetState();

  if(keypadState.state == KEYPAD_STATE_DOWN || keypadState.state == KEYPAD_STATE_HOLD) {
    // extend timeout any time input key is pressed
    timeout = millis() + MODE_TIMEOUT;
  }
/*
  //LcdClear();
  LcdGoTo(0, 0);
  LcdWriteString("Test");
  
  LcdGoTo(0, 1);
  String l1 = "Key: ";
  l1 += keypadState.key;
  l1 += " ";
  LcdWriteString(l1);
  LcdGoTo(0, 2);
  String l2 = "Last Key: ";
  l2 += keypadState.lastKey;
  l2 += " ";
  LcdWriteString(l2);
  LcdGoTo(0, 3);
  String l3 = "State: ";
  l3 += keypadState.state;
  l3 += "  ";
  LcdWriteString(l3);
  return;
  */

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
  } else {
    String text = "MODE: ";
    text += MODE;
    text += " input: ";
    text += keypadState.key;
    text += "  ";
    
    LcdGoTo(0, 2);
    LcdWriteString(text);
  }

  if(!retainMode && MODE != MODE_NORMAL) {
    MODE = MODE_NORMAL;
    modeNormalStart(keypadState);
  }
}

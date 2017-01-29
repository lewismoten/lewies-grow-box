#include "lcd.h";
// TODO: Reasearch menu systems ....
// See https://www.youtube.com/watch?v=VtZvf5T98FI

void modeMenuStart(KeypadState keypadState) {
  LcdClear();
  LcdWriteInverted(true);
  LcdWriteString("     MENU     ");
  LcdWriteInverted(false);
  LcdWriteString("A Time        ");
  LcdWriteString("B             ");
  LcdWriteString("C             ");
  LcdWriteString("D             ");
  LcdWriteInverted(true);
  LcdWriteString("* Exit  # More");
  LcdWriteInverted(false);
}

bool menuLoop(KeypadState keypadState) {
  
  if(keypadState.state == KEYPAD_STATE_DOWN && keypadState.key == KEYPAD_KEY_STAR) {
    return false;
  }

  // stay in this mode
  return true;
}


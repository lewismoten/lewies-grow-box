
// TODO: Reasearch menu systems ....
// See https://www.youtube.com/watch?v=VtZvf5T98FI

void modeMenuStart(KeypadState keypadState) {

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 0);
  display.println("     MENU    ");
  display.setTextColor(BLACK, WHITE);
  display.println("A Time       ");
  display.println("B Contrast   ");
  display.println("C            ");
  display.println("D            ");
  display.setTextColor(BLACK, WHITE);
  display.println("* Exit # More");
  display.display();
}

bool menuLoop(KeypadState keypadState) {
  
  if(keypadState.state == KEYPAD_STATE_DOWN && keypadState.key == KEYPAD_KEY_STAR) {
    return false;
  }

  // stay in this mode
  return true;
}


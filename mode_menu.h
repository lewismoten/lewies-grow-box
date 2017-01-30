// TODO: Reasearch menu systems ....
// See https://www.youtube.com/watch?v=VtZvf5T98FI

int menu = 0;
uint8_t contrast_temp;
time_t time_temp;
String mask = "##:##:##";
String text = "";
int mask_pos = 0;
#include "settings.h"

void timeMenu(KeyPad keypad) {

  if(keypad.hasChanged) {
    if(keypad.isNumber) {
      text = text.substring(0, mask_pos) + keypad.label + text.substring(mask_pos + 1);

      // move to the next # or digit
      mask_pos++;
      if(mask_pos >= mask.length()) mask_pos = 0;
      char input = mask[mask_pos];
      while(input != '#') {
        mask_pos++;
        if(mask_pos >= mask.length()) mask_pos = 0;
        input = mask[mask_pos];
      }

    } else if(keypad.label == '#') {
      tmElements_t tm;
      if (RTC.read(tm)) {
        tm.Hour = text.substring(0, 2).toInt();
        tm.Minute = text.substring(3, 5).toInt();
        tm.Second = text.substring(6, 8).toInt();
        RTC.write(tm);
      }
      menu = 0;
    } else if(keypad.label == '*') {
      menu = 0;
    }
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(WHITE, BLACK);
  display.println("     Time     ");
  display.setTextColor(BLACK, WHITE);
  display.setCursor(15, 18);
  display.print(text.substring(0, mask_pos));
  display.setTextColor(WHITE, BLACK);
  display.print(text[mask_pos]);
  display.setTextColor(BLACK, WHITE);
  display.println(text.substring(mask_pos + 1));

  display.setCursor(0, 40);
  display.setTextColor(WHITE, BLACK);
  display.println("*CANCEL    #OK");
  display.display();  
}

void contrastMenu(KeyPad keypad) {

  if(keypad.hasChanged) {
    if(keypad.label == 'A') {
      contrast_temp++;
      if(contrast_temp > 0x7f) {
        contrast_temp = 0x7f;
      }
      display.setContrast(contrast_temp);
    } else if(keypad.label == 'B') {
      contrast_temp--;
      if(contrast_temp < 0) {
        contrast_temp = 0;
      }
      display.setContrast(contrast_temp);
    } else if(keypad.label == 'C') {
      contrast = contrast_temp;
      menu = 0;
    } else if(keypad.label == 'D') {
      display.setContrast(contrast);
      menu = 0;
    }
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(1, 0);
  display.println("Contrast");
  display.setTextSize(2);
  display.println(contrast_temp);
  display.setTextSize(1);
  display.println("A:UP B:DOWN");
  display.println("C:OK D:CANCEL");
  display.display();  
}
void mainMenu(KeyPad keypad) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 0);
  display.println("     MENU     ");
  display.setTextColor(BLACK, WHITE);
  display.println("A Time       ");
  display.println("B Contrast   ");
  display.println("C            ");
  display.println("D            ");
  display.setTextColor(BLACK, WHITE);
  display.println("* Exit  # More");
  display.display();
}
void modeMenuStart() {
  menu = 0;
}

bool menuLoop(KeyPad keypad) {

  if(keypad.hasChanged) {

    if(menu == 0) {
      if(keypad.label == '*') {
        return false;
      }
      if(keypad.label == 'A') {

        // get current time
        tmElements_t tm;
        if (RTC.read(tm)) {
          text = "";
          if(tm.Hour < 10) text += "0";
          text += tm.Hour;
          text += ":";  
          if(tm.Minute < 10) text += "0";
          text += tm.Minute;
          text += ":";
          if(tm.Second < 10) text += "0";
          text += tm.Second;
        } else {
          text = "00:00:00";
        }
  
        mask_pos = 0;
        mask = "##:##:##";
        menu = 1;
      }
      if(keypad.label == 'B') {
        contrast_temp = contrast;
        menu = 2;
        return true;
      }
    }
  }

  if(menu == 0) mainMenu(keypad);
  if(menu == 1) timeMenu(keypad);
  if(menu == 2) contrastMenu(keypad);

  return true;
}



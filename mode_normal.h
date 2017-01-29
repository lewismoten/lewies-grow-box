#include <Wire.h>
#include<Time.h>
#include <DS1307RTC.h>

#include "lcd.h";
#include "clock.h";

void modeNormalStart(KeypadState keypadState) {
  LcdClear();
}

bool normalLoop(KeypadState keypadState) {
  // get current time
  tmElements_t tm;
  time_t t;
  if (RTC.read(tm)) {
    t = makeTime(tm);
  }

  String text = "";

  // Line 1 - Day
  text = "";
  text += dayStr(weekday(t));
  LcdGoTo(0, 0);
  LcdWriteString(text);

  // Line 2 - Date  
  text = "";
  text += monthShortStr(tm.Month);
  text += " ";
  text += tm.Day;
  text += ", ";
  text += tmYearToCalendar(tm.Year);

  LcdGoTo(0, 1);
  LcdWriteString(text);
  
  // Line 3 - Time  
  text = "";
  text += hourFormat12(t);
  text += ":";
  if(tm.Minute < 10) text += "0";
  text += tm.Minute;
  text += ":";
  if(tm.Second < 10) text += "0";
  text += tm.Second;
  text += " ";
  text += isAM(t) ? "AM" : "PM";

  LcdGoTo(0, 2);
  LcdWriteString(text);

  return true;
}

#include <Wire.h>
#include<Time.h>
#include <DS1307RTC.h>

/*
 * Lewie's Grow Box
 * 
 * Create an automated growbox to assist with indoor gardening
 */

#include "lcd.h";
#include "clock.h";
#include "keypad.h";

void setup() {

  LcdInitialize();

//  Serial.begin(9600);
//  while(!Serial);
//  Serial.println("setting up!");

  delay(200);

  // I think this is changing analog pins to "virtual" digital pins
  // ie - A5 becomes pins 14-21
  DDRC |= _BV(2) | _BV(3);
  PORTC |= _BV(3);

  // This fixes the clock and syncs it up
  //ClockSetup();

}

void loop() {
  
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

  LcdGoTo(0, 4);
  LcdWriteString(" ");
  LcdGoTo(0, 4);
  LcdWriteString(keypadGetLabel());

  delay(250);

}

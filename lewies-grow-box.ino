/*
 * Lewie's Grow Box
 * 
 * Create an automated growbox to assist with indoor gardening
 */
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Time.h>
#include <DS1307RTC.h>

#define MODE_NORMAL 0
#define MODE_MENU 1
#define MODE_TIMEOUT 10 * 1000 // 10 seconds

byte MODE = MODE_NORMAL;
unsigned long timeout = 0;

#define LCD_CLK 8
#define LCD_DIN 9
#define LCD_DC 10
#define LCD_CE 11
#define LCD_RST 12
#define LCD_BACKLIGHT 6
// Clk, Din, DC, CE, RST
Adafruit_PCD8544 display = Adafruit_PCD8544(LCD_CLK, LCD_DIN, LCD_DC, LCD_CE, LCD_RST);
//Adafruit_PCD8544 display = Adafruit_PCD8544(13,11,5,4,3); // optimal for hardware?
#include "KeyPad.h"
#include "mode_menu.h"
#include "mode_normal.h"
#include "settings.h"

KeyPad keypad = KeyPad();

void setup() {

  pinMode(LCD_BACKLIGHT, OUTPUT);
  digitalWrite(LCD_BACKLIGHT, LOW);

  display.begin(contrast, 0x04);
  //display.setContrast(contrast);
  display.setTextWrap(false);

  // Show splash screen
  display.display();
  delay(500);
  // This fixes the clock and syncs it up
  //ClockSetup();
  modeNormalStart();
}

void loop() {

  keypad.update();

  if(keypad.isPressed) {
//    digitalWrite(LCD_BACKLIGHT, HIGH);
    // extend timeout any time input key is pressed
    timeout = millis() + MODE_TIMEOUT;
  }

  if(MODE == MODE_NORMAL && keypad.hasChanged && keypad.isPressed) {
    // Any time a key is pressed during the normal mode, jump to the menu
    MODE = MODE_MENU;
    modeMenuStart();
    return;
  } else if(MODE != MODE_NORMAL && millis() > timeout) {
    digitalWrite(LCD_BACKLIGHT, LOW);
    modeNormalStart();
    MODE = MODE_NORMAL;
    return;
  }

  bool retainMode = false;
  if(MODE == MODE_NORMAL) {
    retainMode = normalLoop();
  } else if(MODE == MODE_MENU) {
    retainMode = menuLoop(keypad);
  }

  if(!retainMode && MODE != MODE_NORMAL) {
    MODE = MODE_NORMAL;
    modeNormalStart();
  }
}

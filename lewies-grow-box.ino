/*
 * Lewie's Grow Box
 * 
 * Create an automated growbox to assist with indoor gardening
 */

#include "lcd.h";

void setup() {

  LcdInitialize();

}

void loop() {

  counter++;

  // Create text to display
  String text = "Loop #";
  text += counter;

  // Indicate that display should start writing at the top left corner
  LcdGoTo(0, 0);

  // Write out the text
  LcdWriteString(text);

  delay(500);

}

// Lewie's Grow Box
#ifndef LCD_H
#define LCD_H

// Connect a Nokia5110 LCD Module:

// RST to PIN 12 (pulse to reset module)
#define LCD_PIN_RST 12

// CE to PIN 11 (chip enable: this lets us send multiple bits over one pin)
#define LCD_PIN_CE 11

// Dc to PIN 10 (Data/Command: This lets us say if we are sending a command or data)
#define LCD_PIN_DC 10

// Din to PIN 9 (Serial data input - the actual data and/or command is sent on this pin)
#define LCD_PIN_DIN 9

// Clk to PIN 8 (Serial clock - this is how we tell the module that another bit is being sent)
#define LCD_PIN_CLK 8

// Vcc to nothing why does the module still work without power? this doesn't make sense...
// BL to 3.3v (backlight - optional)
// Gnd to GND (Ground... no doubt about it)

// ---------------------------------------------------------

#define LCD_SEND_COMMAND LOW
#define LCD_SEND_DATA HIGH

#define LCD_COLUMNS 84
#define LCD_ROWS 6

// Normal commands
#define LCD_COMMAND_NORMAL 0x20         // 0010 0000

// Display mode 00001D0E where bits D/E are...
#define LCD_COMMAND_DISPLAY 0x8 // 0000 1d0e
#define LCD_DISPLAY_OFF 0x0    // 0000 0000
#define LCD_DISPLAY_ON 0x1     // 0000 0001
#define LCD_DISPLAY_INVERT 0x5 // 0000 0101
#define LCD_DISPLAY_NORMAL 0x4 // 0000 0100

// Move to x/y address
#define LCD_COMMAND_ADDRESS_Y 0x40      // 0100 0yyy (banks 0-5)
#define LCD_COMMAND_ADDRESS_X 0x80      // 1xxx xxxx (column 0-83)

// Extended commands
#define LCD_COMMAND_EXTENDED 0x21       // 0010 0001

// Temperature control 000001ab
#define LCD_XCOMMAND_TEMPERATURE_0 0x4   // 0000 0100
#define LCD_XCOMMAND_TEMPERATURE_1 0x5   // 0000 0101
#define LCD_XCOMMAND_TEMPERATURE_2 0x6   // 0000 0110
#define LCD_XCOMMAND_TEMPERATURE_3 0x7   // 0000 0111
#define LCD_XCOMMAND_VOLTAGE 0x40        // 01vv vvvv

// Bias system 0001 0abc
#define LCD_XCOMMAND_BIAS 0x10
#define LCD_BIAS_1_100 0x0
#define LCD_BIAS_1_80  0x1
#define LCD_BIAS_1_65  0x2
#define LCD_BIAS_1_48  0x3
#define LCD_BIAS_1_40  0x4
#define LCD_BIAS_1_24  0x5
#define LCD_BIAS_1_18  0x6
#define LCD_BIAS_1_10  0x7

#include "font.h";

int counter;
bool writeInverted = false;

// Tutorial Video https://www.youtube.com/watch?v=E6PUa4n2DnA
// Example code with a font http://playground.arduino.cc/Code/PCD8544
// Data Sheet for PCD8544: https://cdn-shop.adafruit.com/datasheets/pcd8544.pdf

void LcdWriteRaw(byte operation, byte payload) {
  digitalWrite(LCD_PIN_DC, operation);
  digitalWrite(LCD_PIN_CE, LOW);
  shiftOut(LCD_PIN_DIN, LCD_PIN_CLK, MSBFIRST, payload);
  digitalWrite(LCD_PIN_CE, HIGH);  
}

void LcdWriteCmd(byte cmd) {
  LcdWriteRaw(LCD_SEND_COMMAND, cmd);
}
void LcdWriteInverted(bool isInverted) {
  writeInverted = isInverted;
}
void LcdWriteData(byte data) {
  if(writeInverted) {
    data = ~data;
  }
  LcdWriteRaw(LCD_SEND_DATA, data);
}
void LcdWriteCharacter(char character) {
  // A character is made up of 5 columns of 8 pixels each
  for(int i = 0; i < 5; i++) {
    // Our array starts at the first printable ascii character (0x20)
    LcdWriteData(ASCII[character - 0x20][i]);
  }
  LcdWriteData(0x00); // separating pixels
}

void LcdWriteString(String text) {
  for(int i = 0; i < text.length(); i++) {
    LcdWriteCharacter(text.charAt(i));
  }
}

void LcdWriteCharacters(char *characters) {
  while(*characters) {
    LcdWriteCharacter(*characters++);
  }
}
void LcdGoTo(int x, int y) {
  LcdWriteCmd(LCD_COMMAND_ADDRESS_X | x);
  LcdWriteCmd(LCD_COMMAND_ADDRESS_Y | y);
}
void LcdClear() {
  int addresses = LCD_COLUMNS * LCD_ROWS;
  for(int i = 0; i < addresses; i++) {
    // Clear out the current address
    LcdWriteData(0x00);
  }
  LcdGoTo(0, 0);
}

void LcdInitialize() {
  
  pinMode(LCD_PIN_RST, OUTPUT);
  pinMode(LCD_PIN_CE, OUTPUT);
  pinMode(LCD_PIN_DC, OUTPUT);
  pinMode(LCD_PIN_DIN, OUTPUT);
  pinMode(LCD_PIN_CLK, OUTPUT);
  
  // Send reset pulse
  digitalWrite(LCD_PIN_RST, LOW);
  digitalWrite(LCD_PIN_RST, HIGH);

  // Select extended instruction set
  LcdWriteCmd(LCD_COMMAND_EXTENDED);

  // Change contrast (34 and above causes a problem)
  // Note - I don't see any change regardless of what this value is
  LcdWriteCmd(LCD_XCOMMAND_VOLTAGE | 23);

  // Bias / Mutex ? What is this?
  LcdWriteCmd(LCD_XCOMMAND_BIAS | LCD_BIAS_1_40);

  // Select normal instruction set
  LcdWriteCmd(LCD_COMMAND_NORMAL);

  // Set normal mode (write black on blank)
  LcdWriteCmd(LCD_COMMAND_DISPLAY | LCD_DISPLAY_NORMAL);

  // Wipe out previous data
  LcdClear();

}

#endif

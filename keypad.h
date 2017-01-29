/*
 * This library was developed for a specific setup of a 4x4 keypad converted to an analog sinal
 * 
 * It's difficult to write a schematic in ASCII
 * Connect the keypad pins to the arduino with the
 * specified resistence to convert the keypad digital i/o
 * into analog values.
 * 
 * KEYPAD PIN 0 to 4.9k to ARDUINO PIN 1
 * KEYPAD PIN 1 to 2.7k to ARDUINO PIN 1
 * KEYPAD PIN 2 to 1.5k to ARDUINO PIN 1
 * KEYPAD PIN 3         to ARDUINO PIN 1
 * KEYPAD PIN 3 to 1.5K to ARDUINO GND
 * KEYPAD PIN 4 to 660  to ARDUINO 5v
 * KEYPAD PIN 5 to 440  to ARDUINO 5v
 * KEYPAD PIN 6 to 220  to ARDUINO 5v
 * KEYPAD PIN 7         to ARDUINO 5v
 * 
 * NOTE: You can add resistance by chaining resisitors
 * 3x 220 resisters
 * 2x 1.5k resistors
 * 2x 1.2k resistors
 * 
 * This is not a perfect digital i/o to analog way of doing things.
 * It's more of a hack. You can use any combinationof resisters as you
 * like. The important thing is to make sure you have a unique value
 * for each key, they are not zero, and that it is not too close to 
 * another keys value (including 0 - no keys pressed).
 * 
 * The analog signal tends to report a value that is often an exact value
 * but can deviate by 1 or 2. Use KEYPAD_BIAS to compensate
 */

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#ifndef AnalogKeyPad
#define AnalogKeyPad

class KeyPad {
  public:
    char label;
    int code;
    bool hasChanged;
    bool isPressed;
    int input;
    KeyPad();
    KeyPad(int pin, char* labels, int values[17]);
    void update();
  private:
    int pinInput;
    char* mapLabels;
    int mapValues[17];
};

#endif

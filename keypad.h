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

// This bias should be the minimum difference between two key values divided in half
// Our minimum value is between keys 1 and 2
//   309 - 300 = 9
//   9 / 2 = 4.5
//   round down = 4
#define KEYPAD_BIAS 4

#define KEYPAD_KEY_NONE 0

#define KEYPAD_KEY_1 1
#define KEYPAD_KEY_2 2
#define KEYPAD_KEY_3 3
#define KEYPAD_KEY_4 5
#define KEYPAD_KEY_5 6
#define KEYPAD_KEY_6 7

#define KEYPAD_KEY_7 9
#define KEYPAD_KEY_8 10
#define KEYPAD_KEY_9 11
#define KEYPAD_KEY_0 14

#define KEYPAD_KEY_A 4
#define KEYPAD_KEY_B 8
#define KEYPAD_KEY_C 12
#define KEYPAD_KEY_D 16

#define KEYPAD_KEY_STAR 13
#define KEYPAD_KEY_HASH 15

// This is the analog pin that the arduino will read from
#define KEYPAD_ANALOG_PIN 1

// These values may change based on
//  - Resistors used
//  - Keypad resistence
//  - Temperature
static const int keypadValues[] = {
  // Nothing
    0,

  // [1]  [2]  [3]  [A]
    300, 309, 319, 329,

  // [4]  [5]  [6]  [B]
    358, 371, 385, 400,

  // [7]  [8]  [9]  [C]
    443, 463, 485, 510,

  // [*]  [0]  [#]  [D]
    781, 846, 925, 1016  

};

// These are the labels for each of the keys on the 4x4 key pad
static const String keypadLabels[] = {
  "",

  "1", "2", "3", "A",
  
  "4", "5", "6", "B",
  
  "7", "8", "9", "C",
  
  "*", "0", "#", "D"

};

byte lastKey = KEYPAD_KEY_NONE;
bool keypadChanged = false;

struct KeypadState {
  String state;
  byte key;
  byte lastKey;
};

#define KEYPAD_STATE_NONE ""
#define KEYPAD_STATE_DOWN "down"
#define KEYPAD_STATE_HOLD "hold"
#define KEYPAD_STATE_UP "up"

KeypadState keypadState = {
  KEYPAD_STATE_NONE,
  KEYPAD_KEY_NONE,
  KEYPAD_KEY_NONE
};

/**
 * Determine if the analog value is close to the
 * value associated with the label
 * @param {int} targetValue The value we are expecting
 * @param {int} value The value we have
 * @returns {boolean} True if value is close to the targetValue; otherwise false
 */
boolean keypadIsNear(int targetValue, int value) {

  // How far off are we from the target value?
  int delta = abs(targetValue - value);

  // true if we close enough, otherwise false.
  return delta < KEYPAD_BIAS;

}

/**
 * map the value to teh apropriate key index
 * @param {int} value A value between 0 and 1023
 * @returns {byte} A key between 1 and 16; or 0 if nothing matched
 */
byte keypadMapValue(int value) {

  // Set our default value to nothing pressed
  byte key = KEYPAD_KEY_NONE;

  // Was something pressed?
  if(value != 0) {

    // Let's loop through all of our known key values
    for (int i = 0; i < sizeof(keypadValues); i++) {

      // Get the value of a known key vaue
      int knownValue = keypadValues[i];

      // Check to see if the value is near our key
      if (keypadIsNear(knownValue, value)) {

        key = i;

        // Stop checking all other keys
        break;

      }
    }
  }

  // here is the key that was pressed
  return key;
  
}

void updateState(byte key) {
  keypadState.lastKey = keypadState.key;
  keypadState.key = key;  

  if(keypadState.key == KEYPAD_KEY_NONE) {
    if(keypadState.lastKey == keypadState.key) {
      keypadState.state = KEYPAD_STATE_NONE;
    } else {
      keypadState.state = KEYPAD_STATE_UP;
    }
  } else if (keypadState.key == keypadState.lastKey) {
    keypadState.state = KEYPAD_STATE_HOLD;
  } else {
    keypadState.state = KEYPAD_STATE_DOWN;
  }

}

/**
 * Get the key that was pressed
 * @returns {signed byte} The key index that was pressed, otherwise -1
 */
byte keypadGetKey() {
  // Read the keypad
  int input = analogRead(KEYPAD_ANALOG_PIN);  


  // Return key that input represents
  byte key = keypadMapValue(input);

  updateState(key);

  return key;
}

KeypadState keypadGetState() {
  keypadGetKey();
  return keypadState;
}
  
/**
 * Get the pressed keys label
 * @returns {String} The pressed keys label, otherwise empty string
 */
String keypadGetLabel() {

  // What key does the input represent?
  byte key = keypadGetKey();

  // Return the keys label if we have it
  // otherwise nothing was pressed
  return key == KEYPAD_KEY_NONE ? " " : keypadLabels[key];

}


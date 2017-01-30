#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "KeyPad.h"

KeyPad::KeyPad() {
  pinInput = 1;
  mapLabels = " 123A456B789C*0#D";
  int values[17] = {0, 300, 309, 319, 329,358, 371, 385, 400, 443, 463, 485, 510, 781, 846, 925, 1016 };
  for(int i = 0; i < 17; i++) {
    mapValues[i] = values[i];
  }
  repeatRate = 250;
}
KeyPad::KeyPad(int pin, char* labels, int values[17]) {
  label = ' ';
  code = 0;
  hasChanged = false;
  isPressed = false;
  pinInput = pin;
  mapLabels = labels;
  for(int i = 0; i < sizeof(mapValues); i++) {
    mapValues[i] = values[i];
  }
  repeatRate = 250;
}

void KeyPad::update() {
  int lastCode = code;
  input = analogRead(pinInput);
  code = 0;
  for(int i = 0; i < sizeof(mapValues); i++) {
    if(abs(mapValues[i] - input) < 5) {
      code = i;
      break;
    }
  }
  label = mapLabels[code];
  isPressed = code != 0;
  hasChanged = code != lastCode;
  isNumber = label >= '0' && label <= '9';

  long time_now = millis();
  if(hasChanged) {
    started = time_now;
  } else if(isPressed && started + repeatRate < time_now) {
    // trigger change again (hold and repeat while holding)
    hasChanged = true;
    started = time_now;
  }
}



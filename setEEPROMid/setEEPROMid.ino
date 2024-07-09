#include <EEPROM.h>
byte id = 255;

void setup() {
  // put your setup code here, to run once:
// ID for pure data patches
// 0 == knock detector
// 1 == left fan
// 2 == right fan
// use the following to set the id for the board. Only do this the first time.
EEPROM.update(0,0);
}

void loop() {
  // put your main code here, to run repeatedly:

}

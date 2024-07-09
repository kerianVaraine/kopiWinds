#include <EEPROM.h>
byte id = 255;

char incomingChar;
bool newData = false;
bool connected = false;



void setup() {
// ID for pure data patches
// 0 == knock detector
// 1 == left fan
// 2 == right fan
// use the following to set the id for the board. Only do this the first time.
// EEPROM.update(0,1);

  Serial.begin( 9600 );

  while (!Serial) {
  //wait for serial connection
    }
  EEPROM.get(0, id);

  delay(5000);

  Serial.print("StoredID: ");
  Serial.println(id, 3);
}

void loop() {

  if (Serial.available()) {
    // wait for "go" string from pure data patch.
    receiveChar();
    if(connected == false){
      waitForConnection();
    }
  }

  if(connected){ 
    Serial.println("woohoo");
    delay(200);
  }

}

void receiveChar(){
  incomingChar = Serial.read();
  newData = true;
}

void waitForConnection() {
      if( newData == true ) {
        Serial.print("New Data: ");
        Serial.println(incomingChar);
        if(incomingChar == 'G') {
          connected = true;
          Serial.println("connected");
        }
        newData = false;
      }
}

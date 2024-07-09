#include <EEPROM.h>
//ID detection
byte id = 255;
char incomingChar;
bool newData = false;
bool connected = false;
  // delay for broadcasting ID
const int period = 1000;
unsigned long time_now = 0;

// Knock Sensor vars
const int knockSensor[2] = { A0, A2};
const int threshold = 30;
// vars for blocking hit detection for a period of time
// can be controlled at Puredata path via serial
const int lockTime = 50;
static uint32_t lastHitTime = 0;

int sensorReading[2] = {0, 0};

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    //wait for serial connection
   }
  // Get and Send initial ID number to Pure Data patch
  EEPROM.get(0, id);
  delay(5000);
  Serial.print("StoredID: ");
  Serial.println(id, 3);
}

void loop()
{
  
  // While not connected, send and check for PD to register COM port
  if (connected == false ){
    // look for char from Pure data
    receiveChar();
    waitForConnection();
    // delay before sending id again without blocking
    time_now = millis();
    // send ID
    Serial.print("StoredID: ");
    Serial.println(id, 3);
    while(millis() < time_now + period){
      // wait period ms
    }
  
  }  else{
  
  // iterate over contact mics to get value
  for (int i = 0; i < 2; i++)
  {
    // read twice to minimize interference
    sensorReading[i] = analogRead(knockSensor[i]);
    sensorReading[i] = analogRead(knockSensor[i]);

    if (sensorReading[i] >= threshold && millis() - lastHitTime > lockTime)
    {
      lastHitTime = millis();
      // replace with switch/case please
      if (i ==0) {
        Serial.print("left");
      }
      if (i == 1) {
        Serial.print("right");
      }

      Serial.print(" ");
      Serial.println(sensorReading[i]);
    }
  }
  delay(1);
  }
}

// ID checking functions
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
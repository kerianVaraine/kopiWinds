// for use with the testing.pd puredata patch.
#include <FastLED.h>
#include <EEPROM.h>
// EEPROM ID detection
byte id = 255;
char incomingChar;
bool newData = false;
bool connected = false;
  // delay for broadcasting ID
const int period = 1000;
unsigned long time_now = 0;

// Fan driver specific
int PWM_out_pin = 9; // pin to fan PWM
int control_byte = 0; // wind speed 0-255

#define LED_CHIPSET WS2812B
#define COLOR_ORDER GRB

#define NUM_FAN_LEDS 6
#define FAN_DATA_PIN 10

#define NUM_FRONT_LEDS 8
#define FRONT_DATA_PIN 6

#define BRIGHTNESS 255
#define FRAMES_PER_SECOND 120

CRGB fan_leds[NUM_FAN_LEDS];
CRGB front_leds[NUM_FRONT_LEDS];

void setup() {
  delay(2000);
  pinMode(PWM_out_pin, OUTPUT);
  setFanSpeed(255);
  LEDS.addLeds<LED_CHIPSET, FRONT_DATA_PIN, COLOR_ORDER>(front_leds, NUM_FRONT_LEDS);
  LEDS.addLeds<LED_CHIPSET, FAN_DATA_PIN, COLOR_ORDER>(fan_leds, NUM_FAN_LEDS);
  FastLED.setBrightness(BRIGHTNESS);    
// Fill LED colors
  for(int n = 0; n < NUM_FAN_LEDS; n++){
    fan_leds[n] = CRGB::Red;
  }
  for(int n = 0; n < NUM_FRONT_LEDS; n++){
    front_leds[n] = CRGB::Red;
  } 

  // Serial connect and broadcast ID at EEPROM[0]
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

uint8_t gHue = 96;  // rotating "base color" used by many of the patterns

void loop() {
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
    if (Serial.available()) {
      control_byte = byte(Serial.read()); // Serial.read() returns int, note.
      setFanSpeed(control_byte);
      FastLED.setBrightness(control_byte);
    }
  }

    //  rainbow();
    // //    // do some periodic updates
    //  EVERY_N_MILLISECONDS(20) {
    //  gHue++;
    //  }

  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}




void rainbow() {
  // FastLED's built-in rainbow generator
  fill_rainbow(fan_leds, NUM_FAN_LEDS, gHue, 5);
  fill_rainbow(front_leds, NUM_FRONT_LEDS, gHue, 5);

}

/**
 * Sets the speed of a fan using PWM (Pulse Width Modulation).
 *
 * @param speed The speed of the fan to be set. Must be a value between 0 and 255.
 *
 * @return void
 *
 * @throws None
 */
void setFanSpeed(byte speed) {
  speed = constrain(speed, 0, 255);
  analogWrite( PWM_out_pin, speed);
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
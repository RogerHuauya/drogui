#include "MM7150.h"
uint8_t state = 0;
uint8_t result;
accel acc;
void setup() {
  pinMode(LED_BUILTIN,OUTPUT); // LED
  pinMode(20,OUTPUT);
  pinMode(PIN_INT, INPUT);
  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000); // Wire bus, SCL pin 19, SDA pin 18, ext pullup, 400kHz
  digitalWrite(LED_BUILTIN, LOW);
  attachInterrupt(digitalPinToInterrupt(PIN_INT), IntPinHandler, CHANGE);
  delay(1000);
  result = VREG_init();                // Initialize VREG registers
  digitalWrite(LED_BUILTIN, HIGH);

  acc.init();
}

void loop() {
 
  acc.read();
  delay(1000);
}

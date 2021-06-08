#include "utils.h"
#include "i2c.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initI2C(SLAVE, I2C1, 0x60);
  clearI2Cregisters(I2C1);
}

void loop() {
  setReg(0x05, 15.28);
  // put your main code here, to run repeatedly:
  delay(1000);
}

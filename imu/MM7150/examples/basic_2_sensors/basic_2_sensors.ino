#include <MM7150.h>
uint8_t state = 0;
uint8_t result;
accel acc;
orient ori;
char out[40];
void setup() {
  pinMode(LED_BUILTIN,OUTPUT); // LED
  pinMode(17,OUTPUT);
  pinMode(PIN_INT, INPUT);
  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000); // Wire bus, SCL pin 19, SDA pin 18, ext pullup, 400kHz
  digitalWrite(LED_BUILTIN, LOW);
  attachInterrupt(digitalPinToInterrupt(PIN_INT), IntPinHandler, CHANGE);
  delay(1000);
  result = VREG_init();                // Initialize VREG registers
  digitalWrite(LED_BUILTIN, HIGH);

  acc.init(100,20);
  ori.init(50,200);
}

void loop() {
  /*acc.read();
  ori.read();
  sprintf(out, "Acc:: X: %4.3f Y: %4.3f Z: %4.3f",acc.dDataX, acc.dDataY, acc.dDataZ);
  Serial.println(out);
  sprintf(out, "Ori:: X: %4.3f Y: %4.3f Z: %4.3f W: %4.3f",ori.dDataX, ori.dDataY, ori.dDataZ, ori.dDataW);
  Serial.println(out);
  delay(1);*/
}
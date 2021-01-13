#include <MM7150.h>
uint8_t state = 0;
uint8_t result;
gyro gyr;
inclin inc;
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

  gyr.init(100,200);
  inc.init(100,200);
}

void loop() {
  gyr.read();
  inc.read();
  sprintf(out, "gyr:: X: %4.3f Y: %4.3f Z: %4.3f",gyr.dDataX, gyr.dDataY, gyr.dDataZ);
  Serial.println(out);
  sprintf(out, "inc:: X: %4.3f Y: %4.3f Z: %4.3f",inc.dDataX, inc.dDataY, inc.dDataZ);
  Serial.println(out);
  delay(1);
}

#include <Arduino.h>
#include <i2c_driver.h>
#include <i2c_driver_wire.h>

enum I2C_PORTS {I2C1, I2C2, I2C4};
enum I2C_MODE {MASTER, SLAVE};
enum I2C_ERR {SUCCESS, IDLE_ERROR, ACK_ERROR, COL_ERROR, TRM_ERROR, \
                    RCV_ERROR, TIMEOUT_ERROR, TMR_FULL_ERROR, RBF_ERROR};

#define I2C_REG_SIZE 300
uint8_t i2c1Reg[I2C_REG_SIZE];
uint8_t i2c2Reg[I2C_REG_SIZE];

int ptr = 0, cnt = 0;

void clearI2Cregisters(int n){
    if(n == I2C1) for(int i = 0; i < I2C_REG_SIZE; i++) i2c1Reg[i] = 0;     
    else if(n == I2C2) for(int i = 0; i < I2C_REG_SIZE; i++) i2c2Reg[i] = 0;
     
}


void requestI2C1(){
  
  Serial.println("request");
  Wire1.write(i2c1Reg[ptr]);     // respond with message of 6 bytes
  cnt++; i2c1Reg[ptr] = cnt;
}

void receiveI2C1(int reg){
  
  Serial.println("receive");
    ptr = Wire1.read();
    if(Wire1.available()){
      i2c1Reg[ptr] = Wire1.read();
    }
}

void initI2C(int mode,int n, int adress){
    if(n == I2C1){
        Wired1.begin(adress);
        Wired1.onRequest(requestI2C1);
        Wired1.onReceive(receiveI2C1); 
    }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initI2C(SLAVE, I2C1, 20);
}

void loop() {
  Serial.println("Hola mundo");
  // put your main code here, to run repeatedly:
  delay(500);
}

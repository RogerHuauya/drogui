import serial
from time import sleep

ser = serial.Serial("COM3",115200)
ser.flushInput()
print("AccX\tAccY\tAccZ\tGyrX\tGyrY\tGyrZ");
while True:
    if(ser.inWaiting()):
        try:
            print(ser.readline().decode(), end = "")
        except:
            pass
    sleep(0.001)
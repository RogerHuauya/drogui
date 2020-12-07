import serial
from time import sleep

ser = serial.Serial("COM11",115200)
ser.flushInput()
print("AccX\tAccY\tAccZ\tOriW\tOriX\tOriY\tOriZ\n");
while True:
    if(ser.inWaiting()):
        try:
            print(ser.readline().decode(), end = "")
        except:
            pass
    sleep(0.001)
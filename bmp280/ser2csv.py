import serial
from time import sleep

ser = serial.Serial("COM5",9600)
ser.flushInput()
while True:
    if(ser.inWaiting()):
        try:
            print(ser.readline().decode(), end = "")
        except:
            pass
    sleep(0.01)
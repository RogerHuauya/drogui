
import serial

serialP = serial.Serial("/dev/ttyACM0",9600)
serialP.flush()
data = ""
while(1):
	print("hola2")
	#if( serialP.in_waiting > 0 ):
	#print("hola")	
	data = serialP.readline().decode('utf-8').rstrip()
	print(data)
	#print(data.decode("UTF-8"))
	file = open("data.txt","a")
	file.write(data)
	#file.write(data.decode("UTF-8")[:-1])
	file.close()



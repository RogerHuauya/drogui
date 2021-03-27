import serial
import time
import numpy as np 
import matplotlib.pyplot as plt
ser = serial.Serial('/COM6',115200)
ser.flushInput()

rec_buff = ''
rec_buff2 = ''
time_count = 0

def send_at(command,back,timeout):
	rec_buff = ''
	ser.write((command+'\r\n').encode())
	time.sleep(timeout)
	if ser.inWaiting():
		time.sleep(0.01)
		rec_buff = ser.read(ser.inWaiting())
	if rec_buff != '':
		if back not in rec_buff.decode():
			#print(command + ' ERROR')
			#print(command + ' back:\t' + rec_buff.decode())
			return 0, ''
		else:
			#print(rec_buff.decode())
			return 1, rec_buff.decode()
	else:
		print('No response from GPS')
		return 0, ''

def get_gps_position():
	rec_null = True
	answer = 0
	print('Start GPS session...')
	rec_buff = ''
	send_at('AT+CGPS=1,1','OK',1)
	time.sleep(2)
	x_pos = []
	y_pos = []
	while rec_null:
		answer, rec_buff = send_at('AT+CGPSINFO','+CGPSINFO: ',1)
		if answer == 1:
			answer = 0
			if ',,,,,,,,' in rec_buff:
				print('GPS is not ready')
				rec_null = False
				time.sleep(1)
			else:
				#print(rec_buff)
				raw_coordinates = rec_buff.split(':')[1]
				coordinates = raw_coordinates.split(',')
				print(float(coordinates[0]), float(coordinates[2]))
				x_pos.append(float(coordinates[0]) - 1211)
				y_pos.append(float(coordinates[2]) - 7658)
				plt.axis([-1, 1, -1, 1])
				plt.scatter(x_pos, y_pos)
				plt.pause(0.01)
		else:
			print('error %d'%answer)
			rec_buff = ''
			send_at('AT+CGPS=0','OK',1)
			return False
		time.sleep(1.5)


try:
	get_gps_position()
except:
	if ser != None:
		ser.close()
if ser != None:
		ser.close()

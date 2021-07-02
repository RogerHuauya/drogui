import socket
import time
import threading
import struct

HOST = '192.168.0.12'  # The server's hostname or IP address
PORT = 8000       # The port used by the server
global msg
msg = ""
reg = 0
val = 0.5
def thread_function():
    global msg
    while True:
        msg = input()

x = threading.Thread(target=thread_function)
name = input('Input your name: ')
name = name + ": "
x.start()
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.settimeout(0.1)
    while True:
        if msg != "":
            print(msg)
            cmd = msg.split()[0] 
            if cmd == 'rg':
                reg, val = msg.split()[1:3]
                print(reg,val)
                ba = bytearray()
                ba.append(ord('d'))
                ba.append(ord('$'))
                ba.append(ord('w'))
                ba.append(int(reg))
                ba += bytearray(struct.pack("f", float(val)))  
                print(ba)
                s.sendall(ba)
            else:
                s.sendall(msg.encode())
        msg = ""
        data = ""
        try:
            data = s.recv(1024)
        except:
            pass
        if data != '':
            cin = list(data)
            print(chr(cin[0]),end='')
            print(chr(cin[1]), end='') 
            print(chr(cin[2]), end='') 
            print(cin[3:])
            print('Received', repr(data))

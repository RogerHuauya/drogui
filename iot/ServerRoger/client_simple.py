
import socket
import time
import threading

HOST = '191.98.175.75'  # The server's hostname or IP address
PORT = 1194        # The port used by the server
global msg
msg = ""

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
            s.sendall((name + msg).encode())
        msg = ""
        data = ""
        try:
            data = s.recv(1024)
        except:
            pass
        if data != '':
            print('Received', repr(data))

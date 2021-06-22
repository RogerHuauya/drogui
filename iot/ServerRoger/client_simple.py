
import socket
import time
import threading

HOST = '127.0.0.1'  # The server's hostname or IP address
PORT = 1024        # The port used by the server
global msg
msg = ""
def thread_function():
    global msg
    while True:
        msg = input()
x = threading.Thread(target=thread_function)
x.start()
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.settimeout(0.1)
    while True:
        s.sendall(msg.encode())
        msg = ""
        data = ""
        try:
            data = s.recv(1024)
        except:
            pass
        if data != '':
            print('Received', repr(data))
import socket
import struct

PLC_IP = "127.0.0.1"  # Replace with your PLC's IP address
PLC_PORT = 1234

print("Connecting...")
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((PLC_IP, PLC_PORT))
try:
    while True:
        sock.sendall(b"R")
        data = sock.recv(4)
        counter_value = struct.unpack(">I", data)[0]
        print(f"Counter value: {counter_value}")
except KeyboardInterrupt:
    print("Stopping...")
finally:
    sock.close()

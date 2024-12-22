import socket
import struct
import time

PLC_IP = "127.0.0.1"  # Replace with your PLC's IP address
PLC_PORT = 1234

def connect_to_plc():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((PLC_IP, PLC_PORT))
    return sock

def request_counter_value(sock):
    sock.sendall(b"R")
    data = sock.recv(4)
    return struct.unpack(">I", data)[0]

def main():
    print("Connecting...")
    sock = connect_to_plc()
    try:
        while True:
            counter_value = request_counter_value(sock)
            print(f"Counter value: {counter_value}")
            time.sleep(1)
    except KeyboardInterrupt:
        print("Stopping...")
    finally:
        sock.close()

if __name__ == "__main__":
    main()


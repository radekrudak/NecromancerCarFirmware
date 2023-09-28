#!/usr/bin/python3

import socket
import argparse
import time
#parser = argparse.ArgumentParser(description='Terminal program for comunication with ESP8266 controing RC model car')
#parser.add_argument('comand_for_car', metavar='CMD', type=str, required=False,
#                    help='string contaning coad to be sent to car')
#args = parser.parse_args()

#print(args.comand_for_car)


UDP_IP = "192.168.1.2"
UDP_PORT = 6969
MESSAGE = b"Hello, World!"
bufferSize = 1024

print("UDP target IP: %s" % UDP_IP)
print("UDP target port: %s" % UDP_PORT)
print("message: %s" % MESSAGE)

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

start_time = time.time()

sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
msgFromServer = sock.recvfrom(bufferSize)
end_time = time.time()

print(msgFromServer, "Ping+Pong: ", end_time-start_time)

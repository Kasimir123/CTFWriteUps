#!/usr/bin/env python2

import socket
import sys
import os
import struct
from ctypes import *
import threading
import select
import time

''''Procotol Services'''
bus_service = "\x01"
payload_service = "\x02"
telemetry_service = "\x03"
control_service = "\x04"
disconnect_service = "\x05"

'''Protocol fields'''
length = "\x12"
fragment_id = "\x00"
data = "\x00"*256

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

def connect(sock):
    server_address = ("192.168.7.17", 3210)
    sock.connect(server_address)

def transceive_packet(sock, service, length, fragment_id, data):
    count = 0
    if service==telemetry_service:
        while count < 12:
            fragment_id = struct.pack('>B',count)
            packet = service + length + fragment_id + data
            sock.send(packet)
            count = count+1
            time.sleep(0.2)#must keep this delay between packets for tranmission sync with satellite
    else:
        packet = service + length + fragment_id + data
        sock.send(packet)
        while count <=3:
            data = sock.recv(74)
            print data
            count = count+1
            time.sleep(0.2)#must keep this delay between packets for tranmission sync with satellite

connect(sock)
transceive_packet(sock, telemetry_service, length, fragment_id, data)
transceive_packet(sock, disconnect_service, length, fragment_id, data)

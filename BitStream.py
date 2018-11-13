import serial
import numpy as np

def getBitStream(size):
    """Get an array of 1's and 0's of length size"""
    ser = serial.Serial()
    ser.baudrate = 19200
    ser.port = 'COM5'
    ser.timeout = 0;
    ser.stopbits = ser.STOPBITS_ONE
    ser.bytesize = ser.EIGHTBITS
    print ser.portsrt       # what port was really used

    list_of_ints = list(ser.read(size=size/8))   # 8 bits per byte, ser reads byte
    list_of_bits = []
    for int in list_of_ints:
        str_byte = bin(int)
        str_bits = format(str_byte, "08b")
        for c in str_bits:
            list_of_bits.append(int(c));

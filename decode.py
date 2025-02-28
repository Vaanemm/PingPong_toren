from serial.tools.list_ports import comports
from serial import Serial
import time

ports = list(comports())
for p in ports:
    print(p)


ser = Serial('COM5', 9600, timeout=0.5)
if (ser.is_open):
    print("connectie geslaagd")
else:
    print("connectie gefaald")
print(ser)

print(ser.readline().decode('utf-8'))
time.sleep(1)
ser.write(f's200\n'.encode('ascii'))
print(ser.readline().decode('utf-8'))
time.sleep(1)
ser.close()
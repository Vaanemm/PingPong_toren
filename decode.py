from serial.tools.list_ports import comports
from serial import Serial
import time

ports = list(comports())
for p in ports:
    print(p)

#print ('t')


ser = Serial('/dev/cu.usbserial-A10JVCT6', 9600, timeout=0.5)
if (ser.is_open):
    print("connectie geslaagd")
else:
    print("connectie gefaald")
print(ser)

print(ser.readline().decode('utf-8'))
time.sleep(1)
ser.write(f's600\n'.encode('ascii'))
print(ser.readline().decode('utf-8'))
time.sleep(1)


while True:
    time.sleep(1)
    print(ser.readline().decode('utf-8'))

ser.close()
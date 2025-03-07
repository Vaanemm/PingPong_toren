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

targetGeraakt = False
ledGeraakt = False
getal_s = 800
#getal_p= 1.2375
#getal_i = 0.5;
#getal_d = 0.1

time.sleep(1)
ser.write(f's{getal_s}\n'.encode('ascii'))
#ser.write(f'i{getal_i}\n'.encode('ascii'))
#ser.write(f'd{getal_d}\n'.encode('ascii'))
#ser.write(f'p{getal_p}\n'.encode('ascii'))

while True:
    time.sleep(0.1)
    print(ser.readline().decode('utf-8'))
    if (targetGeraakt):
        ledGeraakt = True
        targetGeraakt = False

ser.close()


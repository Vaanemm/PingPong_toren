from serial.tools.list_ports import comports
from serial import Serial
import time



def doeDeAnimatie():
    ser.write(f'g\n'.encode('ascii'))
    #print("we zijn er")

num = 10000
cijfers = ["1", "2", "3", "4", "5", "6", "7", "8", "9"]
def isGeraakt(target, num):
    #print(type(target))
    #print(target)
    woord = ""
    for i, char in enumerate(lijn):
        if char == 't':
            num = i
        if (i > num and i< (num+4)):
            if (char in cijfers):
                woord += char
            else:
                woord += 0
    print(woord)
    if int(woord) > 100:
        return True
    else:
        return False

if __name__ == "__main__":
    ports = list(comports())
    for p in ports:
        print(p)

    # print ('t')

    ser = Serial('/dev/cu.usbserial-A10JVCT6', 9600, timeout=0.5)
    if (ser.is_open):
        print("connectie geslaagd")
    else:
        print("connectie gefaald")
    print(ser)

    targetGeraakt = True
    getal_s = 800
    # Target = 0
    # getal_p= 1.2375
    # getal_i = 0.5;
    # getal_d = 0.1

    time.sleep(1)
    #ser.write(f's{getal_s}\n'.encode('ascii'))
    # ser.write(f'i{getal_i}\n'.encode('ascii'))
    # ser.write(f'd{getal_d}\n'.encode('ascii'))
    # ser.write(f'p{getal_p}\n'.encode('ascii'))


    while True:
        time.sleep(0.1)
        #print(ser.readline().decode('utf-8'))
        lijn = ser.readline().decode('utf-8')
        print(lijn)
        if isGeraakt(lijn, num) == True:
            doeDeAnimatie()
            #targetGeraakt = False


    ser.close()
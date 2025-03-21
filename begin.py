from serial.tools.list_ports import comports
from serial import Serial
import time

ser = Serial('/dev/cu.usbserial-A10JVCT6', 9600, timeout=0.5)

def doeDeAnimatie():
    ser.write(f'g\n'.encode('ascii'))
    #print("we zijn er")

'''num = 10000
numB = 10000
numC = 10000
numD = 10000
numE= 10000 ho ho ho'''

cijfers = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]
alfabet = ["a", "b", "c", "d", "e", "f", "g", "h"]
def isGeraakt(num, numB, numC, numD, numE):
    targetWaarde = "0"
    target2Waarde = "0"
    target3Waarde = "0"
    target4Waarde = "0"
    countWaarde = ""
    lijn = ser.readline().decode('utf-8')
    for i, char in enumerate(lijn): 
        if char == 'a':
            num = i
        if ((i > num) and (char in cijfers)):
            #print("ervoor", num)
            targetWaarde += char
        if ((i > num) and (char in alfabet)):
            num = 1000
            #print("erna", num)
            
        if char == 'b':
            numB = i
        if ((i > numB) and (char in cijfers)):
            target2Waarde += char
        if ((i > numB) and (char in alfabet)):
             numB = 1000
          
            
        if char == 'c':
            numC = i
        if ((i > numC) and (char in cijfers)):
            target3Waarde += char
        if ((i > numC) and (char in alfabet)):
            numC = 1000
            
        if char == 'd':
            numD = i
        if ((i > numD and i< (numD+4)) and (char in cijfers)):
            target4Waarde += char
        if ((i > numD) and (char in alfabet)):
             numD = 1000
            
        if char == 'e':
            numE= i
        if (i == (numE+1)):
            countWaarde = char
        if ((i > numE) and (char in alfabet)):
             numE = 1000
             
    #print(f' 1 = {targetWaarde}, 2 is {target2Waarde}, 3 is {target3Waarde} en 4 is {target4Waarde}')      
            
    #print(targetWaarde, "en", target2Waarde)
    return ((int(targetWaarde) > 100), (int(target2Waarde)>400),(int(target3Waarde)>100),(int(target4Waarde)>100),int(countWaarde))

if __name__ == "__main__":
    ports = list(comports())
    for p in ports:
        print(p)

    # print ('t')
    
    
    if (ser.is_open):
        print("connectie geslaagd")
    else:
        print("connectie gefaald")
    print(ser)


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


    '''while True:
        time.sleep(0.1)
        #print(ser.readline().decode('utf-8'))
        lijn = ser.readline().decode('utf-8')
        #print(lijn)
        if isGeraakt(lijn, num) == True:
            doeDeAnimatie()'''



    ser.close()

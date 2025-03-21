""" IMPORTS """

import numpy as np
import cv2

from serial.tools.list_ports import comports
from serial import Serial
import time
from pygame import mixer
import pygame

from begin import doeDeAnimatie
from begin import isGeraakt

ser = Serial('/dev/cu.usbserial-A10JVCT6', 9600, timeout=0.5)

""" MAIN """

if __name__ == '__main__':

    webcam = True

    # open webcam video stream
    if webcam:
        cap = cv2.VideoCapture(0)
        
    else:
        import pyrealsense2 as rs
        
        # Configure streams
        pipeline = rs.pipeline()
        config = rs.config()

        config.enable_stream(rs.stream.depth, rs.format.z16, 30)
        other_stream, other_format = rs.stream.color, rs.format.bgr8
        config.enable_stream(other_stream, other_format, 30)

        # Start streaming
        pipeline.start(config)
        profile = pipeline.get_active_profile()


    # initialize the HOG descriptor/person detector
    hog = cv2.HOGDescriptor()
    hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())

    cv2.startWindowThread()

    # the output will be written to output.avi
    out = cv2.VideoWriter(
        'output.avi',
        cv2.VideoWriter_fourcc(*'MJPG'),
        15.,
        (640,480))
    
    
    
    
    
    
    
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

    
    num = 1000
    numB = 1000
    numC = 1000
    numD = 1000
    numE = 1000
    speeltdrum = False

    pygame.mixer.init()
    shoot = pygame.mixer.Sound("/Users/vladislavandruetan/Downloads/shoot-1-81135.mp3")
    klappen=pygame.mixer.Sound("/Users/vladislavandruetan/Downloads/crowd-applause-236697.mp3")
    muziek_drummen=pygame.mixer.Sound("/Users/vladislavandruetan/Downloads/cinematic-drums-146028.mp3")
    last_shot=pygame.mixer.Sound("/Users/vladislavandruetan/Downloads/mixkit-shatter-shot-explosion-1693.wav")
    
    shoot.set_volume(0.9)  #1.0 is max volume
    klappen.set_volume(1)
    muziek_drummen.set_volume(0.25)
    last_shot.set_volume(0.9)
    
    teRaken = 1
    ser.write('s110\n'.encode('ascii'))


    try:
        while(True):
            if webcam:
                # Capture frame-by-frame for computer webcam
                ret, frame = cap.read()
            
            else:
                frames = pipeline.wait_for_frames()
                color_frame = frames.get_color_frame()
                color_image = np.asanyarray(color_frame.get_data())
                    
            
                # resizing for faster detection
                frame = cv2.resize(color_image, (640, 480))
            
            # using a greyscale picture, also for faster detection
            gray = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)
        
            # detect people in the image
            # returns the bounding boxes for the detected objects
            boxes, weights = hog.detectMultiScale(gray, winStride=(8,8), hitThreshold=0.45)
        
            boxes = np.array([[x, y, x + w, y + h] for (x, y, w, h) in boxes])
            if len(boxes)>0:
                ser.write('x\n'.encode('ascii'))
            else:
                ser.write('z\n'.encode('ascii'))
            
        
            for (xA, yA, xB, yB) in boxes:
                # display the detected boxes in the colour picture
                cv2.rectangle(frame, (xA, yA), (xB, yB),
                                  (0, 255, 0), 2)
            
            # Write the output video
            out.write(frame.astype('uint8'))
            # Display the resulting frame
            cv2.imshow('frame',frame)
            
            
            
            
            
            lijn = ser.readline().decode('utf-8')
            #print(lijn)
            
            geraakt, geraakt2, geraakt3, geraakt4, countWaarde = isGeraakt(num, numB, numC, numD, numE)
            #print("Geraakt =", geraakt, "Geraakt2 =", geraakt2,"Geraakt3 =", geraakt3,"Geraakt4 =", geraakt4, "countWaarde =",countWaarde)
            
            if countWaarde == 1:
                #print("in de stopping")
                muziek_drummen.stop()
                speeltdrum = False
            elif geraakt == True and teRaken == 1:
                doeDeAnimatie()
                muziek_drummen.stop()
                shoot.play()
                speeltdrum = False
                teRaken = 2
                ser.write('s420\n'.encode('ascii'))
            elif geraakt2 == True and teRaken == 2:
                doeDeAnimatie()
                muziek_drummen.stop()
                shoot.play()
                speeltdrum = False
                teRaken = 4
                ser.write('760\n'.encode('ascii'))
            elif geraakt3 == True and teRaken == 3:
                print('niks')
            elif geraakt4 == True and teRaken == 4:
                doeDeAnimatie()
                muziek_drummen.stop()
                last_shot.play()
                speeltdrum = False        
                time.sleep(1)   
                klappen.play()
                time.sleep(2)
                teRaken = 1
                ser.write('s110\n'.encode('ascii'))
            elif (speeltdrum == False):
                #print('hehe')
                muziek_drummen.play()
                speeltdrum = True
             
        
                
             
            # Escape
            k = cv2.waitKey(1)
            if k == 27:
                break
            
    finally:
        if webcam:
            # When everything done, release the capture
            cap.release()
        else:
            pipeline.stop()
        # release the output
        out.release()
        # finally, close the window
        cv2.destroyAllWindows()

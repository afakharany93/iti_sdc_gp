import numpy as np
import math
from math import pi
from math import cos, sin, tan
import pickle
import cv2
import csv
import time
from sdc_serial import sdc_serial

import sys
import os
import platform
if platform.platform() == 'Linux-4.4.50-v7+-armv7l-with-debian-8.0':
    import subprocess
    subprocess.call(" ln -s /usr/local/lib/python3.4/dist-packages/cv2.so cv2.so", shell=True)		        
import cv2

def set_res(cap, x,y):
    cap.set(cv2.CV_CAP_PROP_FRAME_WIDTH, int(x))
    cap.set(cv2.CV_CAP_PROP_FRAME_HEIGHT, int(y))
    return str(cap.get(cv2.CV_CAP_PROP_FRAME_WIDTH)),str(cap.get(cv2.CV_CAP_PROP_FRAME_HEIGHT))

ser = sdc_serial('/dev/ttyACM0',9600) #arduino serial port definition
try:
	dir = time.strftime("%d-%m-%Y_%I:%M:%S")
	os.makedirs(dir, exist_ok=True)
	os.chdir(dir)
	os.makedirs('IMG', exist_ok=True)
	cam = cv2.VideoCapture(1)   # 0 -> index of camera
	#e,c = set_res(cam, 1280,720)
	#print(e,c)
	with open('data.csv', 'w', newline='') as csvfile:
		datawriter = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
		counter = int(0)
		while(True):
			s, img = cam.read()
			ser.flush()	#flush serial
			ser.update_serial() #update serial
			if s: # frame captured without any errors
				steering_angle = ser.get_steering()/100.0
				throttle = ser.get_throttle()/100.0
				ultrasonics = ser.get_us()

				img_name =  str(counter) + '.jpg'
				img_path = dir + '/'+'IMG'+'/' + img_name
				cv2.imwrite('IMG/'+img_name,img) #save image
				data = [img_path, steering_angle, throttle] + ultrasonics
				datawriter.writerow(data)
			counter += 1

finally:
	ser.exit()



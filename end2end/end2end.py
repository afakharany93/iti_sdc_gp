import sys
import os
import platform
import subprocess
if platform.platform() == 'Linux-4.9.32-v7+-armv7l-with-debian-8.0':
    subprocess.call(" ln -s /usr/local/lib/python3.4/dist-packages/cv2.cpython-34m.so cv2.so", shell=True)		        
import cv2

subprocess.call(" uvcdynctrl -d /dev/video0 -s \"Focus, Auto\" 0", shell=True)	
subprocess.call(" uvcdynctrl -d /dev/video0 -s \"Focus (Absolute)\" 0", shell=True)

from sdc_serial import sdc_serial
import cv2
import keras
from keras.models import load_model
import numpy as np
import time

width = 320	#new image width
hight = 160
dim = (width, hight)
cam = cv2.VideoCapture(0)
model = load_model('model_final.h5')
ser = sdc_serial('/dev/ttyACM0',9600)


while(true):
	if cam.isOpened(): # try to get the first frame
	    rval, img_center = cam.read()
	else:
	    rval = False
	count = 0
	while rval:
		start = time.time()
	    rval, img_center = cam.read()

		img_center = cv2.resize(cv2.cvtColor(img_center, cv2.COLOR_BGR2GRAY), dim, interpolation = cv2.INTER_AREA)-127.0 - 0.5
		

		img_center = np.reshape(img_center, (1,img_center.shape[0], img_center.shape[1],1))
		action = model.predict(img_center)
		steering = round(action[0][0][0])
		throttle = round(action[1][0][0])

		steering = min(max(steering,0),8)
		throttle = min(max(throttle,0),8)

		ser.send_data_serial(steering,throttle)
		end = time.time()
		print(count,' steer=',steering,' throt=', throttle,' elapsed=', end-start)


	    count+=1
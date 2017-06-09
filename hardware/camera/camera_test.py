import sys
import os
import platform
if platform.platform() == 'Linux-4.4.50-v7+-armv7l-with-debian-8.0':
    import subprocess
    subprocess.call(" ln -s /usr/local/lib/python3.4/dist-packages/cv2.so cv2.so", shell=True)		        
import cv2
# initialize the camera
cam = cv2.VideoCapture(0)   # 0 -> index of camera
s, img = cv2.cam.read()
if s:    # frame captured without any errors
    '''namedWindow("cam-test",CV_WINDOW_AUTOSIZE)'''
    #imshow("cam-test",img)
    #waitKey(0)
    #destroyWindow("cam-test")
    cv2.imwrite("test.jpg",img) #save image

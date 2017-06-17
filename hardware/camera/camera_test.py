import sys
import os
import platform
if platform.platform() == 'Linux-4.9.32-v7+-armv7l-with-debian-8.0':
    import subprocess
    subprocess.call(" ln -s /usr/local/lib/python3.4/dist-packages/cv2.cpython-34m.so cv2.so", shell=True)		        
import cv2

def set_res(cap, x,y):
    cap.set(3, int(x))
    cap.set(4, int(y))
    return str(cap.get(3)),str(cap.get(4))

# initialize the camera
cam = cv2.VideoCapture(0)   # 0 -> index of camera
e,c = set_res(cam,1280,720)
print(e,c)
s, img = cam.read()
if s:    # frame captured without any errors
    '''namedWindow("cam-test",CV_WINDOW_AUTOSIZE)'''
    #imshow("cam-test",img)
    #waitKey(0)
    #destroyWindow("cam-test")
    cv2.imwrite("test.jpg",img) #save image

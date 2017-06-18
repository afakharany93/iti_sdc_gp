import cv2


def set_res(cap, x,y):
    cap.set(3, int(x))
    cap.set(4, int(y))
    return str(cap.get(3)),str(cap.get(4))

cv2.namedWindow("preview")
vc = cv2.VideoCapture(2)
e,c = set_res(vc,1280,720)
print(e,c)
vc.set(int(39), 0) # turn the autofocus off, http://docs.opencv.org/trunk/d4/d15/group__videoio__flags__base.html#ggaeb8dd9c89c10a5c63c139bf7c4f5704da7c2fa550ba270713fca1405397b90ae00

if vc.isOpened(): # try to get the first frame
    rval, frame = vc.read()
else:
    rval = False
count = 0
while rval:
    cv2.imshow("preview", frame)
    #cv2.imwrite('IMG/'+str(count)+".jpg",frame) #save image
    rval, frame = vc.read()
    key = cv2.waitKey(20)
    if key == 27: # exit on ESC
        break
    count+=1
cv2.destroyWindow("preview")
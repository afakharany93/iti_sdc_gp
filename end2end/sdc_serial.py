import serial

class sdc_serial(object):
    #DATA RECV: 'START',us_r,us_m,us_l,steer,throttle, 'END'
    #DATA SEND: 'START',steer,throttle, 'END'
    #ex: 
    def __init__(self, port, baudrate):
        self.port = port
        self.baudrate = baudrate
        self.name=serial.Serial(self.port,self.baudrate)


    def update_serial(self):

        # while self.name.ser.in_waiting:  # Or: while ser.inWaiting():

        flag=0
        for i in range(0,10):
            data=self.name.readline()
            if 'Start' in str(data) :
                #print('found start in', i)
                #print(data)
                flag=1  
                break  
        if flag==1 :
            self.steer=self.name.readline()
            self.throttle=self.name.readline()
            self.us_r=self.name.readline()
            self.us_m=self.name.readline()
            self.us_l=self.name.readline()        

    def get_serial(self):
        
        data=self.name.readline()
        return data

    def get_right_ultrasonic(self):
        right_us = float(str(self.us_r)[2:-5])
        return right_us

    def get_left_ultrasonic(self):
        left_us = float(str(self.us_l)[2:-5])
        return left_us

    def get_middle_ultrasonic(self):
        middle_us = float(str(self.us_m)[2:-5])
        return middle_us

    def get_us(self):
        right =  self.get_right_ultrasonic()
        left = self.get_left_ultrasonic()
        middle = self.get_middle_ultrasonic()
        return [left, middle, right]

    def get_steering(self):
        steer = float(str(self.steer)[2:-5])    #casting from byte object to string, removing unwanted stuff from the frame, casting the number to float
        #print(steer)
        return steer 
    
    def get_throttle(self):
        throttle = float(str(self.throttle)[2:-5])
        return throttle

    #def conv_send(data):
    #    return bytes(str(data),"ascii")

    def set_serial(self,data):
        data_send=bytes(str(data),"ascii")
        self.name.write(data_send)

    def send_data_serial(self,steering_val,throttle_val):
        #self.name.write("Start")
        self.name.write(b's')   #115
        steering_send=bytes(str(steering_val),"ascii")
        self.name.write(steering_send)
        throttle_send=bytes(str(throttle_val),"ascii")
        self.name.write(throttle_send)
        self.name.write(b'e')   #101

    def flush(self):
        self.name.flush()

    def exit(self):
        self.name.close()



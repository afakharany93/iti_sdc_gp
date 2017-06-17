import serial

class sdc_serial(object):
    #DATA: 'START',us_r,us_m,us_l,steer,throttle, 'END'
    #ex: 
    def __init__(self, port, baudrate):
        self.port = port
        self.baudrate = baudrate
        self.name=serial.Serial(self.port,self.baudrate)


    def update_serial(self):

        # while self.name.ser.in_waiting:  # Or: while ser.inWaiting():

        flag=0
        for i in range(0,8)
            data=self.name.readline()
            if data=='START' :
                flag=1    
        if flag==1 :
            self.us_r=self.name.readline()
            self.us_m=self.name.readline()
            self.us_l=self.name.readline()
            self.steer=self.name.readline()
            self.throttle=self.name.readline()


    def get_serial(self):
        
        data=self.name.readline()
        return data

    def get_us(self)

        return self.us_r,self.us_m,self.us_l

    def get_steering(self)

        return self.steer
    
    def get_throttle(self)

        return self.throttle
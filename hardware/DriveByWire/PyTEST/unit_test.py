from sdc_serial import sdc_serial

import time
ser = sdc_serial('/dev/ttyACM0',9600)
count=0
ser.send_data_serial(4,8)
ser.exit()
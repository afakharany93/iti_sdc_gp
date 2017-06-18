from sdc_serial import sdc_serial

ser = sdc_serial('/dev/ttyACM0',9600)
for i in range(5):
	ser.flush()
	ser.update_serial()
	print(ser.get_serial())
	print(ser.get_us())
	print(ser.get_steering())
	print(ser.get_throttle())

ser.exit()
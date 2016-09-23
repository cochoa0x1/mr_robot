import serial
from struct import pack, unpack
import time

s = serial.Serial('/dev/ttyUSB1',115200)
s.timeout =1
s.write_timeout =1

#sudo chmod a+rw /dev/ttyUSB0 

def update_motors(v_left, v_right):

	d_left = v_left >= 0
	d_right = v_right >=0
	s_left = abs(int(v_left))
	s_right = abs(int(v_right))


	motor_packet = pack('>c?B?B','M',d_left,s_left,d_right,s_right)

	s.reset_output_buffer()
	s.write(motor_packet)


def read_ultrasonic():
	v= s.readline()
	s.reset_input_buffer()
	return float(v.strip('\r\n'))


def test_loop(t,V,VT,K):

	while(1):
		time.sleep(t)
		try:
			v = read_ultrasonic()
			print 'ultrasonic: ', v

			if v >  K:
				update_motors(V,V)
			else:
				update_motors(VT,-1*VT)

		except:
			print 'error'

		


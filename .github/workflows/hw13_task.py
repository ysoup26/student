# hw13 arduino-raspberry comm
# week11_task.ino
# IR sensor sensing

import serial
import time
import struct

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
print('wait until serial port is ready...')
   ## wait until serial is ready
time.sleep(1)

# vairalbes
NumOfData = 3         # Number of data to send to Arduino
dataSendReadyFlag = 1 # set dataSend flag

while 1:
        if dataSendReadyFlag:  #write(allways first)
                ser.write(b'A')
                dataSendReadyFlag=1
                time.sleep(0.2)  #0.2 delay comm

        if(ser.inWaiting() ==  NumOfData):
                dataSendReadyFlag=1 # set dataSendReady flag
                readBytes = ser.read(NumOfData)
                decode = struct.unpack_from('BBB',readBytes)
                count = decode[0]
                left_sensor_value=decode[1]
                right_sensor_value=decode[2]
                print ('count: {}, left: {}, right: {}'.format(count,left_sensor_value,right_sensor_value))

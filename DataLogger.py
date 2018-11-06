from serial import Serial
import time

ser = Serial('/dev/ttyACM0', 115200)

for x in range(0, 100, 5):
    ser.write(b"setBrightness(12,x)")
    time.sleep(.2)

#datetime,tempF,humidity,soilMoisture1,soilMoisture2,
#while 1:
	#if (ser.in_waiting > 0):
		#line = ser.readline()
		#print(line)

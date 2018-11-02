from serial import Serial
import time

ser = Serial('/dev/ttyACM0', 115200)

ser.write(b"tempF()")

#datetime,tempF,humidity,soilMoisture1,soilMoisture2,
while 1:
	if (ser.in_waiting > 0):
		line = ser.readline()
		print(line)

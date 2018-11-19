from serial import Serial
import time

ser = Serial('/dev/ttyACM0', 115200)

for x in range(0, 100, 5):
    ser.write(b"setBrightness(12,x)")
    time.sleep(.2)

log = open('log.csv', 'w')

while 1:
    ser.write(b"getTempF(2,4)")
    while 1:
        if (ser.in_waiting > 0):
            temp = ser.readline()
            break

    ser.write(b"getHumidity(2,4)")
    while 1:
        if (ser.in_waiting > 0):
            humidity = ser.readline()
            break

    ser.write(b"getSoilMoisture(2,4, 1.356)")
    while 1:
        if (ser.in_waiting > 0):
            soil1 = ser.readline()
            break

    ser.write(b"getSoilMoisture(2,4, 1.333)")
    while 1:
        if (ser.in_waiting > 0):
            soil2 = ser.readline()
            break

    print(str(time.time()) + "," + str(temp) + "," + str(humidity) + "," + str(soil1) + "," + str(soil2))
    time.sleep(1)




#datetime,tempF,humidity,soilMoisture1,soilMoisture2,

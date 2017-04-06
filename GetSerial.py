import serial
import time
import csv

sercon = serial.Serial("COM4",9600)

while True:
    received = sercon.readline().strip()
    received = received.split("_")
    datetime = time.strftime("%Y-%m-%d %H:%M:%S")
    received.append(datetime)
    #print received

    if received[0][0] == '!':
        #print "started"
        #print "sensor " + received[0][2] + " restarted at " + datetime
        with open("Log.txt", 'a') as logfile:
            logfile.write("sensor " + received[0][2] + " restarted at " + datetime + "\n")

    elif len(received) == 4:
        if received[0][0] is not "NAN":
            with open("SensorLog.csv", 'ab') as csvfile:
                filewrite = csv.writer(csvfile, delimiter=",")
                filewrite.writerow(received)

    #print "Done"

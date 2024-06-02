import serial.tools.list_ports
import numpy as np
import csv
import time
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation



fieldnames = ["x_val", "y_val", "y_val"]

ports = serial.tools.list_ports.comports()

serialInst = serial.Serial()
portList = []

for one in ports:
    portList.append(str(one))
    print(str(one))

com = input("Select a COMPORT")

for i in range(0,len(portList)):
    if portList[i].startswith("COM" + str(com)):
        portVar = "COM" + str(com)
        print(portVar)

serialInst.port = portVar
serialInst.baudrate = 115200
serialInst.open()

while True:

    #Sends command to Arduino to begin or end a scan
    command = input("Begin Scan(Y/N): ").lower()
    serialInst.write(command.encode('utf-8'))

    if command == 'exit':
        exit()
    #Reads the data sent by arduino 
    if serialInst.in_waiting:
        packet = serialInst.readline()
        """Checks if the correct three order of numbers have been received and stores them
            into a list.  
        """
        if packet.startswith('#') and packet.endswith('#'):
          data_string = packet[1:-1]
          coordListSpherical = data_string.strip().split(',')
          x_val = coordListSpherical[0]*np.sin(coordListSpherical[1])*np.cos(coordListSpherical[2])
          y_val = coordListSpherical[0]*np.sin(coordListSpherical[1])*np.sin(coordListSpherical[2])
          z_val = coordListSpherical[0]*np.cos(coordListSpherical[1])

          with open('data.csv', 'a') as csv_file:
            csv_writer = csv.DictWriter(csv_file, fieldnames = fieldnames)

            info = {
                
                    "x_val":x_val,
                    "y_val":y_val,
                    "z_val":z_val,
            }

            csv_writer.writerow(info)

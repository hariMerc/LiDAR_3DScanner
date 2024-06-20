'''
    This python code reads the r, theta, phi values sent by the esp32 and converts the values to
    x, y, and z and stores them into a .PCD file.
'''




import serial.tools.list_ports
import numpy as np
import open3d as o3d
from datetime import datetime

pc = o3d.geometry.PointCloud()
current_date_time = datetime.now()
filename ='Scan'+current_date_time.strftime("%Y%m%d_%H%M")+'.pcd'
fields = ["x_val", "y_val", "z_val"]
cartCoordList = []

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
    if serialInst.in_waiting:
        packet = serialInst.readline()
        data_string = packet.decode('utf-8')[1:-1]
        coordListSpherical = [float(x) for x in packet.decode('utf-8').strip().split(',')]

        x_val = coordListSpherical[0] * np.sin(coordListSpherical[2]*(np.pi/180)) * np.cos(coordListSpherical[1]*(np.pi/180))
        y_val = coordListSpherical[0] * np.sin(coordListSpherical[2]*(np.pi/180)) * np.sin(coordListSpherical[1]*(np.pi/180))
        z_val = coordListSpherical[0] * np.cos(coordListSpherical[2]*(np.pi/180))
        point = [x_val, y_val, z_val]
        pc.points.append(point)

        o3d.io.write_point_cloud(filename, pc)
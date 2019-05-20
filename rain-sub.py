# Import library
import paho.mqtt.subscribe as subscribe
import struct

# Variabel for subsribe parameter and save data
# hostname = "demo.thingsboard.io" # Using Thingsboard Live Demo Server
# TOKEN = ... # Use the token from your Thingsboard Live Demo Server
# PASS = ... # Usually empty if you use Thingsboard Live Demo Server

hostname = "127.0.0.1" # Using localhost
auth = {'username':TOKEN,'password':PASS}
port = 1883
topic = "v1/devices/me/telemetry"

dataSet = []
i = 0
total_intensity = 0

# Looping to get data and count the overall intensity
while True:
        message = subscribe.simple(topic, hostname=hostname, port=port, auth=auth)
        intensity = struct.unpack("i", message.payload)
        dataSet.append(intensity)
        print("Rain Intensity : %s" % intensity)

        total_intensity += intensity[0]
        total_data = len(dataSet)
        overall_intensity = total_intensity / total_data
        print("Overall : %s" % overall_intensity)
        
        i+=1

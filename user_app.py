# Import library
import paho.mqtt.subscribe as subscribe
import struct

# Inisiasi variabel untuk subscribe dan menyimpan data
# hostname = "demo.thingsboard.io"
hostname = "127.0.0.1"
auth = {'username':"4JjDZdXT0hIkvpgR5OwT",'password':""}
port = 1883
topic = "v1/devices/me/telemetry"

dataSet = []
i = 0
total_intensity = 0

# Perulangan untuk mendapatkan data dan menghitung rata-ratanya
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
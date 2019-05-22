# Import library
import paho.mqtt.subscribe as subscribe

# Variable initiation for subscribe and save data
# If get data from thingsboard
# hostname = "demo.thingsboard.io"
# auth = {'username':"4JjDZdXT0hIkvpgR5OwT",'password':""}
# topic = "v1/devices/me/telemetry"

# If get data from localhost using mosquitto
hostname = "127.0.0.1"
port = 1883
topic = "rain"

dataSet = []
i = 0
total_intensity = 0

# Loop to get and count data
while True:
#         If subscribe topic from thingsboard
#         message = subscribe.simple(topic, hostname=hostname, port=port, auth=auth)

#         If subscribe topic from localhost using mosquitto
        message = subscribe.simple(topic, hostname=hostname, port=port)
        data = message.payload
        intensity = int(data.decode())
        dataSet.append(intensity)
        print("Rain Intensity : %s" % intensity)

        total_intensity += intensity[0]
        total_data = len(dataSet)
        overall_intensity = total_intensity / total_data
        print("Overall : %s" % overall_intensity)
        
        i+=1
        

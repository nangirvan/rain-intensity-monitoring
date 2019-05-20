# rain-intensity-monitoring

 There are 2 file :
 
- BLE_SERVER_MQTT.ino : ESP32 have a role as Gateway that collect data from sensor (another ESP32) using BLE (Bluetooth Low Energy) then send them to cloud platform Thingsboard

- rain-sub.py : This is user app that consume data from Thingsboard then show them and count the overall intensity

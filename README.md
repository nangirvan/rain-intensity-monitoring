# rain-intensity-monitoring

ESP32 have a role as Sensor (ble_client) and Gateway (ble_server). They are connected using BLE (Bluetooth Low Energy). Sensor publish data to Gateway then pass it to cloud platform Thingsboard using MQTT or HTTP. User can get data by using (user_app) where it show data from sensor and count the average intensity.
You can see our demo at http://demo.thingsboard.io/dashboard/75844650-7af5-11e9-a31b-bbaebe3575d3?publicId=2e1fc150-6438-11e9-9abc-0b89750c9f1c

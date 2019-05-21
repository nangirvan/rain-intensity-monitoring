#include <BLEDevice.h>
#include <BLEServer.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

WiFiClient espClient;
PubSubClient client(espClient);
HTTPClient http;

long lastMsg = 0;
char msg[50];
int value = 0;
const char* server = "192.168.1.66";      // Broker IP Address
const char* ssid = "A-21";                // Access point name
const char* password = "secrean431038";   // Access point password


BLEServer* myServer = nullptr;
BLEService* myService = nullptr;
BLECharacteristic* myChar = nullptr;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void makeServer(){
  BLEDevice::init("Gateway");
  myServer = BLEDevice::createServer();
  myService = myServer->createService(SERVICE_UUID);
  myChar = myService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  myChar->setValue("Starting");
  
  myService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); 
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}
void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  setup_wifi();
  client.setServer(server,1883);
  client.setCallback(callback);
  makeServer();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  
  // start working...
  Serial.println("=================================");
  Serial.println("Rain Intensity...");
  
 std::string value = myChar->getValue();
      Serial.print("MSG: ");
      Serial.println(value.c_str());
    
  String str = value.c_str();
  char toSend [10];
  str.toCharArray(toSend,10);
  client.loop();
  
  // Publish data to thingsboard using mqtt
  client.publish("rain",toSend);
  
  // POST data to thingsboard using HTTP if you can't connect to thingsboard using mqtt
  String send_data = "{intensity:" + str + "}";
  http.begin("https://demo.thingsboard.io/api/v1/4JjDZdXT0hIkvpgR5OwT/telemetry");  // Access token
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(send_data);
  if(httpResponseCode>0){
    String response = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(response);
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  
  delay(3000);
}


void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
    }
    else if(messageTemp == "off"){
      Serial.println("off");
    }
  }
}

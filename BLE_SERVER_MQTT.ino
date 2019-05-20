#include <BLEDevice.h>
//#include <BLEUtils.h>
#include <BLEServer.h>
#include <ThingsBoard.h>

#include <WiFi.h>
#include <PubSubClient.h>


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

WiFiClient espClient;
PubSubClient client(espClient);

int dataSet [] = {380,510,260,340,590,500,480,490,330,450,380,390,360,210,490,230,435,350,580,430,540,445,445,900,510,500,600,600,620,640,320,510,330,490,520,770,320,630,590,320,440,570,420,250,720,400,515,590,400,290,395,600};
int idx = 0;
int sizeArr = 0;

long lastMsg = 0;
char msg[50];
int value = 0;
const char* server = "192.168.43.252";
const char* ssid = "Anonymous";
const char* password = "whoami??";
#define TB_SERVER     "demo.thingsboard.io"
#define TB_PORT       1883
#define TB_TOKEN      "4JjDZdXT0hIkvpgR5OwT"
#define TB_TOPIC      "v1/devices/me/telemetry"
ThingsBoard tb(espClient);


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
  sizeArr = sizeof(dataSet)/sizeof(dataSet[0]);

  setup_wifi();
  //client.setServer(server,1883);
  //client.setCallback(callback);
  makeServer();
  
}

void loop() {

 if(!tb.connected()){
    Serial.print("Connecting to: ");
    Serial.print(TB_SERVER);
    Serial.print( "With TOKEN: ");
    Serial.println(TB_TOKEN);
    if(!tb.connect(TB_SERVER, TB_TOKEN)){
      Serial.println("Failed to connect");
      return;
    }
  }
  // start working...
     Serial.println("=================================");
     Serial.println("Rain Intensity...");
  
    //std::string value = myChar->getValue();
     Serial.print("MSG: ");
    //    Serial.println(value.c_str());
     Serial.println(dataSet[idx]);
     idx++;
     if(idx==sizeArr){
        idx=0;
    }
    
   tb.sendTelemetryFloat("intensity", dataset[idx]);
  
  

   
  
  
  tb.loop();
  delay(2000);
  
  //String str= String(dataSet[idx]);
  //char toSend [10];
  //str.toCharArray(toSend,10);
  //client.loop();
  //client.publish("esp32/temp",toSend);
  //delay(1000);
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

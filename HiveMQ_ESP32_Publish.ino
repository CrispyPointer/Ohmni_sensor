#include <WiFi.h>
#include <PubSubClient.h>

// MQTT client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 
char *mqttServer = "broker.hivemq.com";
int mqttPort = 1883;
const char *c;
String results;
const char *SSID = "Thao";
const char *PWD = "Vinh0907682654@";
void connectToWiFi() {
  Serial.print("Connectiog to ");
 
  WiFi.begin(SSID, PWD);
  Serial.println(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected.");
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}

void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  // set the callback function
  mqttClient.setCallback(callback);
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  setupMQTT();
  while(!Serial){
  Serial.println(".");
  }
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      
      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
        // subscribe to topic
        mqttClient.subscribe("/swa/commands");
      }
      
  }
}

void loop() {
  
  while( Serial.available()>0){
      char n = Serial.read();
      results.concat(n);
      }
     c = results.c_str();
     Serial.println(c);
     Serial.println();                      
   if (!mqttClient.connected()){
    reconnect();
    mqttClient.loop();
    }
   mqttClient.publish("/swa/temperature/2304",c);
   c = 0;
   results = "";
   delay(1000);
  }

  // Hive MQ
  //Step 1: Run code
  //Step 2: Go to this link and click on connect button
  // Add the subscription topics one for each topic the ESP32 uses
  //http://www.hivemq.com/demos/websocket-client/

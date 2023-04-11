#include <WiFi.h>        // 引入WiFi庫
#include <PubSubClient.h> // 引入MQTT庫
#define WIFI_SSID "TrunkStudio-2.4G"        // WiFi名稱
#define WIFI_PASSWORD "22019020" // WiFi密碼

#define MQTT_SERVER "192.168.168.184" // MQTT伺服器的IP位址
#define MQTT_PORT 1883             // MQTT伺服器的埠口
#define MQTT_TOPIC "msg/test"          // MQTT主題名稱
#define MQTT_USERNAME "user"  // 請替換為您的MQTT使用者名稱
#define MQTT_PASSWORD "1234"  // 請替換為您的MQTT密碼

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect with username and password
    if (client.connect("ArduinoClient",  MQTT_USERNAME ,MQTT_PASSWORD)) {
      Serial.println("connected");
      // Once connected, subscribe to the topic(s) you want to receive
      client.subscribe(MQTT_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
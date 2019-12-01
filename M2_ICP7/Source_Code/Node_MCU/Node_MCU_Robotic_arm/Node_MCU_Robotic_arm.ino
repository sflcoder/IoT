#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
SoftwareSerial s(D2,D3); //Rx Tx
const char* ssid = "iDon'tKnow";
const char* password = "pimpleshwar1@108";
const char* mqtt_server = "broker.mqttdashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void send_2_arduino(String str){
  try_again: s.println(str); delay(100);
  while(1){
    if (s.available() >0){
    int check_flag = s.read();
    if (check_flag == 1){break;}
    else if (check_flag == 2 ){goto try_again;}
    }
  }
  Serial.println("msg_sent");
  snprintf (msg, 50, "msg_ack", value);
  client.publish("iot_roboticarm_app/app", msg);
}
void callback(char* topic, byte* payload, unsigned int length) {
  String incoming_msg = topic;
  incoming_msg+=char(':');
  for (int i = 0; i < length; i++) {
    incoming_msg+=((char)payload[i]);
  }
  Serial.println(incoming_msg);
  send_2_arduino(incoming_msg);
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "esp8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("iot_roboticarm_app/recv", "pub ready");
      // ... and resubscribe
      client.subscribe("iot_roboticarm_app/ext");
      client.subscribe("iot_roboticarm_app/rot");
      client.subscribe("iot_roboticarm_app/grb");
      client.subscribe("iot_roboticarm_app/hgt");
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
  Serial.begin(115200);
  s.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
/*long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 50, "hello world #%ld", value);
    client.publish("iot_roboticarm_app/app", msg);
  }*/
}

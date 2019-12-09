#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <Servo.h> 

Servo servo_bin1;
Servo servo_bin2;

const char* ssid = "ApnaTimeAyega";
const char* password = "GullyGirls@112";
//const char* ssid = "a";
//const char* password = "11111111";

const char* mqtt_server = "broker.mqttdashboard.com";

const int trigPin = D5;
const int echoPin = D6;
long duration=0;
int distance;

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

void callback(char* topic, byte* payload, unsigned int length) {
  String rec_topic = topic;
  String incoming_msg;
  Serial.println(rec_topic);
  for (int i = 0; i < length; i++) {
    incoming_msg+=((char)payload[i]);
  }
  Serial.println(incoming_msg);

  if (incoming_msg == "organic"){
    servo_bin1.write(90); 
    delay(3000);
    servo_bin1.write(180);
  }
  else if (incoming_msg == "recycle"){
    servo_bin2.write(0);
    delay(3000);
    servo_bin2.write(90);
  }
  
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
      //client.publish("iot_project_trash/bin", "pub ready");
      // ... and resubscribe
      client.subscribe("iot_project_trash/bin");
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
  //s.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  servo_bin1.attach(D1);  servo_bin1.write(0);
  servo_bin2.attach(D2);  servo_bin2.write(90);

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

   // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
//  Serial.println("Duration:");
//  Serial.println(duration);
  distance= (duration*0.034)/2;  //in cms
 // Serial.println("Distance:");
 // Serial.println(distance);
 
  if(distance < 20){
    snprintf (msg, 50, "Bin Full", value);
    Serial.println("\n Bin Full");
    client.publish("iot_project_trash/bin", msg);
  }
   
  
}

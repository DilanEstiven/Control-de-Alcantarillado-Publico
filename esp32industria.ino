#include <WiFi.h>
#include <PubSubClient.h>
#define LED_1 32
//#define LED_2 14

// Update these with values suitable for your network.

const int trigPin = 13;
const int echoPin = 12;
const char* ssid = "CableMas-FamiliaRios";//Red Wi-Fi a usar HUAWEI nova 9 SE
const char* password = "fra67732819";//fra67732819/dilan2006-
const char* mqtt_server = "192.168.101.17";//Dirección IP del broker (RPI 3B+)
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
const int freq=5000;
const int resolucion=8;
const int ledchannel=0; 
String auxiliar;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
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
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("Salida", "hello world!!");
      // ... and resubscribe
      client.subscribe("Entrada");
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
  pinMode(LED_1, OUTPUT);    
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  // Configura los pines del sensor de ultrasonido
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  ledcSetup(ledchannel, freq, resolucion);
}

void loop() {
  
long duration, distance;
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  
  // Calcula la distancia en centímetros
  distance = duration/58.2;//0.034 / 2;
  
  // Imprime la distancia en el monitor serial
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(500);
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "%ld ", distance);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("Salida", msg);
    Serial.println(auxiliar);
  }
}

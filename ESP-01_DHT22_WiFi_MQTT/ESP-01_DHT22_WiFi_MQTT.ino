// Get ESP8266 going with Arduino IDE
// - https://github.com/esp8266/Arduino#installing-with-boards-manager
// Required libraries (sketch -> include library -> manage libraries)
// - PubSubClient by Nick ‘O Leary
// - DHT sensor library by Adafruit

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define wifi_ssid "wifi net"
#define wifi_password "*xxxxxx"

#define mqtt_server "192.168.1.104"
//#define mqtt_user "pi"
//#define mqtt_password "$oklaM01."

#define humidity_topic "sensor/humidity"
#define temperature_topic "sensor/temperature"
#define led_topic "sensor/led"

#define DHTTYPE DHT22
#define DHTPIN  0

WiFiClient espClient;
PubSubClient client(espClient);

DHT dht(DHTPIN, DHTTYPE,11); // 11 works fine for ESP8266
float newTemp;
float newHum;

const int ledPin = 2;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC address: ");
  Serial.println(WiFi.macAddress());
  if (client.connect("sensor")) {
    // connection succeeded
    Serial.println("Connected ");
    boolean r= client.subscribe("led");
    Serial.println("subscribe ");
    Serial.println(r);
  } 
  else {
    // connection failed
    // mqttClient.state() will provide more information
    // on why it failed.
    Serial.println("Connection failed ");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  Serial.println(message);
  
  if (message == "1") {
    digitalWrite(ledPin, HIGH);
  } 
  else if (message == "0") 
  {
    digitalWrite(ledPin, LOW);
  } 
  else {
    Serial.println("Invalid message");
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you  want to use a username and password, change next line to
    //if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("sensor/led");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


bool checkBound(float newValue, float prevValue, float maxDiff) {
  return !isnan(newValue) &&
         (newValue < prevValue - maxDiff || newValue > prevValue + maxDiff);
}


void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  
  client.setServer(mqtt_server, 1883);
  // client.setClient (espClient);
  client.setCallback(callback);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  delay(3000);
  digitalWrite(ledPin, LOW);
}

long lastMsg = 0;
float temp = 0.0;
float hum = 0.0;
float diff = 0.1;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    newTemp = dht.readTemperature();
    newHum = dht.readHumidity();

    if (checkBound(newTemp, temp, diff)) {
      temp = newTemp;
      Serial.print("New temperature:");
      Serial.println(String(temp).c_str());
      client.publish(temperature_topic, String(temp).c_str(), true);
    }

    if (checkBound(newHum, hum, diff)) {
      hum = newHum;
      Serial.print("New humidity:");
      Serial.println(String(hum).c_str());
      client.publish(humidity_topic, String(hum).c_str(), true);
    }
  }
}

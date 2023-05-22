//https://www.youtube.com/watch?v=nVUC8mqdUlo&t=65s
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define STASSID "LZ_24G"
#define STAPSK  "*andromedA01."

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int relay = 0;

void setup(void) {
  pinMode(relay, OUTPUT);
  
  digitalWrite(relay, LOW);
  delay(2000);
  digitalWrite(relay, HIGH);
  
  Serial.begin(115200);
  Serial.print("setup");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/on", relayon);
  server.on("/off", relayoff);
  //server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}

void relayon() {  
  server.send(200, "text/plain", "relay on");  
  digitalWrite(relay, LOW);
}

void relayoff() {  
  server.send(200, "text/plain", "relay off");  
  digitalWrite(relay, HIGH);
}

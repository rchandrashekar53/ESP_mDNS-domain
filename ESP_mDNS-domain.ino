
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

#include <ESP8266WebServer.h>
#ifndef STASSID
#define STASSID "@@@@@@@@@@@@" // SSID of your network
#define STAPSK  "@@@@@@@@@@@@" // password of your network
#endif

#define DEVICE "myESP"
#define ID "01"

const char* ssid = STASSID;
const char* password = STAPSK;

// HTTP server at port 80 will respond to HTTP requests
ESP8266WebServer server(80);

void handleHTTPRequest() {
  Serial.println("");
  Serial.println("HTTP Request");

  IPAddress ip = WiFi.localIP();
  String ipStr = ip.toString();
  String s = "<!DOCTYPE HTML>\r\n<html><h3><head>Hello from ";
  s += WiFi.hostname() + ".local at " + server.client().localIP().toString() + "</h3></head>";

  Serial.println("Sending 200");
  server.send(200, "text/html", s);
  Serial.println("Done with request");
}

void setup(void) {
  Serial.begin(115200);

  // Connect to WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

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

  // Setup HTTP server
  server.on("/", handleHTTPRequest);
  // Start HTTP server
  server.begin();

  if (!MDNS.begin(DEVICE"_"ID)) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  MDNS.addService("http", "tcp", 80);
  MDNS.announce();
}

void loop(void) {

  MDNS.update();
  server.handleClient();
}

void AnnounceServices()
{
    MDNS.announce();
}

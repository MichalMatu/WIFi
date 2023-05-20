#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

// Replace with your network credentials
const char *ssid = "ESP32-Access-Point";
const char *password = "0123456789";

// Create an instance of the AsyncWebServer
AsyncWebServer server(80);

void setup()
{
  // Configure ESP32 as an access point
  WiFi.softAP(ssid, password);

  // Get IP address of the access point
  IPAddress ip = WiFi.softAPIP();

  // Initialize SPIFFS
  SPIFFS.begin(true);

  // Serve static files from SPIFFS
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  // Start the server
  server.begin();
}

void loop()
{
}

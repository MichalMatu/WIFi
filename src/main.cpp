#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

// Default network credentials
char ssid[32] = "ESP32AP";
char password[64] = "0123456789";

// Create an instance of the AsyncWebServer
AsyncWebServer server(80);

void setup()
{
  Serial.begin(115200);
  // Configure ESP32 as an access point
  WiFi.softAP(ssid, password);

  // Get IP address of the access point
  IPAddress ip = WiFi.softAPIP();

  // Initialize SPIFFS
  SPIFFS.begin(true);

  // Serve static files from SPIFFS
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server.on("/save-credentials", HTTP_POST, [](AsyncWebServerRequest *request)
            {
  String submittedSSID = request->arg("ssid");
  String submittedPassword = request->arg("password");

  // Check if the submitted SSID is not empty and the password has at least 8 characters
  if (submittedSSID.length() > 0 && submittedPassword.length() >= 8) {
    // Update the credentials
    strncpy(ssid, submittedSSID.c_str(), sizeof(ssid));
    strncpy(password, submittedPassword.c_str(), sizeof(password));

    // Print the updated credentials
    Serial.println("Updated SSID: " + String(ssid));
    Serial.println("Updated Password: " + String(password));

    request->send(200, "text/plain", "Credentials updated");
    // Disconnect any connected clients
    WiFi.softAPdisconnect(true);
    // Configure ESP32 as an access point with new credentials
    WiFi.softAP(ssid, password);

  } else {
    request->send(400, "text/plain", "Invalid credentials. SSID must not be empty and password must have at least 8 characters.");
  } });

  // Start the server
  server.begin();
}

void loop()
{
  // Your loop code here
}

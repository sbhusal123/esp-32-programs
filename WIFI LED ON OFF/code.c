#include <WiFi.h>
#include <WebServer.h>

// 1. Enter your Wi-Fi credentials here!
const char* ssid = "<ssid>";
const char* password = "<password>";

// 2. Define the LED Pin
const int LED_PIN = 2; 

// 3. Create a web server object on port 80 (standard HTTP port)
WebServer server(80);

// --- Functions to handle web requests ---

// What happens when someone visits the main page ("/")
void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<style>body { font-family: Arial; text-align: center; margin-top: 50px; } ";
  html += "a { display: inline-block; padding: 15px 30px; margin: 10px; text-decoration: none; font-size: 20px; color: white; border-radius: 5px; } ";
  html += ".btn-on { background-color: #4CAF50; } "; // Green
  html += ".btn-off { background-color: #f44336; } "; // Red
  html += "</style></head><body>";
  
  html += "<h1>ESP32 LED Control</h1>";
  html += "<a href=\"/led/on\" class=\"btn-on\">Turn ON</a>";
  html += "<a href=\"/led/off\" class=\"btn-off\">Turn OFF</a>";
  
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

// What happens when someone visits "/led/on"
void handleLedOn() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED turned ON via web");
  // Redirect back to the main page so the user sees the buttons again
  server.sendHeader("Location", "/");
  server.send(303); 
}

// What happens when someone visits "/led/off"
void handleLedOff() {
  digitalWrite(LED_PIN, LOW);
  Serial.println("LED turned OFF via web");
  // Redirect back to the main page
  server.sendHeader("Location", "/");
  server.send(303); 
}

// ----------------------------------------

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Start with LED off

  // Connect to Wi-Fi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Tell the server which functions to run for which URLs
  server.on("/", handleRoot);
  server.on("/led/on", handleLedOn);
  server.on("/led/off", handleLedOff);

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Listen for incoming web client requests
  server.handleClient();
}
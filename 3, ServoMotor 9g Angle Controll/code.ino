#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>

// --- WiFi Credentials ---
const char* ssid = "";
const char* password = "";

// --- Servo Configuration ---
int servoPin = 27;
Servo myServo;

// --- Web Server Setup ---
// Start a web server on port 80
WebServer server(80);

// --- HTML/CSS/JS Web Page ---
// We store the web page in program memory (PROGMEM) to save RAM
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Servo Control</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; background-color: #f4f4f9; color: #333;}
    .slider { width: 80%; max-width: 400px; height: 25px; background: #d3d3d3; outline: none; border-radius: 12px; }
    h1 { color: #444; }
  </style>
</head>
<body>
  <h1>ESP32 Servo Control</h1>
  <p>Current Angle: <span id="angleValue">180</span>&deg;</p>
  <!-- Slider goes from 0 to 180, default at 180 -->
  <input type="range" min="0" max="180" value="180" class="slider" id="servoSlider" oninput="updateSlider(this.value)">
  
  <script>
    function updateSlider(val) {
      // Update the text on the screen
      document.getElementById('angleValue').innerText = val;
      // Send the value to the ESP32 in the background
      fetch('/set?angle=' + val);
    }
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  // 1. Attach and set initial servo position
  myServo.attach(servoPin);
  myServo.write(180);

  // 2. Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // 3. Define Web Server Routes
  // What happens when someone visits the main IP address
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", index_html);
  });

  // What happens when the slider moves (e.g., visits /set?angle=90)
  server.on("/set", HTTP_GET, []() {
    if (server.hasArg("angle")) {
      String angleValue = server.arg("angle");
      int angle = angleValue.toInt();
      
      // Ensure the angle stays within valid bounds
      if (angle >= 0 && angle <= 180) {
        myServo.write(angle);
        Serial.print("Servo moved to: ");
        Serial.println(angle);
      }
    }
    server.send(200, "text/plain", "OK");
  });

  // 4. Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Listen for incoming clients
  server.handleClient();
}

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

const char* ssid = "YUT";  // Replace with your Wi-Fi SSID
const char* password = "654321RR";  // Replace with your Wi-Fi password

const int irSensorPins[] = {D3, D4, D5, D6};  // Parking slot sensors
bool slotOccupied[4];

const int entryIRSensorPin = D1;  // Entry sensor
const int exitIRSensorPin = D2;   // Exit sensor
const int entryServoPin = D7;     // Entry gate servo
const int exitServoPin = D8;      // Exit gate servo

Servo entryServo;
Servo exitServo;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Connecting to WiFi...");
  }
  Serial.println();
  Serial.println("Wi-Fi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());  // Print the IP address

  // Set up IR sensor pins
  for (int i = 0; i < 4; i++) {
    pinMode(irSensorPins[i], INPUT_PULLUP);
  }

  pinMode(entryIRSensorPin, INPUT_PULLUP);
  pinMode(exitIRSensorPin, INPUT_PULLUP);

  // Set up servos
  entryServo.attach(entryServoPin);
  exitServo.attach(exitServoPin);
  entryServo.write(90);  // Initial position (closed)
  exitServo.write(90);   // Initial position (closed)

  // Set up web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/status", HTTP_GET, handleStatus);

  // Start the web server
  server.begin();
}

void loop() {
  // Check parking slot occupancy
  for (int i = 0; i < 4; i++) {
    slotOccupied[i] = digitalRead(irSensorPins[i]) == LOW;
  }

  // Check if all parking slots are occupied
  bool allSlotsOccupied = true;
  for (int i = 0; i < 4; i++) {
    if (!slotOccupied[i]) {
      allSlotsOccupied = false;
      break;
    }
  }

  // Entry gate control
  int entrySensorValue = digitalRead(entryIRSensorPin);
  if (entrySensorValue == LOW && !allSlotsOccupied) { // Check if there is at least one free slot
    entryServo.write(0);   // Open entry gate
    delay(1000);
    entryServo.write(90);  // Close entry gate
  }

  // Exit gate control
  int exitSensorValue = digitalRead(exitIRSensorPin);
  if (exitSensorValue == LOW) {
    exitServo.write(0);    // Open exit gate
    delay(1000);
    exitServo.write(90);   // Close exit gate
  }

  // Handle web client requests
  server.handleClient();
}

void handleRoot() {
  String html = "<html lang=\"en\"><head>";
  html += "<meta charset=\"UTF-8\" />";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/>";
  html += "<title>Smart Parking System</title>";
  html += "<link href=\"https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;600&display=swap\" rel=\"stylesheet\"/>";
  html += "<style>";
  // Include the CSS from your earlier code here
  html += "body {font-family: \"Poppins\", sans-serif; text-align: center; background-color: #f4f4f4; margin: 0; padding: 0; display: flex; flex-direction: column; min-height: 100vh;} ";
  html += ".header { background: #2c9c4a; color: white; padding: 20px; font-size: 28px; font-weight: 600;} ";
  html += ".sub-header { font-size: 14px; font-weight: 300; color: #eee; margin-top: 5px; letter-spacing: 0.5px;} ";
  html += ".container { flex: 1; display: flex; flex-direction: column; align-items: center; justify-content: center; padding: 30px;} ";
  html += ".gate-container { display: flex; gap: 30px; margin-bottom: 40px; justify-content: center;} ";
  html += ".slot-container { display: flex; gap: 30px; justify-content: center; flex-wrap: nowrap; overflow-x: auto;} ";
  html += ".box { background: white; width: 200px; height: 100px; padding: 20px; border-radius: 10px; box-shadow: 0 5px 15px rgba(0, 0, 0, 0.1); font-size: 18px; font-weight: 600; display: flex; flex-direction: column; justify-content: center; align-items: center;} ";
  html += ".status { margin-top: 10px; font-size: 16px; font-weight: 400;} ";
  html += ".open { color: green;} .closed { color: red;} ";
  html += ".footer { background: #333; color: white; padding: 12px; text-align: center; font-size: 14px; margin-top: auto;} ";
  html += "</style>";
  html += "<script>";
  // Include the JavaScript from your earlier code here
  html += "function refreshStatus() {fetch(\"/status\").then((response) => response.json()).then((data) => {";
  html += "document.getElementById(\"entryGate\").innerText = data.entryGate ? \"Open\" : \"Closed\";";
  html += "document.getElementById(\"entryGate\").className = data.entryGate ? \"status open\" : \"status closed\";";
  html += "document.getElementById(\"exitGate\").innerText = data.exitGate ? \"Open\" : \"Closed\";";
  html += "document.getElementById(\"exitGate\").className = data.exitGate ? \"status open\" : \"status closed\";";
  html += "document.getElementById(\"slot1\").innerText = data.slot1 ? \"Occupied\" : \"Available\";";
  html += "document.getElementById(\"slot1\").className = data.slot1 ? \"status closed\" : \"status open\";";
  html += "document.getElementById(\"slot2\").innerText = data.slot2 ? \"Occupied\" : \"Available\";";
  html += "document.getElementById(\"slot2\").className = data.slot2 ? \"status closed\" : \"status open\";";
  html += "document.getElementById(\"slot3\").innerText = data.slot3 ? \"Occupied\" : \"Available\";";
  html += "document.getElementById(\"slot3\").className = data.slot3 ? \"status closed\" : \"status open\";";
  html += "document.getElementById(\"slot4\").innerText = data.slot4 ? \"Occupied\" : \"Available\";";
  html += "document.getElementById(\"slot4\").className = data.slot4 ? \"status closed\" : \"status open\";";
  html += "});} ";
  html += "setInterval(refreshStatus, 1000);";
  html += "</script>";
  html += "</head><body onload=\"refreshStatus()\">";
  html += "<div class=\"header\">Smart Parking System<div class=\"sub-header\">Manage your parking slots efficiently.</div></div>";
  html += "<div class=\"container\">";
  html += "<div class=\"gate-container\">";
  html += "<div class=\"box\">Entry Gate<div id=\"entryGate\" class=\"status\">Loading...</div></div>";
  html += "<div class=\"box\">Exit Gate<div id=\"exitGate\" class=\"status\">Loading...</div></div>";
  html += "</div><div class=\"slot-container\">";
  html += "<div class=\"box\">Slot 1<div id=\"slot1\" class=\"status\">Loading...</div></div>";
  html += "<div class=\"box\">Slot 2<div id=\"slot2\" class=\"status\">Loading...</div></div>";
  html += "<div class=\"box\">Slot 3<div id=\"slot3\" class=\"status\">Loading...</div></div>";
  html += "<div class=\"box\">Slot 4<div id=\"slot4\" class=\"status\">Loading...</div></div>";
  html += "</div></div><div class=\"footer\">© 2025 Smart Parking System | Designed by ASHISH</div></body></html>";

  server.send(200, "text/html", html);
}

void handleStatus() {
  String json = "{";
  json += "\"slot1\":" + String(slotOccupied[0]) + ",";
  json += "\"slot2\":" + String(slotOccupied[1]) + ",";
  json += "\"slot3\":" + String(slotOccupied[2]) + ",";
  json += "\"slot4\":" + String(slotOccupied[3]) + ",";

  bool allSlotsOccupied = true;
  for (int i = 0; i < 4; i++) {
    if (!slotOccupied[i]) {
      allSlotsOccupied = false;
      break;
    }
  }

  // Prevent entry gate status update if all slots are full
  if (allSlotsOccupied) {
    json += "\"entryGate\":false,";
  } else {
    json += "\"entryGate\":" + String(digitalRead(entryIRSensorPin) == LOW) + ",";
  }

  json += "\"exitGate\":" + String(digitalRead(exitIRSensorPin) == LOW);  
  json += "}";

  server.send(200, "application/json", json);
}

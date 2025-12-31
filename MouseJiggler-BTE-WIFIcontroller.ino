/**
 * ESP32 Bluetooth Mouse Jiggler Pro v5.0
 * ---------------------------------------
 * DESCRIPTION:
 * This project turns an ESP32 (S3/C3/Standard) into a Bluetooth HID Mouse.
 * It prevents a PC from going to sleep by moving the cursor at set intervals.
 * * FEATURES:
 * - Dual Mode: Acts as a BT Mouse for the PC and a Wi-Fi Access Point for control.
 * - Web Interface: Control via smartphone (192.168.4.1) with zero-refresh AJAX.
 * - Countdown Timer: Automatically stops the jiggler after a set time.
 * - Real-time Settings: Adjust movement distance, interval, and LED brightness.
 * - Non-Blocking: Uses millis() timing so the web server remains responsive.
 * - Visual Feedback: RGB LED indicates connection, movement, and standby states.
 */

#include <BleMouse.h>
#include <nvs_flash.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <NimBLEDevice.h>
#include "index.h" // Holds the HTML/CSS/JS string

// Hardware Definitions
#define PIN_NEOPIXEL 48  // Default RGB LED pin for ESP32-S3 (Change to 38 if needed)
#define NUMPIXELS     1
Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// Global Objects
BleMouse* bleMouse = nullptr; // Pointer for dynamic naming
WebServer server(80);
Preferences prefs;

// Config Variables
String wifiSSID, wifiPass, btName;
bool jigglerActive = false;
int brightness = 30;
int moveAmount = 2;
int intervalSec = 1;
unsigned long timerEndMillis = 0;
bool timerRunning = false;
unsigned long lastMoveTime = 0;

int getRSSI() {
  if (bleMouse == nullptr || !bleMouse->isConnected()) return 0;
  return -55; 
}

void handleStatus() {
  String json = "{";
  json += "\"active\":" + String(jigglerActive ? "true" : "false") + ",";
  json += "\"move\":" + String(moveAmount) + ",";
  json += "\"interval\":" + String(intervalSec) + ",";
  json += "\"bright\":" + String(brightness) + ",";
  json += "\"rssi\":" + String(getRSSI()) + ",";
  json += "\"timerRemaining\":" + String(timerRunning ? (long)(timerEndMillis - millis()) : 0) + ",";
  json += "\"ssid\":\"" + wifiSSID + "\",";
  json += "\"btname\":\"" + btName + "\"";
  json += "}";
  server.send(200, "application/json", json);
}


void handleUpdateConfig() {
  if (server.hasArg("ssid")) { prefs.putString("ssid", server.arg("ssid")); }
  if (server.hasArg("pass")) { prefs.putString("pass", server.arg("pass")); }
  if (server.hasArg("bt"))   { prefs.putString("bt", server.arg("bt")); }
  server.send(200, "text/plain", "OK. Restarting...");
  delay(1000);
  ESP.restart();
}

void handleRoot() { server.send(200, "text/html", PAGE_MAIN); }
void handleToggle() { jigglerActive = !jigglerActive; timerRunning = false; server.send(200, "text/plain", "OK"); }
void handleSetMove() { if (server.hasArg("px")) { moveAmount = server.arg("px").toInt(); prefs.putInt("move", moveAmount); } server.send(200, "text/plain", "OK"); }
void handleSetInterval() { if (server.hasArg("sec")) { intervalSec = server.arg("sec").toInt(); prefs.putInt("interval", intervalSec); } server.send(200, "text/plain", "OK"); }
void handleSetBrightness() { if (server.hasArg("val")) { brightness = server.arg("val").toInt(); pixels.setBrightness(brightness); pixels.show(); prefs.putInt("bright", brightness); } server.send(200, "text/plain", "OK"); }
void handleSetTimer() { if (server.hasArg("min")) { timerEndMillis = millis() + (server.arg("min").toInt() * 60000); timerRunning = true; jigglerActive = true; } server.send(200, "text/plain", "OK"); }

void setup() {
  Serial.begin(115200);
  pixels.begin();
  
  // Initialize Preferences
  prefs.begin("jiggler", false);
  moveAmount = prefs.getInt("move", 2);
  intervalSec = prefs.getInt("interval", 1);
  brightness = prefs.getInt("bright", 30);
  wifiSSID = prefs.getString("ssid", "ESP32-Mouse-Config");
  wifiPass = prefs.getString("pass", "12345678");
  btName = prefs.getString("bt", "ESP32-MouseJiggler");

  pixels.setBrightness(brightness);

  // 1. Setup WiFi
  WiFi.softAP(wifiSSID.c_str(), wifiPass.c_str());

  // 2. Setup Web Server
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/toggle", handleToggle);
  server.on("/setMove", handleSetMove);
  server.on("/setInterval", handleSetInterval);
  server.on("/setBrightness", handleSetBrightness);
  server.on("/setTimer", handleSetTimer);
  server.on("/updateConfig", handleUpdateConfig);
  server.begin();

  // 3. Initialize Bluetooth Mouse with Custom Name
  bleMouse = new BleMouse(btName.c_str(), "Espressif", 100);
  bleMouse->begin(); 

  Serial.println("Jiggler Ready.");
}

void loop() {
  server.handleClient();
  
  if (timerRunning && millis() > timerEndMillis) { jigglerActive = false; timerRunning = false; }

  // Safety check: ensure bleMouse is initialized
  if(bleMouse != nullptr && bleMouse->isConnected()) {
    if (jigglerActive) {
      if (millis() - lastMoveTime >= (intervalSec * 1000)) {
        pixels.setPixelColor(0, pixels.Color(0, 255, 0));
        pixels.show();
        bleMouse->move(moveAmount, 0);
        delay(100); 
        bleMouse->move(moveAmount * -1, 0);
        lastMoveTime = millis();
      }
    } else {
      pixels.setPixelColor(0, pixels.Color(30, 30, 30));
      pixels.show();
    }
  } else {
    // Blinking Red: Waiting for Bluetooth
    pixels.setPixelColor(0, (millis() / 500 % 2 == 0) ? pixels.Color(255, 0, 0) : pixels.Color(0, 0, 0));
    pixels.show();
  }
}
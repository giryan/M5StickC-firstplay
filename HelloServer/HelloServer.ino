#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <M5StickC.h>


const char* ssid     = "House of Geek v4.1";
const char* password = "Pa55w0rd";

WebServer server(80);

const int led = M5_LED;
int led_state = HIGH;

void handleRoot() {
  led_state != led_state;
  digitalWrite(led, led_state);
  server.send(200, "text/plain", "hello from M5StickC!");
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  M5.begin();
  M5.Lcd.fillScreen(WHITE);
    M5.Lcd.setTextColor(BLACK, WHITE);
    M5.Lcd.setCursor(7, 0, 2);
    
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  M5.Lcd.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("");
  M5.Lcd.print("Connected to ");
  M5.Lcd.println(ssid);
  M5.Lcd.print("IP address: ");
  M5.Lcd.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    M5.Lcd.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  M5.Lcd.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}

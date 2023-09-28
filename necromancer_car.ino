#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "OTA_setup.h"
#include "comand_interpreter.h"

//#define CONNECT_TO_WIFI
#define ACCES_POINT


#define STASSID "RadoslawPhone"
#define STAPSK "Turkistan31"

#define UDP_PORT 6969

#define ENGINE_PIN_A D1
#define ENGINE_PIN_B D2

#define STEERING_PIN_A D6
#define STEERING_PIN_B D5

#ifdef CONNECT_TO_WIFI
const char* ssid = STASSID;
const char* password = STAPSK;
#endif
#ifdef ACCES_POINT
const char* ssid = "AC-ESP8266";
const char* password = "TURKISTAN-13";

IPAddress local_IP(192,168,1,2);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
#endif

// UDP
WiFiUDP UDP;
char packet[255];
char reply[] = "Packet received!";

ComandInterpreter interpreter(ENGINE_PIN_A,ENGINE_PIN_B,STEERING_PIN_A,STEERING_PIN_B);

void setup() {
  pinMode(ENGINE_PIN_A,OUTPUT);
  pinMode(ENGINE_PIN_B,OUTPUT);

  pinMode(STEERING_PIN_A,OUTPUT);
  pinMode(STEERING_PIN_B,OUTPUT);

  Serial.begin(115200);
  Serial.println("Booting");
  
  #ifdef ACCES_POINT
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid,password) ? "Ready" : "Failed!");
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  WiFi.softAP(ssid);
  //WiFi.softAP(ssid, password, channel, hidden, max_connection)
  #endif

  #ifdef CONNECT_TO_WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  #endif // CONNECT_TO_WIFI if def
  otaSetup();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

    // Begin listening to UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Listening on UDP port ");
  Serial.println(UDP_PORT);

}

void loop() {
  ArduinoOTA.handle();

  // If packet received...
  int packetSize = UDP.parsePacket();
  if (packetSize) {
    Serial.print("Received packet! Size: ");
    Serial.println(packetSize); 
    int len = UDP.read(packet, 255);
    if (len > 0)
    {
      packet[len] = '\0';
    }
    Serial.print("Packet received: ");
    Serial.println(packet);

    // Send return packet
    UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
    UDP.write(reply);
    UDP.endPacket();

  }

}

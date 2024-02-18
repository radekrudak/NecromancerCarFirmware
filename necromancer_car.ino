#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "OTA_setup.h"
#include "comand_interpreter.h"

//#define CONNECT_TO_WIFI
#define ACCES_POINT
#define ENGINE_PIN_A D2
#define ENGINE_PIN_B D1

#define STEERING_PIN_A D6
#define STEERING_PIN_B D5

#define HEAD_LIGHT_PIN D0
#define BACK_LIGHT_PIN D7

#define UDP_PORT 6969

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

#define UDPInBufferSize 256
#define UDPOutBufferSize 256

// UDP
WiFiUDP UDP;
byte UDPInBuffer[UDPInBufferSize] = {0};
byte UDPOutBuffer[UDPOutBufferSize] = {0};

ComandInterpreter interpreter(ENGINE_PIN_A, ENGINE_PIN_B, 
                              STEERING_PIN_A, STEERING_PIN_B,
                              HEAD_LIGHT_PIN, BACK_LIGHT_PIN);

void setup() {
  pinMode(ENGINE_PIN_A,OUTPUT);
  pinMode(ENGINE_PIN_B,OUTPUT);

  pinMode(STEERING_PIN_A,OUTPUT);
  pinMode(STEERING_PIN_B,OUTPUT);

  pinMode(HEAD_LIGHT_PIN,OUTPUT);
  pinMode(BACK_LIGHT_PIN,OUTPUT);
  
  Serial.begin(115200);
  Serial.println("Booting");

  #ifdef ACCES_POINT
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid) ? "Ready" : "Failed!");
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  //WiFi.softAP(ssid);
  //WiFi.softAP(ssid, password, channel, hidden, max_connection)
  #endif

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
  memset(UDPInBuffer, 0, UDPInBufferSize);
  memset(UDPOutBuffer, 0, UDPOutBufferSize);
  interpreter.CheckComandTimeout();

  // If packet received...
  int packetSize = UDP.parsePacket();
  if (packetSize) {
    Serial.print("Received packet! Size: ");
    Serial.println(packetSize);

    int len = UDP.read(UDPInBuffer, UDPInBufferSize-1);

    interpreter.interpretComand(UDPInBuffer);
    Serial.print("Packet received: ");
    Serial.print(UDPInBuffer[0]);
    Serial.print(" ");
    Serial.print(UDPInBuffer[1]);
    Serial.print(" ");
    Serial.print(UDPInBuffer[2], BIN);
    Serial.print('\n');
    // Send return packet
    UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
    UDP.write(6); // return 6 - ACK askii code
    UDP.endPacket();

  }

}

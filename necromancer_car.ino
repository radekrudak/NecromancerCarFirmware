#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ArduinoWiFiServer.h>

#include "OTA_setup.h"
#include "comand_interpreter.h"

//#define CONNECT_TO_WIFI
#define ACCES_POINT


#define STASSID "RadoslawPhone"
#define STAPSK "Turkistan31"


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

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);
#endif

ArduinoWiFiServer server(6969);

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
  WiFi.softAP(ssid, password, channel, hidden, max_connection)
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

  server.begin();
}

void loop() {
  interpreter.CheckComandTimeout();
  ArduinoOTA.handle();
  WiFiClient client = server.available();     // returns first client which has data to read or a 'false' client
  if (client) {                               // client is true only if it is connected and has data to read
    String s = client.readStringUntil('\n');  // read the message incoming from one of the clients
    s.trim();                                 // trim eventual \r
    if(s[0] == 'r' | s[0]== 'R' )
    {
      interpreter.Reset();
      client.print("RESETING \r\n");
    }
    else 
    {
      interpreter.ExecuteComand(s,client);
    }
    Serial.println(s);                        // print the message to Serial Monitor
    //client.print(String(s));                   // this is only for the sending client
    server.flush();                           // flush the buffers
  }
}

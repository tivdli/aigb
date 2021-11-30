/*********
*********/

// Libraries
#include <WiFi.h>
#include "SPIFFS.h"

//Local libraries
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

//local files
#include <aigb.h>
#include <gbint.h>

//setup
#define PORT 80
#define SSID "VGV75195AFCBD"
#define PASS "f5EU7TA4pv3G"

//variables
AsyncWebServer server(PORT);
AsyncWebSocket websocket("/ws");
AsyncEventSource events("/events");
GBINT gbint(&server, &websocket, &events);
AIGB aigb;


void initWifi()
{
    WiFi.begin(SSID, PASS);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        i++;
        Serial.print(".");
        if (i == 20)
        {
            Serial.println("\nConnection failed, resetting AIGB");
            ESP.restart();
        }
        delay(500);
    }
}

void oE(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t * data, size_t len)
{
    gbint.onEvent(server, client, type, arg, data, len);
}

void setup()
{
    Serial.begin(115200);
    
    //start wifi and reset ESP on failure to connect within 10 seconds
    WiFi.begin(SSID, PASS);
    Serial.println("Connecting to wifi");
    initWifi();
    Serial.print("\nConnected to wifi with IP: ");
    Serial.println(WiFi.localIP());
    websocket.onEvent(GBINT::onEvent);
    server.addHandler(&websocket);


    
    gbint.init();
    aigb.init();
    //define baud rate for co2 sensor
    
  
}

void loop()
{
    
    aigb.LED();
    websocket.cleanupClients();
}
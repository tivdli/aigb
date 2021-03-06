/*********
*********/

// Libraries
#include <WiFi.h>
#include "SPIFFS.h"

//Local libraries
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
//#include <MHZ19.h>
#include <ESP32Servo.h>
#include <Adafruit_Sensor.h>
//#include <SPI.h> 
//local files
#include <aigb.h>
#include <gbint.h>
#include <data.h>
//setup
#define PORT 80
#define SSID "NETGEAR96"
#define PASS "5tsWWScWn"

//variables
AsyncWebServer server(PORT);
AsyncWebSocket websocket("/ws");
AsyncEventSource events("/events");

DATA aigb_data;

AIGB aigb(&aigb_data);





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
    GBINT::onEvent(server, client, type, arg, data, len);
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


    
    GBINT::init(&server, &websocket, &events, &aigb_data);
    // aigb.init();
    //define baud rate for co2 sensor
    
  
}

void loop()
{
    //first the variable settings have to be set (or we need a default setting)
    
    
    aigb.Servo_One();
    //temp_reading_inside,Hum_reading_inside =aigb.Measurment_In();
    websocket.cleanupClients();
    
    
}
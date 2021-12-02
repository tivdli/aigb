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
// AIGB aigb;

int temp_reading_inside = 0;
int temp_reading_outside = 0;
int temp_setting_inside_day = 0;
int temp_setting_inside_night = 0;
int Hum_reading_inside = 0;
int Hum_reading_outside = 0;
int Hum_setting_inside_day = 0;
int Hum_setting_inside_night = 0;
int co2_current_inside = 0;
int light_reading_1 = 0;
int light_reading_2 = 0;
int water_leavel_reading = 0;
int pump_power_setting = 0;

int light_power_setting = 0;
int light_color_setting = 0;

int feed_interval_setting = 0;
int feed_volume_setting = 0;




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
    // aigb.init();
    //define baud rate for co2 sensor
    
  
}

void loop()
{
    
    // aigb.LED();
    // temp_reading_inside,Hum_reading_inside =aigb.Measurment_In();
    websocket.cleanupClients();
}
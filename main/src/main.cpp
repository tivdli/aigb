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


void setup(){
    Serial.begin(115200);
    //start wifi and reset ESP on failure to connect within 10 seconds
    WiFi.begin(SSID, PASS);
    Serial.println("Connectin to wifi");
    int i =0;
    while (WiFi.status() != WL_CONNECTED) {
        i++;
        Serial.print(".");
        if (i == 20)
        {
            Serial.println("\nConnection failed, resetting AIGB");
            ESP.restart();    
        }
        delay(500);
    }
    Serial.print("\nConnected to wifi with IP: ");
    Serial.println(WiFi.localIP());
    //init file loader for website
    if (!SPIFFS.begin(true)){
        Serial.println("Error while mounting SPIFFS, restarting ESP");
        ESP.restart();
    }
    //start server
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html");
    });
    
    server.serveStatic("/", SPIFFS, "/");
    server.begin();

}

void loop(){

}
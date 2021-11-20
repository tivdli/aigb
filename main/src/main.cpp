/*********
*********/

// Libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

//local files
#include <aigb.h>
#include <gbint.h>
#include <site.txt>

//setup
#define PORT 80
#define SSID "VGV75195AFCBD"
#define PASS "f5EU7TA4pv3G"

//variables
AsyncWebServer server(PORT);
AsyncWebSocket websocket('/ws');


void setup(){
    Serial.begin(115200);
    //start wifi and reset ESP on failure to connect within 10 seconds
    wifi = WiFi.begin(SSID, PASS);
    for (int i = 0; (WiFi.status() != WL_CONNECTED); i++) {
        if (i == 20)
        {
            Serial.println("Connection failed, resetting AIGB");
            ESP.restart();    
        }
        delay(500);
    }
    //start websocket

}

void webLoop(void * empty){

}

void loop(){

}
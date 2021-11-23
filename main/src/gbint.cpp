#include <Arduino.h>
#include <gbint.h>
#include <ASyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <EEPROM.h>
#include <SPIFFS.h>
GBINT::GBINT(AsyncWebServer server, AsyncWebSocket ws, AsyncEventSource events)
{
    GBINT::ws = ws;
    GBINT::server = server;
    GBINT::events = events;

    EEPROM.begin(GBINT::erSize);
    if (EEPROM.read(0x000) != GBINT::erVersion)
    {
        GBINT::resetmemory();
        GBINT::profileNum = 0;
    }
    else
    {
        GBINT::profileNum = EEPROM.read(GBINT::profileStart - 1);
    }
    if (!SPIFFS.begin(true))
    {
        Serial.println("Error while mounting SPIFFS, restarting ESP");
        ESP.restart();
    }
    //start server
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.html", "text/html"); });

    server.serveStatic("/", SPIFFS, "/");

    server.begin();
}

void GBINT::resetmemory()
{
    for (i = 1; i < GBINT::erSize; i++)
    {
        EEPROM.write(i,0);
    }
    EEPROM.write(0x000, GBINT::erVersion);
    EEPROM.commit();
}

JSONVar GBINT::listprofiles()
{
    if (GBINT::profileNum)
    {
        JSONVar returnArray;
        i = 0;
        for (a = GBINT::profileStart; a <= (GBINT::profileStart + (GBINT::profileLength * GBINT::profileNum)); a += GBINT::profileLength)
        {
            returnArray[i] = EEPROM.read(a);
            i++;
        }
        return returnArray;
    }
    return 0;
}

bool GBINT::setprofile()
{
}
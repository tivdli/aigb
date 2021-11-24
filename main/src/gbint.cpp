#include <Arduino.h>
#include <gbint.h>
#include <ASyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <EEPROM.h>
#include <SPIFFS.h>
GBINT::GBINT(AsyncWebServer *server, AsyncWebSocket *ws, AsyncEventSource *events)
{
    GBINT::ws = ws;
    GBINT::server = server;
    GBINT::events = events;
}

bool GBINT::init()
{
    EEPROM.begin(EEPROMSIZE);
    if (EEPROM.read(0x000) != PROFILEVERSION)
    {
        GBINT::resetmemory();
        GBINT::profileNum = 0;
    }
    else
    {
        GBINT::profileNum = EEPROM.read(PROFILESTART - 1);
    }
    if (!SPIFFS.begin(true))
    {
        Serial.println("Error while mounting SPIFFS, restarting ESP");
        ESP.restart();
    }
    //start server
    GBINT::server->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                      { request->send(SPIFFS, "/index.html", "text/html"); });

    GBINT::server->serveStatic("/", SPIFFS, "/");

    GBINT::server->begin();
}

void GBINT::resetmemory()
{
    for (int i = 1; i < EEPROMSIZE; i++)
    {
        EEPROM.write(i, 0);
    }
    EEPROM.write(0x000, PROFILEVERSION);
    EEPROM.commit();
    delay(500);
}

JSONVar GBINT::listprofiles()
{
    if (GBINT::profileNum)
    {
        JSONVar returnArray;
        int i = 0;
        for (int a = PROFILESTART; a <= (PROFILESTART + (PROFILELENGTH * GBINT::profileNum)); a += PROFILELENGTH)
        {
            returnArray[i] = EEPROM.read(a);
            i++;
        }
        return returnArray;
    }
    return 0;
}

void handleWSMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if(info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        
    }
}

bool GBINT::setprofile(JSONVar msg)
{
    return 1;
}

JSONVar GBINT::getprofile(int number)
{
    JSONVar returnArray;
    int address = PROFILESTART + number * PROFILELENGTH;
    int val;
    for (int a = 0; a < PROFILELENGTH; a++)
    {
        address = address + GBINT::profileMakeup[a];
        val = 0;
        for (int c = 0; c < GBINT::profileMakeup[a]; c++)
        {
            if (c == 0)
            {
                val = EEPROM.read(address);
            }
            else
            {
                val = (val << 8) + EEPROM.read(address);
            }
        }
        returnArray[a] = val;
    }

    return returnArray;
}
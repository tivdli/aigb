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

void GBINT::init()
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
                      { request->send(SPIFFS, "/index.html", "text/html", false); });

    GBINT::server->serveStatic("/", SPIFFS, "/");

    GBINT::server->begin();
}

void GBINT::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
    {
        Serial.printf("New connection: client #%u at %s\n", client->id(), client->remoteIP().toString().c_str());
        client->ping();
    }
    break;
    case WS_EVT_DISCONNECT:
    {
        Serial.printf("Disconnected: client #%u \n", client->id());
    }
    break;
    case WS_EVT_DATA:
    {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
        {
            Serial.println("test");
            GBINT::obj = JSON.parse((char *)data);
            if (strcmp(GBINT::obj["n"], "btn"))
            {
                Serial.printf("\nChanged button %s to %u", obj["i"], obj["d"]);
            }
            else if (strcmp(GBINT::obj["n"], "sld"))
            {
                Serial.printf("\nChanged slider  %s to %u", obj["i"], obj["d"]);
            }
            else if (strcmp(GBINT::obj["n"], "pfs"))
            {
                Serial.printf("\nSelected %s to %u", obj["i"], obj["d"]);
            }
            else if (strcmp(GBINT::obj["n"], "pfw"))
            {
                Serial.printf("\nWrote profile %s to %u", obj["i"], obj["d"]);
            }
            else if (strcmp(GBINT::obj["n"], "inp"))
            {
                Serial.printf("\nInput %s to %u", obj["i"], obj["d"]);
            }
            // else if (strcmp(obj["n"], ""))
            // {
            //     /* code */
            // }
                }
    }
    break;
    case WS_EVT_PONG:
    {
        Serial.printf("Pong from client #%u \n", client->id());
    }
    break;
    case WS_EVT_ERROR:
    {
        Serial.println("WS event error");
    }
    break;
    }
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
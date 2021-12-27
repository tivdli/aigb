#include <Arduino.h>
#include <gbint.h>
#include <ASyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <EEPROM.h>
#include <SPIFFS.h>
// GBINT::GBINT()
// {
// }
AsyncWebSocket * GBINT::ws = 0;
AsyncWebServer * GBINT::server=0;
AsyncEventSource * GBINT::events=0;
int GBINT::profileNum=0;
void GBINT::init(AsyncWebServer *server, AsyncWebSocket *ws, AsyncEventSource *events)
{
    GBINT::ws = ws;
    GBINT::server = server;
    GBINT::events = events;
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
            JSONVar obj = JSON.parse((char *)data);
            String name((const char*)obj["n"]);
            Serial.println(name);
            if (name == "btn")
            {
                Serial.printf("\nChanged button %s to %u", (const char*)obj["i"], (int)obj["d"]);
                int btn_val = (int) obj["d"];
                if (String((const char*) obj["i"]) == "b5")
                {
                    //"{\"result\":true,\"count\":42,\"foo\":\"bar\"}";
                    Serial.println("1");
                    JSONVar msg;
                    msg["key"] = "pfw";
                    msg["go"] = 1;
                    for (int i = 0; i < EEPROM.read(PROFILESTART-2); i++)
                    {
                        if (EEPROM.read(PROFILESTART + PROFILELENGTH*i) == btn_val){
                            msg["go"] = 0;
                            break;
                        }
                    }
                    String send = JSON.stringify(msg);
                    Serial.println("2");
                    GBINT::ws->text(client->id(), send);
                    Serial.println("3");
                }
            }
            else if (name =="sld")
            {
                Serial.printf("\nChanged slider  %s to %u", obj["i"], obj["d"]);
            }
            else if (name == "pfs")
            {
                Serial.printf("\nSelected %s to %u", obj["i"], obj["d"]);
            }
            else if (name == "pfw")
            {
                GBINT::setprofile(obj);
            }
            else if (name == "prr")
            {
                Serial.printf("\nInput %s to %u", obj["i"], obj["d"]);
            }
            //else if (name == "")
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

static void respond(String jstring)
{
    GBINT::ws->text(1,"NULL");
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

void GBINT::setprofile(JSONVar msg)
{
    int profCount = EEPROM.read(PROFILESTART-2)*PROFILELENGTH; 
    int writeStart = PROFILESTART + profCount;
    EEPROM.write(PROFILESTART-2, profCount++);
    for (int i = 0; i < PROFILEENTRIES; i++)
    {

    }
}

JSONVar GBINT::getprofile(int number)
{
    JSONVar returnArray;
    int address = PROFILESTART + number * PROFILELENGTH;
    int val;
    for (int a = 0; a < PROFILELENGTH; a++)
    {
        address = address + profileMakeup[a];
        val = 0;
        for (int c = 0; c < profileMakeup[a]; c++)
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

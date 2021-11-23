#ifndef gbint_h
#define gbint_h

#include <Arduino.h>
#include <ASyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <EEPROM.h>
class GBINT
{
public:
    GBINT(AsyncWebServer server, AsyncWebSocket ws, AsyncEventSource events);
    JSONVar respond();
    JSONVar getprofile(int number);
    JSONVar listprofiles();
    bool setprofile();

private:
    AsyncWebSocket ws;
    AsyncWebServer server;
    AsyncEventSource events;

    //Profile settings
    const int erSize = 512;
    const int erVersion = 178;
    const int profileStart = 0x097;
    const int profileLength = 0x00C;
    const int profileIndexName = 0;
    const int profileIndexLightColor = 1;
    const int profileIndexLightTime = 4;
    const int profileIndexFeedVol = 8;
    const int profileIndexFeedInter = 9; 
    const int profileLenName = 1;
    const int profileLenLightColor = 3;
    const int profileLenLightTime = 4;
    const int profileLenFeedVol = 1;
    const int profileLenFeedInter = 1;
    //profile vars
    int profileNum;
    void resetmemory();
    String respond();
};
#endif
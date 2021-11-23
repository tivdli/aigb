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
    GBINT(AsyncWebServer *server, AsyncWebSocket *ws, AsyncEventSource *events);
    JSONVar getprofile(int number);
    JSONVar listprofiles();
    bool setprofile();
    bool init();

private:
    AsyncWebSocket *ws;
    AsyncWebServer *server;
    AsyncEventSource *events;

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
    //profile makeup = 10B = 1=name 2=rgb 5=h_start 6=m_start 7=h_end 8=m_end 9=feed_volume 10=feed_interval
    //profile vars
    int profileNum;
    void resetmemory();
    void respond();
};
#endif
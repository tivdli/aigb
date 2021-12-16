#ifndef gbint_h
#define gbint_h

#include <Arduino.h>
#include <ASyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <EEPROM.h>

#define EEPROMSIZE 512
#define PROFILEVERSION 178
#define PROFILESTART 0x03F
#define PROFILELENGTH 0x010
#define PROFILESIZEOFNAME 1
#define PROFILESIZEOFLIGHTCOLOR 3
#define PROFILESIZEOFLIGHTTIME 4
#define PROFILESIZEOFFEEDVOL 1
#define PROFILESIZEOFFEEDINTER 1
#define PROFILESIZEOFTEMPDAY 1
#define PROFILESIZEOFTEMPNIGHT 1
#define PROFILESIZEOFHUMDAY 1
#define PROFILESIZEOFHUMNIGHT 1
//profile makeup = 14B = 1=name 2=rgb 5=h_start 6=m_start 7=h_end 8=m_end 9=feed_volume 10=feed_interval 11=tmp_day 12=tmp_night 13=hum_day 14=hum_night

class GBINT
{
public:
    //GBINT();
    JSONVar getprofile(int number);
    JSONVar listprofiles();
    bool setprofile(JSONVar msg);
    static void init(AsyncWebServer *server, AsyncWebSocket *ws, AsyncEventSource *events);
    static void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    static AsyncWebSocket *ws;
    static AsyncWebServer *server;
    static AsyncEventSource *events;

    //Profile settings

    const int profileMakeup[9] = {PROFILESIZEOFNAME, PROFILESIZEOFLIGHTCOLOR, PROFILESIZEOFLIGHTTIME, PROFILESIZEOFFEEDVOL, PROFILESIZEOFFEEDINTER, PROFILESIZEOFTEMPDAY, PROFILESIZEOFTEMPNIGHT, PROFILESIZEOFHUMDAY, PROFILESIZEOFHUMNIGHT};

    //profile vars
    static int profileNum;
    static void resetmemory();
};
#endif
#ifndef gbint_h
#define gbint_h

#include <Arduino.h>
#include <ASyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <EEPROM.h>
#include <data.h>
#define EEPROMSIZE 512
#define PROFILEVERSION 1
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
const int PROFILEENTRIES = PROFILESIZEOFNAME + PROFILESIZEOFLIGHTCOLOR + PROFILESIZEOFLIGHTTIME + PROFILESIZEOFFEEDVOL + PROFILESIZEOFFEEDINTER + PROFILESIZEOFTEMPDAY + PROFILESIZEOFTEMPNIGHT + PROFILESIZEOFHUMDAY + PROFILESIZEOFHUMNIGHT;
const int PROFILEAMOUNT = PROFILESTART - 2;
//profile makeup = 14B = 1=name 2=rgb 5=h_start 6=m_start 7=h_end 8=m_end 9=feed_volume 10=feed_interval 11=tmp_day 12=tmp_night 13=hum_day 14=hum_night

const int profileMakeup[9] = {PROFILESIZEOFNAME, PROFILESIZEOFLIGHTCOLOR, PROFILESIZEOFLIGHTTIME, PROFILESIZEOFFEEDVOL, PROFILESIZEOFFEEDINTER, PROFILESIZEOFTEMPDAY, PROFILESIZEOFTEMPNIGHT, PROFILESIZEOFHUMDAY, PROFILESIZEOFHUMNIGHT};
class GBINT
{
public:
    static void getprofile(int number, int *data_var[PROFILEENTRIES]);
    static JSONVar listprofiles();

    static void init(AsyncWebServer *server, AsyncWebSocket *ws, AsyncEventSource *events, DATA *aigb_data);
    static void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    //profile vars
    static AsyncWebSocket *ws;
    static AsyncWebServer *server;
    static AsyncEventSource *events;
    //Profile settings
private:
    GBINT();
    static void setprofile(JSONVar msg);
    static void resetmemory();
    static int profileNum;
    static int *temp_reading_inside;
    static int *temp_reading_outside;
    static int *temp_setting_inside_day;
    static int *temp_setting_inside_night;
    static int *Hum_reading_inside;
    static int *Hum_reading_outside;
    static int *Hum_setting_inside_day;
    static int *Hum_setting_inside_night;
    static int *co2_current_inside;
    static int *light_reading_1;
    static int *light_reading_2;
    static int *water_level_reading;
    static int *pump_power_setting;

    static int *light_power_setting;
    static int *light_color_setting;

    static int *feed_interval_setting;
    static int *feed_volume_setting;
    static bool *pump_stat;
    static bool *light_stat;
    static bool *feed_stat;
    static bool *air_stat;
    static DATA *aigb_data;

};

void alloc_array(int size, int **result);
#endif
#ifndef gbint_h
#define gbint_h

#include <Arduino.h>
#include <ASyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <EEPROM.h>
#include <data.h>
#define EEPROMSIZE 512
#define PROFILEVERSION 4
#define PROFILESTART 0x03F
#define PROFILELENGTH 0x010

const int PROFILEENTRIES = 15;
const int PROFILEAMOUNT = PROFILESTART - 2; //location of profile_counter
//profile makeup = 14B = 1=name 2=rgb 5=h_start 6=m_start 7=h_end 8=m_end 9=pumppower 10=feed_volume 11=feed_interval 12=tmp_day 13=tmp_night 14=hum_day 15=hutn_m

class GBINT
{
public:
    static void getprofile(int number, int *data_var[PROFILEENTRIES]);
    static String listprofiles();
    static void update();
    static void init(AsyncWebServer *server, AsyncWebSocket *ws, AsyncEventSource *events, DATA *aigb_data);
    static void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    //profile vars
    static AsyncWebSocket *ws;
    static AsyncWebServer *server;
    static AsyncEventSource *events;
    //Profile settings
protected:
    GBINT();
    static void set_profile(JSONVar msg);
    static void resetmemory();
    static int profileNum;
    static DATA *aigb_data;
    static void set_subsystem(String btn, int set_state);
    static int check_profile(int name_number);
    static void fill_page(int id);
    static void handle_direct_input(int id, String input_id, int value);
    static String get_profile(int number);
    static void activate_profile(int number);
};

void alloc_array(int size, int **result);
#endif
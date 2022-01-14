#include <Arduino.h>
#include <gbint.h>
#include <ASyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <EEPROM.h>
#include <SPIFFS.h>
#include <data.h>

//definition of static pointers to allocate memory space
AsyncWebSocket *GBINT::ws = 0;
AsyncWebServer *GBINT::server = 0;
AsyncEventSource *GBINT::events = 0;
DATA *GBINT::aigb_data = 0;
int GBINT::profileNum = 0;

//initializes webserver and checks memory version
void GBINT::init(AsyncWebServer *server, AsyncWebSocket *ws, AsyncEventSource *events, DATA *aigb_data)
{
    //point class variables to main vars
    GBINT::ws = ws;
    GBINT::server = server;
    GBINT::events = events;

    GBINT::aigb_data = aigb_data;
    EEPROM.begin(EEPROMSIZE);

    //Check if EEPROM version matches class version
    if (EEPROM.read(0x000) != PROFILEVERSION)
    {
        Serial.println("Resetting memory");
        GBINT::resetmemory();
        GBINT::profileNum = 0;
    }
    //If matches, set profileNum to amount of profiles in EEPROM.
    else
    {
        GBINT::profileNum = EEPROM.read(PROFILEAMOUNT);
        Serial.print("Starting with ");
        Serial.print(GBINT::profileNum);
        Serial.println(" stored profiles");
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

//The messy heap that is the main attraction of this code. All server events get routed from here.
void GBINT::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
    {
        Serial.printf("New connection: client #%u at %s\n", client->id(), client->remoteIP().toString().c_str());
        //client->ping(); Ping causes the websocket to not send messages, dont use.
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
            JSONVar obj = JSON.parse((char *)data);
            String name((const char *)obj["n"]);
            //Handle incoming button presses
            if (name == "btn")
            {
                String switch_id = (const char * ) obj["i"];
                //check if not profile save button, and (de)activate subsystem accordingly
                if (switch_id != "b5" && switch_id != "b6")
                {
                    GBINT::set_subsystem(switch_id, (bool)obj['d']);
                }
                else if (switch_id == "b5")
                {
                    JSONVar msg;
                    msg["key"] = "pfw";
                    msg["go"] = GBINT::check_profile((int)obj["d"]);
                    String tosend = JSON.stringify(msg);
                    client->text(tosend);
                } 
                else if (switch_id == "b6")
                {
                    GBINT::activate_profile((int)obj["d"]);
                    GBINT::fill_page(client->id());
                }
            }
            
            //handle client startup and page fill
            else if (name=="stt")
            {
                GBINT::fill_page(client->id());
            }
            //handle input from direct control
            else if (name == "inp")
            {
                String input_id = (const char *) obj["i"];
                int value = (int) obj["d"];

                GBINT::handle_direct_input(input_id, value);
            }
            //select profile and set data variables accordingly. Then, send values to 
            else if (name == "pfs")
            {
                int pf_num = (int) obj["d"];
                Serial.println(pf_num);
                client->text(GBINT::get_profile(pf_num));
            }
            //Create or update profile
            else if (name == "pfw")
            {
                GBINT::set_profile(obj);
            }
            //Activate selected profile
            //return list of profiles to client
            else if (name == "prr")
            {
                Serial.println("Client request: stored profiles");
                String send = GBINT::listprofiles();
                Serial.println(send);
                client->text(send);
            }
            else if (name == "utv")
            {
                Serial.print("system time update to: ");
                Serial.print((int) obj["d"][2]);Serial.print("-");
                Serial.print((int) obj["d"][1]);Serial.print("-");
                Serial.print((int) obj["d"][0]);Serial.print("  ");
                Serial.print((int) obj["d"][3]);Serial.print(":");
                Serial.print((int) obj["d"][4]);Serial.print(":");
                Serial.println((int) obj["d"][5]);

                Serial.println(GBINT::aigb_data->pump_power_setting);
            }
        }         
    }
    break;
    case WS_EVT_PONG:
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

String GBINT::listprofiles()
{
    JSONVar l_data;
    if (GBINT::profileNum > 0)
    {
        int i = 0;
        l_data["key"] = "pfo";
        String s;
        for (int a = PROFILESTART; a < (PROFILESTART + (PROFILELENGTH * GBINT::profileNum)); a += PROFILELENGTH)
        {
            s = i;
            l_data[s] = EEPROM.read(a);
            i++;
        }
        return JSON.stringify(l_data);
    }
    return "0";
}

void GBINT::set_profile(JSONVar msg)
{
    int profCount = GBINT::profileNum * PROFILELENGTH;
    int writeStart = PROFILESTART + profCount;
    GBINT::profileNum++;
    EEPROM.write(PROFILESTART - 2, GBINT::profileNum);
    for (int i = 0; i < PROFILEENTRIES; i++)
    {
        EEPROM.write(writeStart + i, (int)msg["i"][i]);
        Serial.println( (int)msg["i"][i]);
    }
    Serial.print("Printed thing: ");
    Serial.println(EEPROM.commit());
    delay(500);
}

String GBINT::get_profile(int number)
{
    JSONVar data;
    //KLOPT NIET, ALTIJD PROFIEL 0
    int address = GBINT::check_profile(number);
    data["key"] = "pfr";
    String s;
    for (int a = 0; a < PROFILELENGTH; a++)
    {
        s = a;
        data[s] = EEPROM.read(address+a);
    }
    return JSON.stringify(data);
}

void alloc_array(int size, int **result) { 
  *result = (int *)::operator new(size); 
} 

//Check if profile exists, returns 0 when false and EEPROM index if true
int GBINT::check_profile(int name_number)
{
    Serial.print(name_number);
    Serial.println("  - check if profile exists");
    for (int a = PROFILESTART; a < EEPROMSIZE; a+=PROFILELENGTH)
    {
        Serial.print(a);
        Serial.print(" -> ");
        Serial.println(EEPROM.read(a));
        if (EEPROM.read(a) == name_number)
        {
            return a;
        }
    }
    return -1;
}

//Get current AIGB data from DATA block and send to requesting client
void GBINT::fill_page(int id){
    JSONVar msg;
    msg["key"]="stt";
    msg["bt0"] = GBINT::aigb_data->pump_stat;
    msg["bt1"] = GBINT::aigb_data->light_stat;
    msg["bt2"] = GBINT::aigb_data->feed_stat;
    msg["bt3"] = GBINT::aigb_data->air_stat;
    
    msg["sl0"] = GBINT::aigb_data->pump_power_setting;
    msg["sl1"] = GBINT::aigb_data->light_power_setting;
    msg["sl2"] = GBINT::aigb_data->air_power_setting;

    msg["ll0"] = GBINT::aigb_data->light_reading_1;
    msg["ll1"] = GBINT::aigb_data->light_reading_2;

    msg["clr"] = ((GBINT::aigb_data->light_color_setting_r<<16) + (GBINT::aigb_data->light_color_setting_g<<8) + GBINT::aigb_data->light_color_setting_b);

    msg["fv0"] = GBINT::aigb_data->feed_volume_setting;
    msg["fv1"] = GBINT::aigb_data->feed_interval_setting;

    msg["as0"] = GBINT::aigb_data->co2_current_inside;
    msg["as1"] = GBINT::aigb_data->temp_reading_inside;
    msg["as2"] = GBINT::aigb_data->temp_reading_outside;
    msg["as3"] = GBINT::aigb_data->Hum_reading_inside;
    msg["as4"] = GBINT::aigb_data->Hum_reading_outside;

    msg["ai0"] = GBINT::aigb_data->temp_setting_current;
    msg["ai1"] = GBINT::aigb_data->hum_setting_current;
    
    msg["prf"] = GBINT::profileNum;

    msg["pfa"] = GBINT::aigb_data->Profile_user;


    GBINT::ws->text(id, JSON.stringify(msg));
}

void GBINT::handle_direct_input(String input_id, int value)
{
    if (GBINT::aigb_data->Profile_user >= 0){
        GBINT::aigb_data->Profile_user = -1;
    }
    if (input_id == "s1"){
        Serial.print("set pump_power_settting: ");
        Serial.println(value);
        GBINT::aigb_data->pump_power_setting = value;
    }
    else if (input_id == "s2"){
        GBINT::aigb_data->light_power_setting = value;
    }
    else if (input_id == "s3"){
        GBINT::aigb_data->air_power_setting = value;
    }
    else if (input_id == "c1"){
        GBINT::aigb_data->light_color_setting_r = (value & 0xFF0000)>>16;
        GBINT::aigb_data->light_color_setting_g = (value & 0x00FF00)>>8;
        GBINT::aigb_data->light_color_setting_b = (value & 0x0000FF);

    }
    else if (input_id == "f1"){
        GBINT::aigb_data->feed_interval_setting = value;
    }
    else if (input_id == "f2"){
        GBINT::aigb_data->feed_volume_setting = value;
    }
    else if (input_id == "a1"){
        GBINT::aigb_data->temp_setting_current = value;
    }
    else if (input_id == "a2"){
        GBINT::aigb_data->hum_setting_current = value;
    }
}

//push current profile to AIGB 
void GBINT::activate_profile(int number)
{
    int address = GBINT::check_profile(number);
    int val = 0;
    Serial.print("Profile start at");
    Serial.println(address);
    if (address != 0)
    {
        GBINT::aigb_data->Profile_user = number;
        Serial.print("Set PROFILE active to: ");
        Serial.println(GBINT::aigb_data->Profile_user);
        //start at i=1 because we don't use the first byte (name)
        for (int i = 1; i < PROFILEENTRIES; i++)
        {
            val = EEPROM.read(address +i);
            Serial.println(val);
            switch (i-1){
                case 0:
                GBINT::aigb_data->light_color_setting_r = val;
                break;
                case 1:
                GBINT::aigb_data->light_color_setting_g = val;
                break;
                case 2:
                GBINT::aigb_data->light_color_setting_b = val;
                break;
                case 3:
                GBINT::aigb_data->h_morning = val;
                break;
                case 4:
                GBINT::aigb_data->m_morning = val;
                break;
                case 5:
                GBINT::aigb_data->h_night = val;
                break;
                case 6:
                GBINT::aigb_data->m_night = val;
                break;
                case 7:
                GBINT::aigb_data->pump_power_setting = val;
                break;
                case 8:
                GBINT::aigb_data->feed_volume_setting = val;
                break;
                case 9:
                GBINT::aigb_data->feed_interval_setting = val;
                break;
                case 10:
                GBINT::aigb_data->temp_setting_inside_day = val;
                break;
                case 11:
                GBINT::aigb_data->temp_setting_inside_night = val;
                break;
                case 12:
                GBINT::aigb_data->Hum_setting_inside_day = val;
                break;
                case 13:
                GBINT::aigb_data->Hum_setting_inside_night = val;
                break;
                default:
                Serial.println("something went wrong while writing in activate_profile, unknown index");
            }

        }

    }
    else{
        Serial.println("something went wrong while writing in activate_profile, unknown profile");
    }
    Serial.println("Profile activated");
}

void GBINT::set_subsystem(String btn, bool set_state)
{
    if (GBINT::aigb_data->Profile_user >= 0){
        GBINT::aigb_data->Profile_user = -1;
    }
    bool * ptr;
    if (btn == "b1")
    {
        ptr = &GBINT::aigb_data->pump_stat;
    }
    else if ( btn == "b2")
    {
        ptr = &GBINT::aigb_data->light_stat;
    }
    else if ( btn == "b3")
    {
        ptr = &GBINT::aigb_data->feed_stat;
    }
    else if ( btn == "b4")
    {
        ptr = &GBINT::aigb_data->air_stat;
    }
    else {
        Serial.println("Unknown index in set_subsystem");
        return;
    }

    if (*ptr != set_state)
    {
        *ptr = set_state;
    }
}
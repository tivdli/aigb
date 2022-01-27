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
            Serial.println(obj);
            String name((const char *)obj["n"]);
            //Handle incoming button presses
            if (name == "btn")
            {
                String switch_id = (const char * ) obj["i"];
                //check if not profile save button, and (de)activate subsystem accordingly
                if (switch_id != "b5" && switch_id != "b6")
                {
                    GBINT::set_subsystem(switch_id, (int)obj['d']);
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
                GBINT::handle_direct_input(client->id(), input_id, value);
            }
            //select profile and set data variables accordingly. Then, send values to 
            else if (name == "pfs")
            {
                client->text(GBINT::get_profile((int) obj["d"]));
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
            }
        }         
    }
    break;
    case WS_EVT_ERROR:
    {
        Serial.println("WS event error");
    }
    break;
    }
}

void GBINT::update()
{
    GBINT::fill_page(-1);
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
    int address = PROFILESTART + number * PROFILELENGTH;
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
    for (int a = PROFILESTART; a < EEPROMSIZE; a+=PROFILELENGTH)
    {
        if (EEPROM.read(a) == name_number)
        {
            return a;
        }
    }
    return 0;
}

//Get current AIGB data from DATA block and send to requesting client
void GBINT::fill_page(int id){
    JSONVar msg;
    msg["key"]="stt";
    msg["bt0"] = GBINT::aigb_data->get("ps_o");
    msg["bt1"] = GBINT::aigb_data->get("ls_o");
    msg["bt2"] = GBINT::aigb_data->get("fs_o");
    msg["bt3"] = GBINT::aigb_data->get("as_o");
    
    msg["sl0"] = GBINT::aigb_data->get("ps_p");
    msg["sl1"] = GBINT::aigb_data->get("ls_p");
    msg["sl2"] = GBINT::aigb_data->get("as_p");

    msg["ll0"] = GBINT::aigb_data->get("lr_1");
    msg["ll1"] = GBINT::aigb_data->get("lr_2");

    msg["clr"] = ((GBINT::aigb_data->get("ls_r")<<16) + (GBINT::aigb_data->get("ls_g")<<8) + GBINT::aigb_data->get("ls_b"));

    msg["fv0"] = GBINT::aigb_data->get("fs_v");
    msg["fv1"] = GBINT::aigb_data->get("fs_i");

    msg["as0"] = GBINT::aigb_data->get("cr_1");
    msg["as1"] = GBINT::aigb_data->get("tr_2");
    msg["as2"] = GBINT::aigb_data->get("tr_1");
    msg["as3"] = GBINT::aigb_data->get("hr_1");
    msg["as4"] = GBINT::aigb_data->get("hr_2");

    msg["ai0"] = GBINT::aigb_data->get("ts_c");
    msg["ai1"] = GBINT::aigb_data->get("hs_c");
    
    msg["prf"] = GBINT::profileNum;

    if (id != -1)
    {GBINT::ws->text(id, JSON.stringify(msg));}
    else{
        GBINT::ws->textAll(JSON.stringify(msg));
    }
}

void GBINT::handle_direct_input(int id, String input_id, int value)
{
    if (input_id == "s1"){
        GBINT::aigb_data->set("ps_p",value);
    }
    else if (input_id == "s2"){
        GBINT::aigb_data->set("ls_p",value);
    }
    else if (input_id == "s3"){
        GBINT::aigb_data->set("as_p",value);
    }
    else if (input_id == "c1"){
        GBINT::aigb_data->set("ls_r",((value & 0xFF0000)>>16));
        GBINT::aigb_data->set("ls_g",((value & 0x00FF00)>>8));
        GBINT::aigb_data->set("ls_b",(value & 0x0000FF));

    }
    else if (input_id == "f1"){
        GBINT::aigb_data->set("fs_i",value);
    }
    else if (input_id == "f2"){
        GBINT::aigb_data->set("fs_v",value);
    }
    else if (input_id == "a1"){
        GBINT::aigb_data->set("ts_c",value);
    }
    else if (input_id == "a2"){
        GBINT::aigb_data->set("hs_c",value);
    }
}

//push current profile to AIGB 
void GBINT::activate_profile(int number)
{
    int address = GBINT::check_profile(number);
    int val = 0;
    if (address != 0)
    {
        //start at i=1 because we don't use the first byte (name)
        for (int i = 1; i < PROFILEENTRIES; i++)
        {
            val = EEPROM.read(address +i);
            switch (i-1){
                case 0:
                GBINT::aigb_data->set("ls_r",val);
                break;
                case 1:
                GBINT::aigb_data->set("ls_g",val);
                break;
                case 2:
                GBINT::aigb_data->set("ls_b",val);
                break;
                case 3:
                GBINT::aigb_data->set("tm_h",val);
                break;
                case 4:
                GBINT::aigb_data->set("tm_m",val);
                break;
                case 5:
                GBINT::aigb_data->set("tn_h",val);
                break;
                case 6:
                GBINT::aigb_data->set("tn_m",val);
                break;
                case 7:
                GBINT::aigb_data->set("ps_p",val);
                break;
                case 8:
                GBINT::aigb_data->set("fs_v",val);
                break;
                case 9:
                GBINT::aigb_data->set("fs_i",val);
                break;
                case 10:
                GBINT::aigb_data->set("ts_d",val);
                break;
                case 11:
                GBINT::aigb_data->set("ts_n",val);
                break;
                case 12:
                GBINT::aigb_data->set("hs_d",val);
                break;
                case 13:
                GBINT::aigb_data->set("hs_n",val);
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

void GBINT::set_subsystem(String btn, int set_state)
{
    if (btn == "b1")
    {
        //GBINT::aigb_data->set("ps_o", 0 ? GBINT::aigb_data->get("ps_o") == 1 : 1);
    }
    else if ( btn == "b2")
    {
        //GBINT::aigb_data->set("ls_o", 0 ? GBINT::aigb_data->get("ls_o") == 1 : 1);
    }
    else if ( btn == "b3")
    {
        //GBINT::aigb_data->set("fs_o", 0 ? GBINT::aigb_data->get("fs_o") == 1 : 1);
    }
    else if ( btn == "b4")
    {
        //GBINT::aigb_data->set("as_o", 0 ? GBINT::aigb_data->get("as_o") == 1 : 1);
    }
    else {
        Serial.println("Unknown index in set_subsystem");
        return;
    }
}
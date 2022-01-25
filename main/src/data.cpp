#include <Arduino.h>
#include <gbint.h>
#include <ASyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <EEPROM.h>
#include <SPIFFS.h>
#include <data.h>

DATA::DATA()
{
}

void DATA::set(String name, int val)
{
    bool changed= false;
    int * ptr = pointer(name);
    changed= true ? *ptr != val : false;
    *ptr = val;
    if (changed == true){
        GBINT::update();
    }
}

int DATA::get(String name)
{
    int * value = pointer(name);
    return *value;
}

int * DATA::pointer(String name)
{
    int * val = 0;
    if (name == (String) "tr_1")
    {
        val = &tr_1;
    }
    else if (name == "tr_2")
    {
        val = &tr_2;
    }
    else if (name == "ts_d")
    {
        val = &ts_d;
    }
    else if (name == "ts_n")
    {
        val = &ts_n;
    }
    else if (name == "ts_c")
    {
        val = &ts_c;
    }
    else if (name == "hs_c")
    {
        val = &hs_c;
    }
    else if (name == "hr_1")
    {
        val = &hr_1;
    }
    else if (name == "hr_2")
    {
        val = &hr_2;
    }
    else if (name == "hs_d")
    {
        val = &hs_d;
    }
    else if (name == "hs_n")
    {
        val = &hs_n;
    }
    else if (name == "cr_1")
    {
        val = &cr_1;
    }
    else if (name == "lr_1")
    {
        val = &lr_1;
    }
    else if (name == "lr_2")
    {
        val = &lr_2;
    }
    else if (name == "wr_c")
    {
        val = &wr_c;
    }
    else if (name == "ps_p")
    {
        val = &ps_p;
    }
    else if (name == "as_p")
    {
        val = &as_p;
    }
    else if (name == "ls_p")
    {
        val = &ls_p;
    }
    else if (name == "ls_r")
    {
        val = &ls_r;
    }
    else if (name == "ls_g")
    {
        val = &ls_g;
    }
    else if (name == "ls_b")
    {
        val = &ls_b;
    }
    else if (name == "po_c")
    {
        val = &po_c;
    }
    else if (name == "fs_i")
    {
        val = &fs_i;
    }
    else if (name == "fs_v")
    {
        val = &fs_v;
    }
    else if (name == "ps_o")
    {
        val = &ps_o;
    }
    else if (name == "ls_o")
    {
        val = &ls_o;
    }
    else if (name == "fs_o")
    {
        val = &fs_o;
    }
    else if (name == "as_o")
    {
        val = &as_o;
    }
    else if (name == "tm_h")
    {
        val = &tm_h;
    }
    else if (name == "tm_m")
    {
        val = &tm_m;
    }
    else if (name == "tn_h")
    {
        val = &tn_h;
    }
    else if (name == "tn_m")
    {
        val = &tn_m;
    }
    else if (name == "po_u")
    {
        val = &po_u;
    }
    else
    {
        Serial.print("Something went wrong getting: ");
        Serial.println(name);
    }
    return val;
}
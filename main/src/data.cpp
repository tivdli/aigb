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
    bool set = false;
    if (name == "temp_reading_outside")
    {
        set = true ? temp_reading_outside != val : false;
        temp_reading_outside = val;
    }
    else if (name == "temp_reading_inside")
    {
        set = true ? temp_reading_inside != val : false;
        temp_reading_inside = val;
    }
    else if (name == "temp_setting_inside_day")
    {
        set = true ? temp_setting_inside_day != val : false;
        temp_setting_inside_day = val;
    }
    else if (name == "temp_setting_inside_night")
    {
        set = true ? temp_setting_inside_night != val : false;
        temp_setting_inside_night = val;
    }
    else if (name == "temp_setting_current")
    {
        set = true ? temp_setting_current != val : false;
        temp_setting_current = val;
    }
    else if (name == "hum_setting_current")
    {
        set = true ? hum_setting_current != val : false;
        hum_setting_current = val;
    }
    else if (name == "Hum_reading_inside")
    {
        set = true ? Hum_reading_inside != val : false;
        Hum_reading_inside = val;
    }
    else if (name == "Hum_reading_outside")
    {
        set = true ? Hum_reading_outside != val : false;
        Hum_reading_outside = val;
    }
    else if (name == "Hum_setting_inside_day ")
    {
        set = true ? Hum_setting_inside_day != val : false;
        Hum_setting_inside_day = val;
    }
    else if (name == "Hum_setting_inside_night")
    {
        set = true ? Hum_setting_inside_night != val : false;
        Hum_setting_inside_night = val;
    }
    else if (name == "co2_current_inside ")
    {
        set = true ? co2_current_inside != val : false;
        co2_current_inside = val;
    }
    else if (name == "light_reading_1 ")
    {
        set = true ? light_reading_1 != val : false;
        light_reading_1 = val;
    }
    else if (name == "light_reading_2 ")
    {
        set = true ? light_reading_2 != val : false;
        light_reading_2 = val;
    }
    else if (name == "water_level_readin")
    {
        set = true ? water_level_reading != val : false;
        water_level_reading = val;
    }
    else if (name == "pump_power_setting")
    {
        set = true ? pump_power_setting != val : false;
        pump_power_setting = val;
    }
    else if (name == "air_power_setting")
    {
        set = true ? air_power_setting != val : false;
        air_power_setting = val;
    }
    else if (name == "light_power_setting")
    {
        set = true ? light_power_setting != val : false;
        light_power_setting = val;
    }
    else if (name == "light_color_setting_r")
    {
        set = true ? light_color_setting_r != val : false;
        light_color_setting_r = val;
    }
    else if (name == "light_color_setting_g =")
    {
        set = true ? light_color_setting_g != val : false;
        light_color_setting_g = val;
    }
    else if (name == "light_color_setting_b")
    {
        set = true ? light_color_setting_b != val : false;
        light_color_setting_b = val;
    }
    else if (name == "current_profile")
    {
        set = true ? current_profile != val : false;
        current_profile = val;
    }
    else if (name == "feed_interval_setting")
    {
        set = true ? feed_interval_setting != val : false;
        feed_interval_setting = val;
    }
    else if (name == "feed_volume_setting")
    {
        set = true ? feed_volume_setting != val : false;
        feed_volume_setting = val;
    }
    else if (name == " pump_stat")
    {
        set = true ? pump_stat != val : false;
        pump_stat = val;
    }
    else if (name == " light_stat")
    {
        set = true ? light_stat != val : false;
        light_stat = val;
    }
    else if (name == " feed_stat")
    {
        set = true ? feed_stat != val : false;
        feed_stat = val;
    }
    else if (name == " air_stat")
    {
        set = true ? air_stat != val : false;
        air_stat = val;
    }
    else if (name == "h_morning")
    {
        set = true ? h_morning != val : false;
        h_morning = val;
    }
    else if (name == "m_morning")
    {
        set = true ? m_morning != val : false;
        m_morning = val;
    }
    else if (name == "h_night")
    {
        set = true ? h_night != val : false;
        h_night = val;
    }
    else if (name == "m_night")
    {
        set = true ? m_night != val : false;
        m_night = val;
    }
    else if (name == "Profile_user")
    {
        set = true ? Profile_user != val : false;
        Profile_user = val;
    }
    if (set){
        GBINT::update();
    }
}

int DATA::get(String name)
{
    int val = 0;
    if (name == "temp_reading_outside")
    {
        val = temp_reading_outside;
    }
    else if (name == "temp_reading_inside")
    {
        val = temp_reading_inside;
    }
    else if (name == "temp_setting_inside_day")
    {
        val = temp_setting_inside_day;
    }
    else if (name == "temp_setting_inside_night")
    {
        val = temp_setting_inside_night;
    }
    else if (name == "temp_setting_current")
    {
        val = temp_setting_current;
    }
    else if (name == "hum_setting_current")
    {
        val = hum_setting_current;
    }
    else if (name == "Hum_reading_inside")
    {
        val = Hum_reading_inside;
    }
    else if (name == "Hum_reading_outside")
    {
        val = Hum_reading_outside;
    }
    else if (name == "Hum_setting_inside_day ")
    {
        val = Hum_setting_inside_day;
    }
    else if (name == "Hum_setting_inside_night")
    {
        val = Hum_setting_inside_night;
    }
    else if (name == "co2_current_inside ")
    {
        val = co2_current_inside;
    }
    else if (name == "light_reading_1 ")
    {
        val = light_reading_1;
    }
    else if (name == "light_reading_2 ")
    {
        val = light_reading_2;
    }
    else if (name == "water_level_readin")
    {
        val = water_level_reading;
    }
    else if (name == "pump_power_setting")
    {
        val = pump_power_setting;
    }
    else if (name == "air_power_setting")
    {
        val = air_power_setting;
    }
    else if (name == "light_power_setting")
    {
        val = light_power_setting;
    }
    else if (name == "light_color_setting_r")
    {
        val = light_color_setting_r;
    }
    else if (name == "light_color_setting_g =")
    {
        val = light_color_setting_g;
    }
    else if (name == "light_color_setting_b")
    {
        val = light_color_setting_b;
    }
    else if (name == "current_profile")
    {
        val = current_profile;
    }
    else if (name == "feed_interval_setting")
    {
        val = feed_interval_setting;
    }
    else if (name == "feed_volume_setting")
    {
        val = feed_volume_setting;
    }
    else if (name == " pump_stat")
    {
        val = pump_stat;
    }
    else if (name == " light_stat")
    {
        val = light_stat;
    }
    else if (name == " feed_stat")
    {
        val = feed_stat;
    }
    else if (name == " air_stat")
    {
        val = air_stat;
    }
    else if (name == "h_morning")
    {
        val = h_morning;
    }
    else if (name == "m_morning")
    {
        val = m_morning;
    }
    else if (name == "h_night")
    {
        val = h_night;
    }
    else if (name == "m_night")
    {
        val = m_night;
    }
    else if (name == "Profile_user")
    {
        val = Profile_user;
    }
    return val;
}
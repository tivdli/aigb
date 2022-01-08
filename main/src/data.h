#ifndef data_h
#define data_h
class DATA
{
public:
    DATA();
    int temp_reading_inside = 20;
    int temp_reading_outside = 15;
    int temp_setting_inside_day = 21;
    int temp_setting_inside_night = 16;
    int Hum_reading_inside = 90;
    int Hum_reading_outside = 60;
    int Hum_setting_inside_day = 100;
    int Hum_setting_inside_night = 75;
    int co2_current_inside = 200;
    int light_reading_1 = 128;
    int light_reading_2 = 256;
    int water_level_reading = 1;
    int pump_power_setting = 69;
    int air_power_setting = 42;
    int light_power_setting = 93;
    int light_color_setting = 0xFF00FF;

    int feed_interval_setting = 12;
    int feed_volume_setting = 2;
    bool pump_stat = 1;
    bool light_stat = 0;
    bool feed_stat = 1;
    bool air_stat = 0;
};
#endif
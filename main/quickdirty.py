var =    """static int *temp_reading_inside;
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
    static bool *air_stat;"""

var = var.replace("static", "")
var = var.translate({ord(" "): None})
var = var.replace("\n", "")
var = var.replace(";",",")
var = var.replace("bool", "")
var = var.replace("int", "")
var = var.replace("*", " &")
print(var)
var = var.split()
c = 0
for i in range(0, len(var)):
    c+=1
    if c==3:
        var[i] = var[i]

print(var)
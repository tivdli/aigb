#ifndef data_h
#define data_h

class DATA
{
public:
    DATA();
    void set(String var, int val);
    int get(String name);
    int * pointer(String name);
private:
    int tr_1 = 15; //temp reading 1
    int tr_2 = 20; //temp reading 2
    int ts_d = 21; //temp setting day
    int ts_n = 16; //temp setting night
    int ts_c = 16; //temp setting current
    int hs_c = 65; //humidity setting current
    int hr_1 = 90; //humidity reading inside
    int hr_2 = 60; //humidity reading outside
    int hs_d = 100; //Humidity setting day
    int hs_n = 75; //Humidity setting night
    int cr_1 = 200; //CO2 reading current
    int lr_1 = 128; //LDR 1
    int lr_2 = 255; //LDR 2
    int wr_c = 1; //water level reading
    int ps_p = 69; //pump power setting
    int as_p = 42; //air power setting
    int ls_p = 93; //Light power setting
    int ls_r = 0xFF; //light red setting
    int ls_g = 0x00; //light green setting
    int ls_b = 0xFF; //light blue setting
    int po_c = -1; //currently selected profile
    int fs_i = 12; //feed interval setting (days)
    int fs_v = 2; //feed volume setting (ml)
    int ps_o = 1; //Pump setting on/off
    int ls_o = 1; //light setting on/off
    int fs_o = 1; //feed setting on/off
    int as_o = 1; //air setting on/off
    int tm_h = 7; //morning hour setting
    int tm_m = 0; //morgning minute setting
    int tn_h = 20; //night hour setting
    int tn_m = 30; //night minute setting
    int po_u; //profile in use or not
};
#endif
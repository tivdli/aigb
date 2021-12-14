#ifndef aigb_h
#define aigb_h

#include <Arduino.h>
#include <ESP32Servo.h>
#include <MHZ19.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SoftwareSerial.h>

#define CO2_TX 1
#define Vernevelaar 2
#define CO2_RX 3
#define Led_pin 4
#define Led_B 13
#define Pomp_Voeding 16
#define Pomp_Water 17
#define Fan_Control_1 18
#define Fan_Control_2 19
#define SDA 21
#define SCL 22
#define Peltier_1 23
#define Peltier_2 25
#define Led_R 26
#define Led_G 27
#define Servo_1 32
#define Servo_2 33
#define LDR_1 34
#define LDR_2 36
#define Water_Level 39


class AIGB{
    public:
        int test;
        int Test();
        AIGB();
        void init();
        int Calibrate(int *temp_day, int *temp_night ,int *Hum_day, int *Hum_night,int *pump_power, int *light_power,int *light_color,int *feed_interval,int *feed_volume);
        int Control();
        int LED();
        void Moisture();
        int Get_Co2();
        int Get_Hum_In();
        int Get_Hum_Out();

        int Get_Temp_Out();
        int Get_Temp_In();
        int Get_water();
        int Get_LDR_One();
        int Get_LDR_Two();

        void Servo_one();
        void Servo_two();
        void Water_Con();
        void Food_Con();
        void Pel_one();
        void Pel_two();
        void Control_Fan();
        void Led_Strip();
        int Time();
        Servo MyServo1;
        Servo MyServo2;
        
        #define CO2_TX 1
        #define Vernevelaar 2
        #define CO2_RX 3
        SoftwareSerial co2Serial;
        
        bool day;
        int *Setting[7];
        int* temp_day;
        int* temp_night;
        int* Hum_day;
        int* Hum_night;
        int* pump_power;
        int* light_power;
        int* light_color;
        int* feed_interval;
        int* feed_volume;
    private:
       
    

};

#endif
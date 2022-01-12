#ifndef aigb_h
#define aigb_h

#include <Arduino.h>
#include <ESP32Servo.h>
#include <MHZ19.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SoftwareSerial.h>
#include <data.h>
// #include <MHZ19PWM.h>

#define CO2_TX 27
#define Vernevelaar 2
#define CO2_RX 25
#define Led_pin 4
#define Led_B 13
#define Pomp_Voeding 3
#define Pomp_Water 1
#define Fan_Control_1 18
#define Fan_Control_2 19
#define SDA 21
#define SCL 22
#define Peltier_1 23
#define Peltier_2 25
#define CO2_PWM 14 //is not used?
#define Led_G 27// is not used?
#define Servo_1 32
#define Servo_2 33
#define LDR_1 34
#define LDR_2 36
#define Water_Level 39




class AIGB{
    public:
        
        int Test();
        AIGB(DATA * data);
        void init();
        //"main" functions
        int Calibrate(int *temp_day, int *temp_night ,int *Hum_day, int *Hum_night,int *pump_power, int *light_power,int *light_color,int *feed_interval,int *feed_volume);
        int Control();
        //measurment
        int LED();
        void Moisture();
        void Get_Co2();
        int Get_Hum();
        int Get_Temp();
        int Get_water();
        int Get_LDR();
        unsigned long _time = 0;
        bool waitMode = false;
        // control function
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
        
    
        SoftwareSerial mySerial;
        // MHZ19 myMHZ19;
        
        bool day;
        //pointers for settings
        int* temp_day;
        int* temp_night;
        int* Hum_day;
        int* Hum_night;
        int* pump_power;
        int* light_power;
        int* light_color;
        int* feed_interval;
        int* feed_volume;

        // Variable pointers for control and given values (temp waterlevel, hum enz.)
        int* Temp_Set;
        int* Hum_Set;

        //variables 
        int Water_level;
        int Hum_In;
        int Temp_In;
        int Hum_Out;
        int Temp_out;

        //int ppm;//variable for co2
        long ppm, ppm2=0.0;
        int light_valuel;
        int ppmrange = 5000;
        unsigned long pwmtime;
        // difference variables local
        int Temp_Dif;
        int Hum_Dif;

       // time variables
       int t; // the time in hours 

       //defined constants
       int NUM_LEDS=100; 

       //formule voor ldrs
       int ldr_1, ldr_2;
       float ldr_01=0.5*ldr_1+5;
       float ldr_02=0.5*ldr_2+5;
       #define RANGE 5000

       unsigned long getDataTimer = 0;
    //    MHZ19PWM mhz;
       DATA *aigb_data;
    private:
       long th,tl,l=0.0;
    

};

#endif


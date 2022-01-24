#ifndef aigb_h
#define aigb_h

#include <AM2320.h>
#include <Arduino.h>
#include <ESP32Servo.h>
#include <MHZ19.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SoftwareSerial.h>
#include <data.h>
// #include <MHZ19PWM.h>

//25 and 27 open
#define Vernevelaar 2

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
#define Led 38

#define PWM1_Ch    0
#define PWM1_Res   16
#define PWM1_Freq  1000

#define PWM2_Ch    1
#define PWM2_Res   16
#define PWM2_Freq  1000

class AIGB{
    public:
        
        
        AIGB(DATA * data);
        void init();
        //"main" functions
        void Calibrate();
        void Control();
        //measurment
        void LED();
        void Moisture();
        void Get_Co2();
        void Get_Hum();
        void Get_Temp();
        void Get_water();
        void Get_LDR();
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
        void Time();
        Servo MyServo1;
        Servo MyServo2;
        
    
        
        // MHZ19 myMHZ19;
        AM2320 th_1;
        AM2320 th_2;
        bool day;

        int food_timer;        
        float PWM2_DutyCycle=(AIGB::aigb_data->pump_power_setting/100)*(1024/2);
        float PWM1_DutyCycle=AIGB::aigb_data->feed_interval_setting*(1024/3);
        //variables 
        int Water_level;
        int Hum_In;
        int Temp_In;
        int Hum_Out;
        int Temp_out;

        //int ppm;//variable for co2 sensor
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
       int Led_Cycle;
      int Led_on;
       unsigned long getDataTimer = 0;
    //    MHZ19PWM mhz;

      

       DATA *aigb_data;


    private:
       long th,tl,l=0.0;
    

};

#endif


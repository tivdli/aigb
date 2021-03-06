#ifndef aigb_h
#define aigb_h

#include <Arduino.h>
#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>
//#include <MHZ19.h>
//#include <AsyncTCP.h>
//#include <ESPAsyncWebServer.h>
//#include <SoftwareSerial.h>
#include <data.h>
#include <SPI.h>
//#include <SoftwareWire.h>
// #include <MHZ19PWM.h>

//25 and 27 open
#define Vernevelaar 2

#define Led_pin 24
#define Led_B 13
#define Pomp_Voeding 3
#define Pomp_Water 1
#define Fan_Control_1 18
#define Fan_Control_2 19
#define SDA 21
#define SCL 22
#define SDA1 33
#define SCL1 32
#define Peltier_1 23
#define Peltier_2 25
#define CO2_PWM 14 //is not used?
#define Led_G // is not used?
#define MyServo_1 26
#define MyServo_2 27
#define LDR_1 5
#define LDR_2 8
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
        void Lightstrip();
        unsigned long _time = 0;
        bool waitMode = false;
        // control function
        
        void Water_Con();
        void Food_Con();
        void Pel_one();
        void Pel_two();
        void Control_Fan();
        void Time();
        void Servo_One();
        void Servo_Two();
    
        
        //MHZ19 myMHZ19;
        
        bool day;
        Adafruit_NeoPixel strip;
        int food_timer;        
        float PWM2_DutyCycle=(100/100)*(1024/2);
        float PWM1_DutyCycle=(1*(1024/3));
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
       int NUM_LEDS=21; 
        int pos = 0; 
       //formule voor ldrs
       int ldr_1, ldr_2;
       float ldr_01=0.5*ldr_1+5;
       float ldr_02=0.5*ldr_2+5;
       int Led_Cycle;
      int Led_on;
       unsigned long getDataTimer = 0;
   //  //    MHZ19PWM mhz;

      

       DATA *aigb_data;


    private:
       long th,tl,l=0.0;
    

};

#endif


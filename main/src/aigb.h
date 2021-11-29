#ifndef aigb_h
#define aigb_h

#include <Arduino.h>
#include <ESP32Servo.h>
#include <MHZ19.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

class AIGB{
    public:
        AIGB();
        void AIGB::init();
        
        
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
        Servo MyServo1;
        Servo MyServo2;
    private:
       
    

};

#endif
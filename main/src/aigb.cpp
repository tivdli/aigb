#include <Arduino.h>
#include <aigb.h>
#include <ESP32Servo.h>
#include <MHZ19.h>

#define CO2_TX 1
#define Vernevelaar 2
#define CO2_RX 3
#define Led 4
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

AIGB::AIGB(){

};

void AIGB::LED(){

} 

void AIGB::Moisture(){

} 


int AIGB::Get_Co2(){

} 
int AIGB::Get_Hum_In(){

} 

int AIGB:: Get_Hum_Out(){

}
int Get_Temp_Out(){

}

int Get_Temp_In(){

}
    
int Get_water(){

}

int Get_LDR_One(){

}

int Get_LDR_Two(){

}

void Servo_one(){

}

void Servo_two(){

}

void Water_Con(){

}

void Food_Con(){

}
        
void Pel_one(){

}
void Pel_two(){

}

void Control_Fan(){

}

void Led_Strip(){

}
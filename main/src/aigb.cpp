#include <Arduino.h>
#include <aigb.h>
#include <ESP32Servo.h>
#include <MHZ19.h>

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


AIGB::AIGB(){
    //now used to define the pin modes and start timers (setup from arduino)
    //still looking if this is the smartest way can not find a lot of examples with h files and with arduino libaries.    
    int ADS_max=4096;
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    Servo MyServo1;
    Servo MyServo2;
    MyServo1.attach(Servo_1,0,180);
    MyServo2.attach(Servo_2,0,180);
    // defined all the outputs
    pinMode(Led_pin,OUTPUT);
    pinMode(Vernevelaar,OUTPUT);
    pinMode(Led_B,OUTPUT);
    pinMode(Led_G,OUTPUT);
    pinMode(Led_R,OUTPUT);
    pinMode(Pomp_Voeding,OUTPUT);
    pinMode(Pomp_Water,OUTPUT);
    pinMode(Fan_Control_1,OUTPUT);
    pinMode(Fan_Control_2,OUTPUT);
    pinMode(Peltier_1,OUTPUT);
    pinMode(Peltier_2,OUTPUT);

    // defines the inputs
    pinMode(Water_Level,INPUT);
    pinMode(LDR_1,INPUT); 
    pinMode(LDR_2,INPUT);   
    init();
}
void AIGB::init(){

}

// function to led a led blink hopfully from there we can built further
int AIGB::LED(){
    digitalWrite(Led_pin,HIGH);
    delay(300);
    digitalWrite(Led_pin,LOW);
    delay(300);
    digitalWrite(Led_pin,HIGH);
    delay(300);
    digitalWrite(Led_pin,LOW);
    
}

void AIGB::Moisture(){

} 


int AIGB::Get_Co2(){

} 

int AIGB::Get_Hum_In(){

} 

int AIGB:: Get_Hum_Out(){

}
int AIGB::Get_Temp_Out(){

}

int AIGB:: Get_Temp_In(){

}
    
int AIGB:: Get_water(){

}

int AIGB:: Get_LDR_One(){

}

int AIGB:: Get_LDR_Two(){

}

// lets servo one move
void AIGB:: Servo_one(){
    MyServo1.write(90);
    delay (200);
    MyServo1.write(180);
    delay (200);
    MyServo1.write(0);
    delay(200); 
}

// lets servo two move
void AIGB:: Servo_two(){
    MyServo2.write(90);
    delay (200);
    MyServo2.write(180);
    delay (200);
    MyServo2.write(0);
    delay(200); 
}

void AIGB:: Water_Con(){

}

void AIGB:: Food_Con(){

}
        
void AIGB:: Pel_one(){

}

void AIGB:: Pel_two(){

}

void AIGB:: Control_Fan(){

}

void AIGB:: Led_Strip(){

}
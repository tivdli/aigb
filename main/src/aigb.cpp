#include <Arduino.h>
#include <aigb.h>
#include <ESP32Servo.h>
#include <data.h>
#include <AM2320.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>
#include <SoftwareSerial.h>
#include <SPI.h> 
#include <Adafruit_NeoPixel.h>
// #include <MHZ19PWM.h>

AIGB::AIGB(DATA * data){
    init();
    AIGB::aigb_data = data;
}

void AIGB::init(){
    //makes four PWM signals for the servos
    int ADS_max=4096;
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    //making two servo objects
    Servo MyServo1;
    Servo MyServo2;
    MyServo1.attach(Servo_1,500,2400);
    MyServo2.attach(Servo_2,0,180);
    // making am2320 sensor object
    
    AM2320 th_1;
    AM2320 th_2;
    // defined all the outputs
    pinMode(Led_pin,OUTPUT);
    pinMode(Vernevelaar,OUTPUT);
    pinMode(Led_B,OUTPUT);
    pinMode(Led_G,OUTPUT);
    //pinMode(Led_R,OUTPUT);
    pinMode(CO2_PWM,INPUT);
    pinMode(Pomp_Voeding,OUTPUT);
    pinMode(Pomp_Water,OUTPUT);
    pinMode(Fan_Control_1,OUTPUT);
    pinMode(Fan_Control_2,OUTPUT);
    pinMode(Peltier_1,OUTPUT);
    pinMode(Peltier_2,OUTPUT);
    pinMode(SDA,OUTPUT);
    pinMode(SCL,OUTPUT);

    // defines the inputs
    pinMode(Water_Level,INPUT);
    pinMode(LDR_1,INPUT); 
    pinMode(LDR_2,INPUT);
   // SoftwareSerial co2Serial(CO2_RX, CO2_TX);
    SoftwareSerial AM2320Serial(SDA, SCL);
    //co2Serial.begin(9600); 
    AM2320Serial.begin(9600);
    Wire.begin();
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, Led_B, NEO_GRB + NEO_KHZ800);

    // MHZ19 myMHZ19;                               
    // SoftwareSerial mySerial(CO2_RX, CO2_TX); 
    

    //Pwm pinnen instellen
    ledcAttachPin(Pomp_Voeding, PWM1_Ch);
    ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);

    ledcAttachPin(Pomp_Water, PWM2_Ch);
    ledcSetup(PWM2_Ch, PWM1_Freq, PWM1_Res);

    Serial2.begin(9600);                   
    //myMHZ19.begin(mySerial);
    //MHZ19PWM mhz(2, MHZ_ASYNC_MODE);
    //mhz.useLimit(5000);

}

void AIGB:: Time(){
    int t=1;
   
}

void AIGB::Calibrate(){
    //Time();
    if (AIGB::aigb_data->Profile_user==false){
    bool day;
    
    if (20 > t && t> 7){
        day=true;
        AIGB::aigb_data->temp_setting_current=AIGB::aigb_data->temp_setting_inside_day;
       AIGB::aigb_data->hum_setting_current=AIGB::aigb_data->Hum_setting_inside_day;
    }

    else{
        day=false;
        
        AIGB::aigb_data->temp_setting_current=AIGB::aigb_data->temp_setting_inside_night;
        AIGB::aigb_data->hum_setting_current=AIGB::aigb_data->Hum_setting_inside_night;
    }
    }
    else{
        //profile is set by user
    }
    // check waterlevel
    Get_water();
    
    // check humidity
    Get_Hum();

    // check light
    Get_LDR();

    Get_Co2();
    
    //Control();
    
}
void AIGB::Control(){
    
    //settings instellen
   
     
    //check settings (looks if the settings are still compatible with the time of day)
    // int* temp_setting= &temp_setting_inside_day;
    
    // temp difference
    Temp_Dif = AIGB::aigb_data->temp_setting_current-Temp_In;
    Hum_Dif = AIGB::aigb_data->hum_setting_current-Hum_In;
    if (-1<Temp_Dif&& Temp_Dif>1){
        //temp control
    }

    else{
        // keep Temp as it is
    }

    if (-1<Hum_Dif && Hum_Dif>1){
        //huminity control
        Moisture();
    }

    else{
        digitalWrite(Vernevelaar,LOW);
    }
    
    LED();
  
    
}

void AIGB::LED(){
    
}

void AIGB::Moisture(){
    digitalWrite(Vernevelaar,HIGH);
} 

// has to be tested if it function 
void AIGB::Get_Co2(){
    
    // Start serial in setup routine

    //Command and response
    byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
    byte response[9];

    // Clear serial buffer
    while (mySerial.available() > 0) {
        mySerial.read();
    }

    // Send command and read response
    mySerial.write(cmd, 9);
    mySerial.readBytes(response, 9);

    // Check if response is valid
    if (response[0] != 0xFF) {
    // wrong starting byte from co2 sensor
    }
    if (response[1] != 0x86) {
    // Wrong command from co2 sensor
    }

    AIGB::aigb_data->co2_current_inside = (256 * response[2]) + response[3];
    Temp_In = response[4]-40;
    byte status = response[5];
    int minimum = (256 * response[6]) + response[7];
    
    Serial.println("ppm: ");
    Serial.println(ppm);
    Serial.println("temp:");
    Serial.println(Temp_In);
    

    delay(5000);
    }
   

   
    

void AIGB::Get_Hum(){
    AIGB::aigb_data->Hum_reading_inside=th_1.h;
    AIGB::aigb_data->Hum_reading_outside=th_2.h;
    
    } 

void AIGB::Get_Temp(){
    AIGB::aigb_data->temp_reading_inside=th_1.t;
    AIGB::aigb_data->temp_reading_outside=th_2.t;
}

    
void AIGB:: Get_water(){
    AIGB::aigb_data->water_level_reading=   2  ;
}

void AIGB:: Get_LDR(){
    ldr_1=analogRead(LDR_1);
    ldr_2=analogRead(LDR_2);
    AIGB::aigb_data->light_reading_1=ldr_01;
    AIGB::aigb_data->light_reading_2=ldr_02;
 }

// lets servo one move
void AIGB:: Servo_one(){
    MyServo1.write(180);
    delay(15);
    MyServo1.write(179);
    delay(15);
    MyServo1.write(175);
  
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

void AIGB::Water_Con(){
    
    ledcWrite(PWM2_Ch, PWM2_DutyCycle);
}

void AIGB::Food_Con(){
    //heel kort aan
    if (AIGB::aigb_data->feed_interval_setting<food_timer){
        
        ledcWrite(PWM1_Ch, PWM1_DutyCycle);
    }
    else{
        ledcWrite(PWM1_Ch, 0);
    }
}
#include <Arduino.h>
#include <aigb.h>
#include <ESP32Servo.h>
#include <MHZ19.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>
#include <SoftwareSerial.h>
#include <SPI.h> 

AIGB::AIGB(){

    
    init();
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
    MyServo1.attach(Servo_1,0,180);
    MyServo2.attach(Servo_2,0,180);
    // making am2320 sensor object
    
    Adafruit_AM2320 AM2320 = Adafruit_AM2320();
    
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
    //SoftwareSerial co2Serial(CO2_RX, CO2_TX);
    //SoftwareSerial AM2320(SDA, SCL);
    //co2Serial.begin(9600); 
    //AM2320.begin(9600);
}

int AIGB:: Time(){
    int t=1;
    return t;
}
int AIGB::Test(){
    int test=1;
    printf("%d", test);
    return 0;
}
int AIGB::Calibrate(int *temp_day, int *temp_night ,int *Hum_day, int *Hum_night,int *pump_power, int *light_power,int *light_color,int *feed_interval,int *feed_volume){
    int t=Time();
    t=6;
    bool day;
    
    if (20 > t > 7){
        day=true;
        int *Temp=temp_day;
        int *Hum=Hum_day;
    }

    else{
        day=false;
        printf("\n eerste pointer: %d", *temp_night);
        printf(" met adres: %d", temp_night);
        int *Temp=temp_night;
        int *Hum=Hum_night;
        printf("\n tweede toegeschreven: %d", *Temp);
        printf(" met adres: %d",Temp);
    }
    Control();
    return 1;
}
int AIGB::Control(){
    
    //settings instellen
   
    
    //check settings (looks if the settings are still compatible with the time of day)
    // int* temp_setting= &temp_setting_inside_day;
    
    // temp difference
   
    delay(500);
    // check waterlevel
    // check humidity
    // check light
    
    return 1;
}
// function to led a led blink hopfully from there we can built further
int AIGB::LED(){
    digitalWrite(Vernevelaar,HIGH);
    Serial.println("on");
    delay(300);
    digitalWrite(Vernevelaar,LOW);
    Serial.println("off");
    delay(300);
    digitalWrite(Vernevelaar,HIGH);
    Serial.println("on");
    delay(300);
    digitalWrite(Vernevelaar,LOW);
    Serial.println("off");
    return 1;
}

void AIGB::Moisture(){

} 

// has to be tested if it function 
int AIGB::Get_Co2(){
    
     
    byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
    byte response[9];
    co2Serial.write(cmd, 9);
      // The serial stream can get out of sync. The response starts with 0xff, try to resync.
    while (co2Serial.available() > 0 && (unsigned char)co2Serial.peek() != 0xFF) {
        co2Serial.read();
    }

    memset(response, 0, 9);
    co2Serial.readBytes(response, 9);

    if (response[1] != 0x86){
        Serial.println("Invalid response from co2 sensor!");
        return -1;
    }

    byte crc = 0;
    for (int i = 1; i < 8; i++) {
        crc += response[i];
    }

    crc = 255 - crc + 1;

    if (response[8] == crc) {
        int responseHigh = (int) response[2];
        int responseLow = (int) response[3];
        int ppm = (256 * responseHigh) + responseLow;
        return ppm;
        } 
        
        else {
        Serial.println("CRC error!");
        return -1;
    }
    }
    

int AIGB::Get_Hum_In(){
    return 1;
} 

int AIGB:: Get_Hum_Out(){
    return 1;
}
int AIGB::Get_Temp_Out(){
    return 1;
}

int AIGB:: Get_Temp_In(){
    return 1;
}
    
// int AIGB:: Get_water(){

// }

// int AIGB:: Get_LDR_One(){

// }

// int AIGB:: Get_LDR_Two(){

// }

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

// void AIGB:: Water_Con(){

// }

// void AIGB:: Food_Con(){

// }
        
// void AIGB:: Pel_one(){

// }

// void AIGB:: Pel_two(){

// }

// void AIGB:: Control_Fan(){

// }

// void AIGB:: Led_Strip(){

// }
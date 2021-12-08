#include <Arduino.h>
#include <aigb.h>
#include <ESP32Servo.h>
#include <MHZ19.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>
#include <Wire.h>
#include <RTClib.h>
#include <list>
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
    // making rtc
    RTC_DS1307 rtc;
    
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
    SoftwareSerial co2Serial(CO2_RX, CO2_TX);
    
    co2Serial.begin(9600); 
    //AM2320.begin(9600);
}

int AIGB:: Time(){
    int t=1;
    return t;
}
int AIGB::Calibrate(int *Settings){
    int t=Time();
    bool day;
    if (20 > t > 7){
        day=true;
    }
    else{
        day=false;
    }
    Control(Settings,day);
    return day;
}
int AIGB::Control(int *Settings,bool day){
    
    //settings instellen
     if (day==true){
        

        int *Setting[7]={&Settings[0],&Settings[2],&Settings[4],&Settings[5],&Settings[6],&Settings[7],&Settings[8]};
    }
    else{
        
        int *Setting[7]={&Settings[1],&Settings[3],&Settings[4],&Settings[5],&Settings[6],&Settings[7],&Settings[8]};
    }
    //Setting (0=day temp, 1=night temp, 2= day hum, 3=night hum, 4=pomp power, 5= light power, 6= light color, 7= food interval, 8=food volume)
    //check settings (looks if the settings are still compatible with the time of day)
    // int* temp_setting= &temp_setting_inside_day;
    // temp difference
    

    // check waterlevel
    // check humidity
    // check light
    
    return 1;
}
// function to led a led blink hopfully from there we can built further
int AIGB::LED(){
    digitalWrite(Vernevelaar,HIGH);
  
    delay(300);
    digitalWrite(Vernevelaar,LOW);
  
    delay(300);
    digitalWrite(Vernevelaar,HIGH);
   
    delay(300);
    digitalWrite(Vernevelaar,LOW);
    
    return 1;
}

// int AIGB::Measurment_In(){
//       // Read the temperature and the humidity:
//     float tempCIn = AM2320.readTemperature();
//     float humidityIn = AM2320.readHumidity();

//     return tempCIn,humidityIn;
// } 

// int AIGB::Measurment_Out(){
//       // Read the temperature and the humidity:
//     float tempC = AM2320.readTemperature();
//     float humidity = AM2320.readHumidity();

//     return tempC,humidity;
// } 

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
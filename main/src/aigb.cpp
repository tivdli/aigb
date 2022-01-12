#include <Arduino.h>
#include <aigb.h>
#include <ESP32Servo.h>
#include <data.h>

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
    MyServo1.attach(Servo_1,0,180);
    MyServo2.attach(Servo_2,0,180);
    // making am2320 sensor object
    
    Adafruit_AM2320 AM2320 = Adafruit_AM2320();
    
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
    //SoftwareSerial AM2320(SDA, SCL);
    //co2Serial.begin(9600); 
    //AM2320.begin(9600);
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, Led_B, NEO_GRB + NEO_KHZ800);

    // MHZ19 myMHZ19;                               
    // SoftwareSerial mySerial(CO2_RX, CO2_TX); 
    

  //Serielle Intialisierung
    
    Serial2.begin(9600);                   
    //myMHZ19.begin(mySerial);
    //MHZ19PWM mhz(2, MHZ_ASYNC_MODE);
    //mhz.useLimit(5000);

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
    //Time();
    
    bool day;
    
    if (20 > t > 7){
        day=true;
        int *Temp_Set=temp_day;
        int *Hum_Set=Hum_day;
    }

    else{
        day=false;
        
        int *Temp_Set=temp_night;
        int *Hum_Set=Hum_night;
        
    }
    
    // check waterlevel
    //Get_water();
    
    // check humidity
    //Get_Hum();
    // check light
    //Get_LDR();

    //Get_Co2();
    
    //Control();
    return 1;
}
int AIGB::Control(){
    
    //settings instellen
   
     
    //check settings (looks if the settings are still compatible with the time of day)
    // int* temp_setting= &temp_setting_inside_day;
    
    // temp difference
    Temp_Dif = *Temp_Set-Temp_In;
    Hum_Dif = *Hum_Set-Hum_In;
    if (-1>Temp_Dif>1){
        //temp control
    }

    else{
        // keep Temp as it is
    }

    if (-1>Hum_Dif>1){
        //huminity control
        //Moisture();
    }

    else{
        digitalWrite(Vernevelaar,LOW);
    }
    
    LED();
  
    return 1;
}
// function to led a led blink hopfully from there we can built further
int AIGB::LED(){
    return 1;
}

void AIGB::Moisture(){
    digitalWrite(Vernevelaar,HIGH);
} 

// has to be tested if it function 
int AIGB::Get_Co2(){
    
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

    int ppm = (256 * response[2]) + response[3];
    int temp = response[4]-40;
    byte status = response[5];
    int minimum = (256 * response[6]) + response[7];
    
    Serial.println("\nppm: ");
    Serial.println(ppm);
    Serial.println("temp:");
    Serial.println(temp);
    delay(5000);
    }
    //Serial.println(ppm);  

   
    
// int AIGB::Get_Co2(){
//       unsigned long ms = millis();

//   if (waitMode)
//   {
//     if (ms - _time >= 500 || ms < _time)
//     {
//       _time = ms;
//       bool state = mhz.isDataReady();
//       Serial.print(F("isDataReady: "));
//       Serial.println(state);
      
//       if (state)
//       {
//         waitMode = false;
//         unsigned long start = millis();
//         float co2 = mhz.getCO2();
//         unsigned long duration = millis() - start;

//         Serial.print(F("CO2: "));
//         Serial.println(co2);
//         Serial.print(F("Duration: "));
//         Serial.println(duration);
//         Serial.println();
//                 }
//     }
//   }
//   else
//   {
//     if (ms - _time >= 10000 || ms < _time)
//     {
//       _time = ms;
//       waitMode = true;
//       mhz.requestData();
//       Serial.println(F("Request data"));
//     }
//   }
    
  
//     pwmtime = pulseIn(CO2_PWM, HIGH, 2000000) / 1000;
//     Serial.print(pwmtime);
//     float pulsepercent = pwmtime / 1004.0;
//     ppm = ppmrange * pulsepercent;
//     Serial.print("\nCO2 Konzentration in der Luft in PPM: ");
//     Serial.println(ppm);
//     delay(5000);

// }

int AIGB::Get_Hum(){
    return 1;
} 

int AIGB::Get_Temp(){
    return 1;
}

    
int AIGB:: Get_water(){

}

int AIGB:: Get_LDR(){
    ldr_1=analogRead(LDR_1);
    ldr_2=analogRead(LDR_2);
    printf("%f",ldr_01);
    printf("%f",ldr_02);
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


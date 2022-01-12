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
    MyServo1.attach(Servo_1,500,2400);
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

void AIGB:: Time(){
    int t=1;
   
}
void AIGB::Test(){
    int test=1;
    printf("%d", test);
    
}
void AIGB::Calibrate(){
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
void AIGB::Control(){
    
    //settings instellen
   
     
    //check settings (looks if the settings are still compatible with the time of day)
    // int* temp_setting= &temp_setting_inside_day;
    
    // temp difference
    Temp_Dif = AIGB::aigb_data->temp_setting_current-Temp_In;
    Hum_Dif = AIGB::aigb_data->hum_setting_current-Hum_In;
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
  
    
}
// function to led a led blink hopfully from there we can built further
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

    int ppm = (256 * response[2]) + response[3];
    Temp_In = response[4]-40;
    byte status = response[5];
    int minimum = (256 * response[6]) + response[7];
    
    Serial.println("\nppm: ");
    Serial.println(ppm);
    Serial.println("temp:");
    Serial.println(Temp_In);
    AIGB::aigb_data->co2_current_inside=ppm;

    delay(5000);
    }
   

   
    

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
    MyServo1.write(180);
    delay(15);
    MyServo1.write(179);
    delay(15);
    MyServo1.write(175);
  
}`1

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

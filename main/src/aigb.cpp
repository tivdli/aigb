#include <Arduino.h>
#include <aigb.h>
#include <ESP32Servo.h>
#include <data.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>
//#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>



//#include <MHZ19PWM.h>
Servo MyServo1;
Servo MyServo2;


/* To Do
Logica

 */

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
    MyServo1.setPeriodHertz(50);
    MyServo1.attach(MyServo_1,500,2400);
    MyServo2.attach(MyServo_2,500,2400);
    // making am2320 sensor object
    
 
    // defined all the outputs
    pinMode(Led_pin,OUTPUT);
    pinMode(Vernevelaar,OUTPUT);
    pinMode(Led_B,OUTPUT);
    //pinMode(Led_G,OUTPUT);
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

    
    
    //SoftwareSerial Serial2(SDA1 , SCL1);

    //AM2320 Sensor1(SDA,SCL);
    //AM2320 Sensor2 (33,32);
 
    Wire.begin();
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, Led_pin, NEO_GRB + NEO_KHZ800);
    strip.begin();
    Lightstrip();
    // MHZ19 myMHZ19;                               
    // SoftwareSerial mySerial(CO2_RX, CO2_TX); 
    

    //Pwm pinnen instellen
    ledcAttachPin(Pomp_Voeding, PWM1_Ch);
    ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);

    ledcAttachPin(Pomp_Water, PWM2_Ch);
    ledcSetup(PWM2_Ch, PWM1_Freq, PWM1_Res);

                      
    //myMHZ19.begin(mySerial);
    //MHZ19PWM mhz(2, MHZ_ASYNC_MODE);
    //mhz.useLimit(5000);

}

void AIGB:: Time(){
    int t=1;
   
}

void AIGB::Servo_One(){
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
		// in steps of 1 degree
		MyServo1.write(pos);    // tell servo to go to position in variable 'pos'
		delay(15);             // waits 15ms for the servo to reach the position
	}
	for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
		MyServo1.write(pos);    // tell servo to go to position in variable 'pos'
		delay(15);             // waits 15ms for the servo to reach the position
	}
    Serial.println("ik kom hier wel");
}

void AIGB::Calibrate(){
    //Time();
    if (AIGB::aigb_data->get("po_u")<=0){
    bool day;
    
    if (20 > t && t> 7){
        day=true;
        AIGB::aigb_data->set("ts_c",AIGB::aigb_data->get("ts_d"));
       AIGB::aigb_data->set("hs_c",AIGB::aigb_data->get("hs_d"));
    }

    else{
        day=false;
        
        AIGB::aigb_data->set("ts_c",AIGB::aigb_data->get("ts_n"));
        AIGB::aigb_data->set("hs_c",AIGB::aigb_data->get("hs_n"));
    }
    }
    else{
        //profile is set by user
    }
    // check waterlevel
    Get_water();
    
    // check humidity
    //Get_Hum();

    // check light
    Get_LDR();

    Get_Co2();
    
    //Control();
    
}
void AIGB::Control(){
    //Adafruit_Neopixel strip = AIGB::strip;
    //settings instellen
   
     
    //check settings (looks if the settings are still compatible with the time of day)
    // temp difference
    Temp_Dif = AIGB::aigb_data->get("ts_c")-Temp_In;
    Hum_Dif = AIGB::aigb_data->get("hs_c")-Hum_In;
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

void AIGB::Lightstrip()
{
    if (aigb_data->get("ls_o") == true)
    {
        strip.Color(aigb_data->get("ls_r"), aigb_data->get("ls_g"), aigb_data->get("ls_b"));
        strip.setBrightness(aigb_data->get("ls_p"));
        strip.show();
    }
    else if (aigb_data->get("ls_o") == false)
    {
        strip.show();
    }
}
void AIGB::LED(){
    if (AIGB::aigb_data->get("wr_c")==0){
        Led_Cycle=200;
        for (int i=0;i<=Led_Cycle;i++){
            if (Led_on==1 && i<=Led_Cycle){
                Led_on=0;
                digitalWrite(Led , HIGH);
                i=0;
            }
            else if(Led_on==0 && i<=Led_Cycle){
                Led_on=1;
                digitalWrite(Led ,LOW);
                i=0;
            }
        }
    }
}

void AIGB::Moisture(){
    digitalWrite(Vernevelaar,HIGH);
} 

// has to be tested if it function 
void AIGB::Get_Co2(){
    
    // Start serial in setup routine
    do{
    th = pulseIn(CO2_PWM, HIGH, 1004000) / 1000;
    tl = 1004 - th;
    //AIGB::aigb_data->set("cr_1",2000 * (th-2)/(th+tl-4));
    ppm = 2000 * (th-2)/(th+tl-4);
    
  } while (th == 0);
  delay(300);
  Serial.println(ppm);
    }
   

   
    

void AIGB::Get_Hum(){
    //get the value of the two huminity sensors
    // AIGB::aigb_data->set("Hum_reading_inside",th_1.h);
    // AIGB::aigb_data->set("Hum_reading_outside",th_2.h);
//     switch(Sensor1.Read()) {
//     case 2:
//       Serial.println("CRC failed");
//       break;
//     case 1:
//       Serial.println("Sensor1 offline");
//       break;
//     case 0:
//       Serial.print("Humidity: ");
//       Serial.print(Sensor1.h);
//       Serial.print("%\t Temperature: ");
//       Serial.print(Sensor1.t);
//       Serial.println("*C");
//       break;
//   }
//    switch(Sensor2.Read()) {
//     case 2:
//       Serial.println("CRC failed");
//       break;
//     case 1:
//       Serial.println("Sensor1 offline");
//       break;
//     case 0:
//       Serial.print("Humidity: ");
//       Serial.print(Sensor2.h);
//       Serial.print("2%\t Temperature: ");
//       Serial.print(Sensor2.t);
//       Serial.println("*C 2");
//       break;
//   }
//   delay(300);
    } 

void AIGB::Get_Temp(){
    //get the value of the temp according to the am2320
    // AIGB::aigb_data->set("tr_2",th_1.t);
    // AIGB::aigb_data->set("tr_1",th_2.t);
}

    
void AIGB:: Get_water(){
    AIGB::aigb_data->set("wr_c",   2  );
}

void AIGB:: Get_LDR(){
    ldr_1=analogRead(LDR_1);
    ldr_2=analogRead(LDR_2);
    AIGB::aigb_data->set("lr_1",ldr_01);
    AIGB::aigb_data->set("lr_2",ldr_02);
 }

// // lets servo one move
// void AIGB:: Servo_one(){
//     MyServo1.write(180);
//     delay(15);
//     MyServo1.write(179);
//     delay(15);
//     MyServo1.write(175);
  
// }

// // lets servo two move
// void AIGB:: Servo_two(){
//     MyServo2.write(90);
//     delay (200);
//     MyServo2.write(180);
//     delay (200);
//     MyServo2.write(0);
//     delay(200); 
// }

void AIGB::Water_Con(){
    
    ledcWrite(PWM2_Ch, PWM2_DutyCycle);
}

void AIGB::Food_Con(){
    //heel kort aan
    if (AIGB::aigb_data->get("fs_i")<food_timer){
        
        ledcWrite(PWM1_Ch, PWM1_DutyCycle);
    }
    else{
        ledcWrite(PWM1_Ch, 0);
    }
}
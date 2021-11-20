#include <Arduino.h>
#include <gbint.h>
#include <ASyncTCP.h>
#include <ESPAsyncWebServer.h>

void GBINT::GBINT(char[] ssid, char[] pass, int port = 80){
    _ssid = ssid;
    _pass = pass;
    _port = port; 
}

bool GBINT::connect(){

}

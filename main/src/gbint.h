#ifndef gbint_h
#define gbint_h

#include <Arduino.h>
#include <ASyncTCP.h>
#include <ESPAsyncWebServer.h>
class GBINT{
    public:
        GBINT(AsyncWebServer server, AsyncWebSocket ws, AsyncEventSource events);
        bool wifiStart();
    private:
        AsyncWebSocket ws;
        AsyncWebServer server;
        AsyncEventSource events;

        String respond();

};
#endif
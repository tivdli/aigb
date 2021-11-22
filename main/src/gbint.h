#ifndef gbint_h
#define gbint_h

#include <Arduino.h>
#include <ASyncTCP.h>
#include <ESPAsyncWebServer.h>
class GBINT{
    public:
        GBINT();
        int getNumber();
};
#endif
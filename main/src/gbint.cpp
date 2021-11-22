#include <Arduino.h>
#include <gbint.h>
#include <ASyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

GBINT::GBINT(AsyncWebServer server, AsyncWebSocket ws, AsyncEventSource events){
    GBINT::ws = ws;
    GBINT::server = server;
    GBINT::events = events;
}

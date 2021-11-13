#include "Config.h"
#include "fileUtils.h"
#include <WiFiManager.h>

extern WiFiManager  wifiMgr;
extern int          wifiBtn;

extern void handleIRbutton(int btn);

#define UDP_PORT        8266
#define MAX_UDP_PACKET  25

#define MIME_JSON       "text/json"
#define MIME_HTML       "text/html"
#define MIME_TEXT       "text/plain"

void initWebserver();
void loopWebserver();
void broadcastEffect(int effect);
void broadcastButton(String btn);
void broadcastSpeed();

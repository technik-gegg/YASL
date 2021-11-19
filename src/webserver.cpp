#include "webserver.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266NetBIOS.h>
#include <WiFiUdp.h>
#include "effectlist.h"

extern byte     newEffect;
extern byte     selectedEffect;
extern bool     isMuted;
extern bool     isPowered;
extern bool     inhibitBroadcast;
extern int      fireCooling, fireSparking, twinkleColor, numBalls;
extern int      wipeSpeed, fadeSpeed, twinkleSpeed, theaterCaseSpeed, 
                meteorRainSpeed, rippleSpeed, blendSpeed, cylonSpeed;

extern void     setEffect(int effect);

WiFiManager             wifiMgr;
ESP8266WebServer        webServer(80);
ESP8266HTTPUpdateServer httpUpdateServer;
WiFiUDP                 udp;
char                    deviceName[15];
char                    udpPacket[MAX_UDP_PACKET];
IPAddress               broadcast = IPAddress(239, 0, 82, 66);
bool                    isBroadcasting = false;

void sendResponse(int status, const char* mime, String value) {
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    webServer.sendHeader("X-Content-Type-Options", "no-sniff");
    webServer.send(status, mime, value);
}

void handle404() {
    String message = "<h1>File Not Found</h1>";
    message += "<p>URI: " + webServer.uri() + "</p>";
    message += "<p>Method: " + String((webServer.method() == HTTP_GET) ? "GET" : "POST") + "</p>";
    if(webServer.args() > 0) {
        message += "<p>Arguments:</p><ul>";
        for (uint8_t i = 0; i < webServer.args(); i++) {
            message += "<li>" + webServer.argName(i) + ": " + webServer.arg(i) + "</li>";
        }
        message += "</ul>";
    }
    sendResponse(404, MIME_HTML, message);
}

void handleButton(String value) {
    if(value == "left") {
        handleIRbutton(IR_LEFT);
    }
    else if(value == "right") {
        handleIRbutton(IR_RIGHT);
    }
    else if(value == "minus") {
        handleIRbutton(IR_MINUS);
    }
    else if(value == "pip") {
        handleIRbutton(IR_PIP);
    }
    else if(value == "av") {
        handleIRbutton(IR_AVTV);
    }
    else if(value == "menu") {
        handleIRbutton(IR_MENU);
    }
    else if(value == "mute") {
        handleIRbutton(IR_MUTE);
    }
    else if(value == "power") {
        handleIRbutton(IR_POWER);
    }
    else if(value == "inc") {
        handleIRbutton(IR_UP);
    }
    else if(value == "dec") {
        handleIRbutton(IR_DOWN);
    }
}

void handleSpeed(int speed) {
    switch(selectedEffect) {
        case E_COLORWIPE:
        wipeSpeed = speed;
        break;

        case E_FADE:
        fadeSpeed = speed;
        break;

        case E_TWINKLE:
        case E_TWINKLE2:
        twinkleSpeed = speed;
        break;

        case E_RIPPLE:
        case E_RIPPLE2:
        rippleSpeed = speed;
        break;

        case E_BLENDWAVE:
        blendSpeed = speed;
        break;

        case E_THEATERCASE:
        case E_THEATERCASE2:
        theaterCaseSpeed = speed;
        break;

        case E_METEORRAIN:
        case E_METEORRAIN2:
        meteorRainSpeed = speed;
        break;
        
        case E_CYLONBOUNCE:
        cylonSpeed = speed;
        break;

        case E_FIRE: 
        fireCooling = speed;
        break;

        case E_BOUNCINGBALLS: 
        numBalls = speed;
        break;
    }

}

void initWebserver() {
    
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    WiFi.softAPmacAddress(mac);
    sprintf(deviceName, "YASL-%02X%02X%02X", mac[WL_MAC_ADDR_LENGTH - 3], mac[WL_MAC_ADDR_LENGTH - 2], mac[WL_MAC_ADDR_LENGTH - 1]);
    wifiMgr.setConfigPortalBlocking(false);
    
    //wifiMgr.resetSettings();                // wipe credentials
    
    if(wifiMgr.autoConnect(deviceName)){
        Serial.println("WiFi connected");
    }
    else {
        Serial.println("Configportal running");
    }

    wifi_set_sleep_type(NONE_SLEEP_T);

    httpUpdateServer.setup(&webServer);
    webServer.on("/mute", HTTP_GET, []() {
        handleButton(String("mute"));
        sendResponse(200, MIME_TEXT, String(isMuted));
    });
    webServer.on("/power", HTTP_GET, []() {
        handleButton(String("power"));
        sendResponse(200, MIME_TEXT, String(isPowered));
    });
    webServer.on("/save", HTTP_GET, []() {
        handleButton(String("pip"));
        sendResponse(200, MIME_TEXT, String("ok"));
    });
    webServer.on("/reload", HTTP_GET, []() {
        readEffect();
        readSpeeds();
        sendResponse(200, MIME_TEXT, String("ok"));
    });
    webServer.on("/reset", HTTP_GET, []() {
        sendResponse(200, MIME_TEXT, String("ok"));
        delay(2000);
        ESP.restart();
    });
    webServer.on("/broadcast", HTTP_GET, []() {
        String value = webServer.arg("v");
        if(value != nullptr)
            inhibitBroadcast = value.toInt() != 0;
        sendResponse(200, MIME_TEXT, String(inhibitBroadcast));
    });
    webServer.on("/effect", HTTP_GET, []() {
        String value = webServer.arg("v");
        if(value.toInt() >= 0 && value.toInt() < MAX_EFFECTS) {
            setEffect(value.toInt());
            sendResponse(200, MIME_TEXT, value);
        }
        else {
            sendResponse(404, MIME_TEXT, value);
        }
    });
    webServer.on("/devices", HTTP_GET, []() {
        char status[1028];
        char temp[128];
        int devices = MDNS.queryService("yasl", "tcp");
        if(devices > 0) {
            sprintf(status, "[");
            for(int i=0; i< devices; i++) {
                Serial.printf("%s - %s\n",MDNS.hostname(i).c_str(), MDNS.IP(i).toString().c_str());
                sprintf(temp, "{ \"Device\": \"%s\", \"IP\":\"%s\" }", MDNS.hostname(i).c_str(), MDNS.IP(i).toString().c_str());
                if(strlen(status) < 900) {
                    strcat(status, temp);
                    if(i < devices-1)
                        strcat (status, ",");
                }
                else {
                    Serial.printf("Device count overrun in '/devices' ...\n");
                    break;
                }
            }
            strcat(status, "]");
            sendResponse(200, MIME_JSON, String(status));
        }
        else {
            Serial.printf("No other YASL devices found.\n");
            sendResponse(200, MIME_JSON, String("[]"));
        }
    });
    webServer.on("/status", HTTP_GET, []() {
        char status[255];
        sprintf(status, "{ \"Device\": \"%s\", \"Effect\": %d, \"Muted\": %d, \"Powered\": %d, \"Version\": \"%s\", \"IPAddress\":\"%s\", \"Broadcast\": %d }", deviceName, selectedEffect, isMuted, isPowered, VERSION, WiFi.localIP().toString().c_str(), !inhibitBroadcast);
        sendResponse(200, MIME_JSON, String(status));
    });
    webServer.on("/config", HTTP_GET, []() {
        String fx = "[";
        char tmp[80];
        int i;
        for(i=0; i < MAX_EFFECTS; i++) {
            sprintf(tmp, " { \"Index\": %d, \"Name\": \"%s\", \"Params\": %d },", i, allEffects[i].displayName, 0);
            fx += tmp;
        }
        sprintf(tmp, " { \"Index\": %d, \"Name\": \"null\", \"Params\": %d }", i, 0);
        fx += tmp;
        fx += "]";
        sendResponse(200, MIME_JSON, fx);
    });
    webServer.on("/button", HTTP_GET, []() {
        String value = webServer.arg("v");
        broadcastButton(value);
        handleButton(value);
        sendResponse(200, MIME_TEXT, value);
    });
    #if defined(USE_FS)
    webServer.serveStatic("/", LittleFS, "/www/");
    #endif

    if(MDNS.begin(deviceName)) {
        MDNS.setHostname(deviceName);
        MDNS.addService("yasl", "tcp", 80);
        Serial.printf("MDNS set to: %s\n", deviceName);
    }
    if(NBNS.begin(deviceName)) {
        Serial.printf("NetBios set to: %s\n", deviceName);
    }
    webServer.onNotFound(handle404);
    webServer.keepAlive(true);
    webServer.begin();
    Serial.println("WebServer running...");
    udp.beginMulticast(WiFi.localIP(), broadcast, UDP_PORT);
}

void broadcastEffect(int effect) {
    if(inhibitBroadcast)
        return;
    if(isBroadcasting) {
        isBroadcasting = false;
        return;
    }
    char tmp[MAX_UDP_PACKET];
    sprintf(tmp,"YASL:FX:%d", effect);
    udp.beginPacketMulticast(broadcast, UDP_PORT, WiFi.localIP());
    udp.write(tmp);
    udp.endPacket();
}

void broadcastButton(String btn) {
    if(inhibitBroadcast)
        return;
    char tmp[MAX_UDP_PACKET];
    sprintf(tmp,"YASL:BTN:%s", btn.c_str());
    udp.beginPacketMulticast(broadcast, UDP_PORT, WiFi.localIP());
    udp.write(tmp);
    udp.endPacket();
    if(btn == "left" || btn == "right") {
        broadcastSpeed();
    }
}

void broadcastSpeed() {
    if(inhibitBroadcast)
        return;
    int speed = -1;
    switch(selectedEffect) {
        case E_COLORWIPE:
          speed = wipeSpeed;
          break;

        case E_FADE:
          speed = fadeSpeed;
          break;

        case E_TWINKLE:
        case E_TWINKLE2:
          speed = twinkleSpeed;
          break;

        case E_RIPPLE:
        case E_RIPPLE2:
          speed = rippleSpeed;
          break;

        case E_BLENDWAVE:
          speed = blendSpeed;
          break;

        case E_THEATERCASE:
        case E_THEATERCASE2:
          speed = theaterCaseSpeed;
          break;

        case E_METEORRAIN:
        case E_METEORRAIN2:
          speed = meteorRainSpeed;
          break;
        
        case E_CYLONBOUNCE:
          speed = cylonSpeed;
          break;

        case E_FIRE: 
          speed = fireCooling;
          break;

        case E_BOUNCINGBALLS: 
          speed = numBalls;
          break;
    }
    if(speed != -1) {
        char tmp[MAX_UDP_PACKET];
        sprintf(tmp,"YASL:SPD:%d", speed);
        udp.beginPacketMulticast(broadcast, UDP_PORT, WiFi.localIP());
        udp.write(tmp);
        udp.endPacket();
    }
}

void loopWebserver() {
    wifiMgr.process();
    webServer.handleClient();
    MDNS.update();
    int pktSize = udp.parsePacket();
    if (pktSize) {
        int len = udp.read(udpPacket, MAX_UDP_PACKET);
        if (len > 0) {
            udpPacket[len] = 0;
        }
        String data = String(udpPacket);
        if(data.startsWith("YASL:FX:")) {
            byte effect = (byte)data.substring(8).toInt();
            if(effect != selectedEffect) {
                isBroadcasting = true;
                setEffect(effect);
            }
        }
        else if(data.startsWith("YASL:BTN:")) {
            String btn = data.substring(9);
            handleButton(btn);
        }
        else if(data.startsWith("YASL:SPD:")) {
            int speed = data.substring(9).toInt();
            handleSpeed(speed);
        }
        else {
            Serial.printf("UDP sent: %s", udpPacket);
        }
    }
}
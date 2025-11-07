#pragma once
#include <WiFi.h>

struct _lv_obj_t;
typedef struct _lv_obj_t lv_obj_t;


class WiFiHandler {
private:
    const char* ssid;
    const char* password;
    unsigned long timeoutMs;
    lv_obj_t* wifi_status_icon;

public:
    WiFiHandler(const char* ssid, const char* password, unsigned long timeoutMs = 10000);
    bool connect();
    bool isConnected() const;

    void createWiFiStatusIcon();
    void UpdateWiFiStatusIcon();

};
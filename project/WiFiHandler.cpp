#include "WiFiHandler.h"
#include <lvgl.h>

WiFiHandler::WiFiHandler(const char* ssid, const char* password, unsigned long timeoutMs)
    : ssid(ssid), password(password), timeoutMs(timeoutMs){}

bool WiFiHandler::connect() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true);
    WiFi.begin(ssid,password);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < timeoutMs) {
        delay(5);
    }
    return WiFi.status() == WL_CONNECTED;

}

bool WiFiHandler::isConnected() const{
    return WiFi.status() == WL_CONNECTED;
}

void WiFiHandler::createWiFiStatusIcon(){
    wifi_status_icon = lv_label_create(lv_scr_act());
    lv_label_set_text(wifi_status_icon, LV_SYMBOL_WIFI);
    lv_obj_align(wifi_status_icon, LV_ALIGN_TOP_RIGHT, -8,8);

}

void WiFiHandler::UpdateWiFiStatusIcon(){
    if (!wifi_status_icon) return;

    if (WiFi.status() == WL_CONNECTED)
        lv_obj_set_style_text_color(wifi_status_icon, lv_color_hex(0x00FF00), 0);
        
    else
        lv_obj_set_style_text_color(wifi_status_icon, lv_color_hex(0xFF0000), 0);
}
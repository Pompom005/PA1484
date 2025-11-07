#pragma once
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <string>

bool httpsGetJson(const String& url, DynamicJsonDocument& doc); //hämtar json document


#pragma once
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <string>
#include <unordered_map>

const String SMHI_ENTRY_JSON        = "https://opendata-download-metobs.smhi.se/api.json";
const String SMHI_VERSION_LATEST_JS = "https://opendata-download-metobs.smhi.se/api/version/latest.json";


//New work
String buildURL(std::string parameter, std::string station, bool latest = true);

bool httpsGetCSV(const String& url, String& csvData);

//Old work

bool httpsGetJson(const String& url, JsonDocument& doc); //hämtar json document

bool getParameter(String parameter, JsonDocument& doc, const String url = SMHI_VERSION_LATEST_JS);

bool getCity(String City, JsonDocument& doc);

struct SmhiObs;
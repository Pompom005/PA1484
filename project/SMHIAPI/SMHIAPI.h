#pragma once
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <unordered_map>
#include <string>
#include "SMHIStationsAndParameters/SMHIParameter.h"
#include "SMHIStationsAndParameters/SMHIStation.h"

const String SMHI_ENTRY_JSON        = "https://opendata-download-metobs.smhi.se/api.json";
const String SMHI_VERSION_LATEST_JS = "https://opendata-download-metobs.smhi.se/api/version/latest.json";


//New work
WiFiClientSecure* getSSLClient();

bool httpsGetJson(const String& url, JsonDocument& doc);

bool httpsGetCSV(const String& url, String& csvData);

bool buildURL(JsonDocument doc, const SMHIParameter& parameter, const SMHIStation& station, bool latest);


//Old work

bool httpsGetJson(const String& url, JsonDocument& doc); //hämtar json document

bool getParameter(String parameter, JsonDocument& doc, const String url = SMHI_VERSION_LATEST_JS);

bool getCity(String City, JsonDocument& doc);

struct SmhiObs;
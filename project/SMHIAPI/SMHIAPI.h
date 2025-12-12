#pragma once
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <unordered_map>
#include <string>
#include "SMHIStationsAndParameters/SMHIParameter.h"
#include "SMHIStationsAndParameters/SMHIStation.h"
#include "LV_Helper.h"

const String SMHI_ENTRY_JSON        = "https://opendata-download-metobs.smhi.se/api.json";
const String SMHI_VERSION_LATEST_JS = "https://opendata-download-metobs.smhi.se/api/version/latest.json";


//New work
WiFiClientSecure* getSSLClient();

bool https_get_json(const String& url, JsonDocument& doc); //Parses url into jsondoc

bool build_url(JsonDocument& doc, const SMHIParameter& parameter, const SMHIStation& station, bool latest); //Parses url into jsondoc, but builds url from station and parameter

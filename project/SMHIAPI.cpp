#include "SMHIAPI.h"

//Struct för data hantering
struct SmhiObs {
  bool ok;
  float value;
  String unit;
  String station;
  String timestampUTC;
};

const String SMHI_ENTRY_JSON        = "https://opendata-download-metobs.smhi.se/api.json";
const String SMHI_VERSION_LATEST_JS = "https://opendata-download-metobs.smhi.se/api/version/latest.json";

// Generic HTTPS → JSON helper (ESP32)
bool httpsGetJson(const String& url, JsonDocument& doc) {

  WiFiClientSecure client;
  client.setInsecure();  // simplify TLS on ESP32
  HTTPClient http;
  if (!http.begin(client, url)) {
    return false;
  }

  const int code = http.GET();
  if (code != HTTP_CODE_OK) {
    http.end();
    return false;
  }

  const String payload = http.getString();
  http.end();

  DeserializationError err = deserializeJson(doc, payload);
  if (err) {
    return false;
  }

  return true;
}

JsonDocument getData(String& parameter, String& city, String url = SMHI_VERSION_LATEST_JS, JsonDocument& doc) {

  doc = httpsGetJson(url, doc);

  for (JsonObject obj: doc["resource"].as<JsonArray>()) {
    String title = String(obj["title"].as<String>()); 
    title.toLowerCase();
    if (title == parameter) {
      //försöker få den att hitta fram till rätt parameter i steg 2, steg 1 är entry point
    }
  }
  return doc;
}


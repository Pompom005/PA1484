#include "SMHIAPI.h"

//Struct för data hantering
struct SmhiObs {
  bool ok;
  float value;
  String unit;
  String station;
  String timestampUTC;
};

// Generic HTTPS → JSON helper (ESP32)
bool httpsGetJson(const String& url, JsonDocument& doc) {

  WiFiClientSecure client;
  client.setInsecure();  // simplify TLS on ESP32
  HTTPClient http;
  if (!http.begin(client, url)) {
    Serial.println("httpsGetJson failed connection to http.");
    return false;
  }

  const int code = http.GET();
  Serial.printf("\nGet command returned: %d", code);
  if (code != HTTP_CODE_OK) { // HTTP_CODE_OK = 200
    http.end();
    return false;
  }

  const String payload = http.getString();
  http.end();

  DeserializationError err = deserializeJson(doc, payload);
  if (err) {
    Serial.printf("Deserialise json returned: %s", err.c_str());
    return false;
  }

  return true;
}

bool getparameter(String parameter, JsonDocument& doc, const String url) {

  httpsGetJson(url, doc);

  for (JsonObject obj: doc["resource"].as<JsonArray>()) {
    String title = String(obj["title"].as<String>()); 
    title.toLowerCase();
    if (title == parameter) {
      //försöker få den att hitta fram till rätt parameter i steg 2, steg 1 är entry point. Fortsätter imorgon
      Serial.printf("\nFound parameter: %s",parameter.c_str()); //felsökning

      for (JsonObject param: obj["link"].as<JsonArray>()) {
        if (param["type"].as<String>().indexOf("/json")) {
          String link = param["href"].as<String>();

          Serial.printf("\nFound link %s", link.c_str());

          bool res = httpsGetJson(link, doc);
          if (res) {
            Serial.println("returned json successfully");
            return true;
          }else {
            Serial.println("httpsgetjson failed.");
            return false;
          }
        }
      }
    }
    else {
      Serial.printf("\nCouldn't find %s", parameter.c_str());
    }
  }
  Serial.println("getparameter gör inget.");
  return false;
}




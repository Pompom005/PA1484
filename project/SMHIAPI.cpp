

//Definierar SmhiObs structen här, problem annars

struct SmhiObs {
  bool ok;
  float value;
  String unit;
  String station;
  String timestampUTC;
};
// SMHI entry points (metobs)
static const char* SMHI_ENTRY_JSON        = "https://opendata-download-metobs.smhi.se/api.json";
// om du vill kan du byta 'latest' till '1.0' här för att testa:
// static const char* SMHI_VERSION_LATEST_JS = "https://opendata-download-metobs.smhi.se/api/version/1.0.json";
static const char* SMHI_VERSION_LATEST_JS = "https://opendata-download-metobs.smhi.se/api/version/latest.json";

// Generic HTTPS → JSON helper (ESP32)
static bool httpsGetJson(const String& url, DynamicJsonDocument& doc) {

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
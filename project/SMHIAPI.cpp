#include "SMHIAPI.h"

WiFiClientSecure* getSSLClient() {
    static WiFiClientSecure client;
    static bool initialized = false;
    
    if (!initialized) {
        client.setInsecure();  // BARA en gång
        client.setTimeout(15000);
        initialized = true;
    }
    return &client;
}

bool httpsGetJson(const String& url, JsonDocument& doc) {

  WiFiClientSecure* client = getSSLClient();
  HTTPClient http;
  if (!http.begin(*client, url)) {
    Serial.println("httpsGetJson http.begin() failed");
    return false;
  }

  Serial.println("\nCalling http.get():");
  const int code = http.GET();
  Serial.printf("\nGet command returned: %d\n", code);
  if (code != HTTP_CODE_OK) { // HTTP_CODE_OK = 200
    http.end();
    return false;
  }

  int contentLength = http.getSize();
  Serial.printf("Content-Length: %d bytes (%.1f KB)\n", contentLength, contentLength / 1024.0);

  const String payload = http.getString();
  Serial.printf("Actual payload size: %d bytes (%.1f KB)\n", payload.length(), payload.length() / 1024.0);

  http.end();

  Serial.printf("Memory after receiving data: %d\n", ESP.getFreeHeap());

  doc.clear();
  DeserializationError err = deserializeJson(doc, payload);
  if (err) {
    Serial.printf("\nDeserialise json returned: %s", err.c_str());
    return false;
  }

  http.end();
  return true;
}

bool buildURL(JsonDocument& doc, const SMHIParameter& parameter, SMHIStation& station, bool latest) {

    /*std::unordered_map<std::string, std::string> parameterMap = {
    {"lufttemperatur", "22"},
    {"luftfuktighet", "6"}, 
    {"vindhastighet", "4"}, 
    {"max av medelvindhastighet", "25"}, 
    {"lufttryck", "9"}, //neråt funkar inte
    {"nederbördsmängd", "27"},
    {"snödjup", "28"},
    {"sikt", "29"},
    {"molntäckning", "30"},
    {"solskenstid", "31"},
    {"globalstrålning", "32"}
    };
    //temperatur, humidity, windspeed

    std::unordered_map<std::string, std::string> cityMap = {
    {"karlskrona", "65090"},    // Karlskrona-Söderstjärna
    {"lund", "53430"},          // Lund
    {"malmö", "53360"},         // Malmö A
    {"stockholm", "09740"},     // Stockholm //här och neråt funkar inte
    {"göteborg", "07160"},      // Göteborg
    {"uppsala", "09760"}        // Uppsala
    };*/

    std::string parameterKey = std::to_string(parameter.smhiParameterkey);
    std::string stationKey = std::to_string(station.key);
    std::string URL = "https://opendata-download-metobs.smhi.se/api/version/latest/parameter/" 
                  + parameterKey + "/station/" 
                  + stationKey + "/period/";

    if (latest) 
      URL += "latest-months/data.json";
    else 
      URL += "corrected-archive/data.json";

    if (httpsGetJson(URL.c_str(), doc))
      return true;
    else 
      return false;
}



bool httpsGetCSV(const String& url, String& csvData) {
  WiFiClientSecure* client = getSSLClient();
  HTTPClient http;
  
  if (!http.begin(*client, url)) {
    Serial.println("httpsGetCSV http.begin() failed");
    return false;
  }

  const int code = http.GET();
  if (code != HTTP_CODE_OK) {
    http.end();
    return false;
  }

  csvData = http.getString();
  http.end();
  return true;
}



//////////////////////////////////////////
/*bool setupStream(const String& url) {
  // Close any existing stream first
  Serial.println("\nExecuting setupStream.");
  closeStream();
  WiFiClientSecure* currentClient = nullptr;
  HTTPClient currentHttp;
  bool streamActive = false;
  Serial.println("Declared variables.");

  currentClient = getSSLClient();
  if (!currentHttp.begin(*currentClient, url)) {
    currentClient = nullptr;
    return false;
  }
  
  int code = currentHttp.GET();
  if (code != HTTP_CODE_OK) {
    currentHttp.end();
    currentClient = nullptr;
    return false;
  }
  
  streamActive = true;
  return true;
}*/

// Function to close the current stream
/*void closeStream() {
  if (streamActive) {
    currentHttp.end();
    currentClient = nullptr;
    streamActive = false;
  }
}*/

// Function to get the current stream
/*WiFiClient* getCurrentStream() {
  return streamActive ? currentHttp.getStreamPtr() : nullptr;
}*/

//////////////////////////////
// Generic HTTPS → JSON helper (ESP32)


bool getParameter(String parameter, JsonDocument& doc, const String url) {
  doc.clear();
  httpsGetJson(url, doc);

  for (JsonObject obj: doc["resource"].as<JsonArray>()) {
    String title = String(obj["title"].as<String>()); 
    title.toLowerCase();

    if (title == parameter) {
      Serial.printf("\nFound parameter: %s",parameter.c_str()); //felsökning

      for (JsonObject param: obj["link"].as<JsonArray>()) {
        String type = param["type"].as<String>();

        if (type.indexOf("application/json") == 0) {
          String link = param["href"].as<String>();

          Serial.printf("\nFound link to parameter: %s", link.c_str());

          doc.clear();
          bool res = httpsGetJson(link, doc);

          if (res) {
            Serial.println("\nGetParameter returned json successfully");
            return true;
          }else {
            Serial.println("\nGetParameter failed getting json.");
            return false;
          }
        }
      }
    }
  }
  Serial.println("getParameter failed.");
  return false;
}

bool getCity(String city, JsonDocument& doc) {
  //doc needs to be sent as parameter after sending it to getParameter
  
  if (doc.isNull()) {
  Serial.println("No data in document for getCity");
  return false;
  }
  
  for (JsonObject obj : doc["station"].as<JsonArray>()) {
    String name = obj["name"].as<String>();
    name.toLowerCase();

    if (name.indexOf(city) >= 0) {

      Serial.printf("\nFound city: %s", name.c_str());

      for (JsonObject obj1 : obj["link"].as<JsonArray>()) {
        String link = obj1["href"].as<String>();
        Serial.printf("\nTook first link: %s\n", link.c_str());
        Serial.print("Free heap before problem: ");
        Serial.println(ESP.getFreeHeap());
        Serial.printf("Link calling get on: %s", link.c_str());
        doc.clear();
        bool res = httpsGetJson(link, doc);

        if (res) {
          Serial.printf("\nGot json for city: %s", city.c_str());
          return true;
        } else {
          Serial.printf("\nhttpsGetJson in getCity failed. City was %s", name.c_str());
          return false;
        }
      }
    }
  }
      
  Serial.printf("\ncouldn't find city: %s", city.c_str());
  Serial.println("\ngetCity function call failed.");
  return false;
}
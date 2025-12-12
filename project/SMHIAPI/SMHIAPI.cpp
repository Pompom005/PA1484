#include "SMHIAPI.h"
#include <sstream>
#include <queue>

WiFiClientSecure* getSSLClient() 
{
    static WiFiClientSecure client;
    static bool initialized = false;
    
    if (!initialized)
     {
        client.setInsecure();  // BARA en gång
        client.setTimeout(15000);
        initialized = true;
    }
    return &client;
}

bool https_get_json(const String& url, JsonDocument& doc) 
{
  Serial.println(url);
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
  
  while(!client->available())
  {

  }

  std::stringstream stream;
  int fileSize = http.getSize();
  if(fileSize != -1)
  {
    Serial.println(http.getSize());
    Serial.println("Read size\n");
  }
  while(client->available() || client->connected())
  {
    if(client->available())
    {
      if(fileSize != -1) //This means that it is one message and not chunked
      {
        //Just read as normal.
        char data = client->read();
        stream << data;
        //Serial.print(data);
      }
      else //Chunked
      {
        std::string line = client->readStringUntil('\r').c_str(); //'\r' is the "end" symbol 
        int packageSizeInBytes = std::stoi(line.c_str(), nullptr, 16); //16 bc hex
        Serial.println(packageSizeInBytes);
        Serial.println("Read size\n");
        if(packageSizeInBytes > 0) //Means there is a package after this first hex message
        {
          client->read(); //Reads the '\n' at the end of "end" of bytes, if not this could cause problems
          std::string dataLine = client->readStringUntil('\r').c_str(); //'\r' also marks the end of the package
          stream << dataLine;
          //Serial.print(dataLine.c_str());
        }
        else if(packageSizeInBytes == 0) //A message with 0 bits is the "end" message for the whole package
        {
          //done
          break;
        }
      }
    }
    else if(fileSize != -1)
    {
        //Get current size of stream
        int size = stream.str().length() * sizeof(char);
        if(size == fileSize)
        {
          break;
        }
    }
  }

  http.end();
  doc.clear();
  DeserializationError err = deserializeJson(doc, stream.str().c_str());
  if (err) {
    Serial.printf("\nDeserialise json returned: %s", err.c_str());
    return false;
  }
  return true;
}

bool build_url(JsonDocument& doc, const SMHIParameter& parameter, const SMHIStation& station, bool latest) {

    std::string parameterKey = std::to_string(parameter.smhi_parameter_key);
    std::string stationKey = std::to_string(station.key);
    std::string URL = "https://opendata-download-metobs.smhi.se/api/version/latest/parameter/" 
                  + parameterKey + "/station/" 
                  + stationKey + "/period/";

    if (latest) 
      URL += "latest-months/data.json";
    else 
      URL += "corrected-archive/data.json";

    if (https_get_json(URL.c_str(), doc))
      return true;
    else 
      return false;
}


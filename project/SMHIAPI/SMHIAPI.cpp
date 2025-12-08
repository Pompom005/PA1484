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

bool httpsGetForecastJson(const String& url, JsonDocument& doc) 
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

  std::stringstream currentLine;
  std::stringstream stream;
  bool foundQuoteMark = false;
  while(client->available() || client->connected())
  {
    //Maybe check the data read vs http.getSize,
    //Could get real size eventually maybe?
    if(client->available())
    {
      char c = (char)(client->read());
      if(c == '"')
      {
        foundQuoteMark = true;
      }

        currentLine << c;
        if(c == '\n') //End of line
        { 
          if(foundQuoteMark)//Check to skip chunk stuff
          {
            //Serial.print(currentLine.str().c_str());
            stream << currentLine.str();
            foundQuoteMark = false;
          }
          currentLine.str(std::string()); //We clear the line, and chunkvalue is removed 
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

bool httpsGetData(const String &url, std::vector<lv_coord_t> &values)
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

  //Find first "value", skip it

  //Find "value"
  //Find next ""
  //Grab number inside that ""
  //Add to vector
  //Repeat

  std::stringstream currentline;
  
  enum class ReadState
  {
    SkipFirst,
    FindQuotemark,
    FindValue,
    GetNumber
  };

  std::queue<ReadState> stateQueue;
  stateQueue.push(ReadState::FindQuotemark);
  stateQueue.push(ReadState::SkipFirst);

  while(client->available() || client->connected())
  {
    //Maybe check the data read vs http.getSize,
    //Could get real size eventually maybe?
    if(client->available())
    {
      char c = (char)(client->read());
      ReadState state = stateQueue.front();

      switch(state)
      {
        case ReadState::SkipFirst:
        {      
          currentline << c;
          std::string str = currentline.str();
          std::string target = "value";
          bool matches = true;

          for(int i = 0; i < str.length(); i++)
          {
            if(str[i] != target[i])
            {
              matches = false;
            }
          }

          if(!matches)
          {
            currentline.str(std::string()); //Clear
            stateQueue.pop();
            stateQueue.push(ReadState::FindQuotemark); //Double because there's always going to be one at the end of this current word
            stateQueue.push(ReadState::FindQuotemark);
            stateQueue.push(ReadState::SkipFirst);
          }
          else if(str.length() == target.length()) //Full match
          {
            currentline.str(std::string()); //Clear
            stateQueue.pop();
            stateQueue.push(ReadState::FindQuotemark);
            stateQueue.push(ReadState::FindQuotemark);
            stateQueue.push(ReadState::FindValue);
          }
          break;
        }
        case ReadState::FindQuotemark:
        {
          if(c == '"')
          {
            stateQueue.pop();
          }
          break;
        }
        case ReadState::FindValue:
        {          
          currentline << c;
          std::string str = currentline.str();
          std::string target = "value";
          bool matches = true;

          for(int i = 0; i < str.length(); i++)
          {
            if(str[i] != target[i])
            {
              matches = false;
            }
          }

          if(!matches)
          {
            currentline.str(std::string()); //Clear
            stateQueue.pop();
            stateQueue.push(ReadState::FindQuotemark);
            stateQueue.push(ReadState::FindQuotemark);
            stateQueue.push(ReadState::FindValue);
          }
          else if(str.length() == target.length()) //Full match
          {
            currentline.str(std::string()); //Clear
            stateQueue.pop();
            stateQueue.push(ReadState::FindQuotemark);
            stateQueue.push(ReadState::FindQuotemark);
            stateQueue.push(ReadState::GetNumber);
          }
          break;
        }
        case ReadState::GetNumber:
        {
          if(c == '"') //Should be end of number
          {
            //Serial.print(std::stof(currentline.str()));
            //Serial.print("  ");
            values.push_back(std::stof(currentline.str()));
            currentline.str(std::string()); //Clear

            if(values.size() % 25 == 0)
            {
              Serial.println(" ");
            }

            stateQueue.pop();
            stateQueue.push(ReadState::FindQuotemark);
            stateQueue.push(ReadState::FindValue);
          }
          else
          {
            currentline << c;
          }
          break;
        }
      }
    }
  }
  http.end();
  return values.size() > 0;
}

bool buildURL(std::vector<lv_coord_t>& values, const SMHIParameter& parameter, const SMHIStation& station, bool latest) {

    std::string parameterKey = std::to_string(parameter.smhiParameterkey);
    std::string stationKey = std::to_string(station.key);
    std::string URL = "https://opendata-download-metobs.smhi.se/api/version/latest/parameter/" 
                  + parameterKey + "/station/" 
                  + stationKey + "/period/";

    if (latest) 
      URL += "latest-months/data.json";
    else 
      URL += "corrected-archive/data.json";

    if (httpsGetData(URL.c_str(), values))
      return true;
    else 
      return false;
}


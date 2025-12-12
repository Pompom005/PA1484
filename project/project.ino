#include <TFT_eSPI.h>
#include <time.h>
#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>
#include <lvgl.h>
#include "Dropdown.h"
#include "BootScreen.h"
#include <sstream>
#include <iostream> 
#include <string>
#include <vector>
#include "WeatherForecastElement.h"
#include "WiFiHandler.h"
#include "Linegraf.h"
#include "SettingsScreen.h"
#include "SMHIStationsAndParameters/SMHIStationsAndParameters.h"
#include "SMHIAPI/SMHIAPI.h"

// test system includes
#include "ParserStuff/SMHIFileManager.h"
#include "lib/SMHITestdata/SMHITestRunner.h"

using namespace std;

// Wi-Fi credentials
WiFiHandler wifi("WiFi name","WiFi password", 15000);

LilyGo_Class amoled;

static lv_obj_t* tileview;

static lv_obj_t* t0;
static lv_obj_t* t1;
static lv_obj_t* t2;

static lv_obj_t* t0_text1;
static lv_obj_t* t0_text2;
static lv_obj_t* t0_label;
//OUR variables

static std::vector<WeatherForecastElement*> forecast_elements;
static lv_obj_t* grafobj;
static SettingsScreen* settings;
static Linegraph* mygrafobj;
static lv_obj_t* forecast_parent;

static lv_obj_t* graphTitle;
static lv_obj_t* graphDescription;

BootScreen boot;
bool bootDone = false;

static SupportedParameter selectedParam;
static int stationKey;

//END of our variables

static void OnTileChanged(_lv_event_t* event)
{
  if(event->code == LV_EVENT_VALUE_CHANGED)
  {
    if(lv_tileview_get_tile_act(tileview) == t0)
    {
      settings->hide_on_tiles();
    }
    else
    {
      settings->show_on_tiles();
    }
  }
}

// Function: Creates UI
static void create_ui()
{
  // Fullscreen Tileview
  tileview = lv_tileview_create(lv_scr_act());
  lv_obj_set_size(tileview, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
  lv_obj_set_scrollbar_mode(tileview, LV_SCROLLBAR_MODE_OFF);
  lv_obj_add_event_cb(tileview, OnTileChanged, LV_EVENT_VALUE_CHANGED, 0);

  // Add two horizontal tiles
  t0 = lv_tileview_add_tile(tileview, 0, 0, LV_DIR_HOR);
  t1 = lv_tileview_add_tile(tileview, 1, 0, LV_DIR_HOR);
  t2 = lv_tileview_add_tile(tileview, 2, 0, LV_DIR_HOR);

  // Tile #1

  {
    //Creating 7-day screen with example values
    int amount = 7;
    forecast_elements = std::vector<WeatherForecastElement*>();
    forecast_elements.resize(amount);

    forecast_parent = lv_obj_create(t1);
    lv_obj_align(forecast_parent, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_content_height(forecast_parent, lv_obj_get_content_height(t1) * 0.85f);
    lv_obj_set_content_width(forecast_parent, lv_obj_get_content_width(t1) * 0.65f);

    float element_size = 1.0 / ((float)amount);
    for(int i = 0; i < amount; i++)
    {
      forecast_elements[i] = new WeatherForecastElement(forecast_parent); //Even smaller to act as padding
      forecast_elements[i]->set_position(i * 0.60f, 0); //-0.5f because it is centered, meaning left side is -0.5f
    }

    forecast_elements[0]->set_values(25, "Karlskrona", "11-01", WeatherType::Clear);
    forecast_elements[1]->set_values(15, "Karlskrona", "11-02", WeatherType::Thunder);
    forecast_elements[2]->set_values(-10, "Karlskrona", "11-03", WeatherType::Thunderstorm);
    forecast_elements[3]->set_values(-36, "Karlskrona", "11-04", WeatherType::NearlyClear);
    forecast_elements[4]->set_values(13, "Karlskrona", "11-05", WeatherType::LightSleet);
    forecast_elements[5]->set_values(15, "Karlskrona", "11-06", WeatherType::HeavySnowShowers);
    forecast_elements[6]->set_values(12, "Karlskrona", "11-07", WeatherType::LightSnowShowers);
  }

  // Tile #2
  {
    grafobj = lv_obj_create(t2);
    lv_obj_align(grafobj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_content_height(grafobj, lv_obj_get_content_height(t2) * 0.85f);
    lv_obj_set_content_width(grafobj, lv_obj_get_content_width(t2) * 0.65f);

    //Example values in case data not working in case of no internet etc
    vector<lv_coord_t> koord = {30, 10, 50, 40, 20, 24, 85, 74, 26, 45, 56, 78, 90, 65, 98};
    mygrafobj = new Linegraph(grafobj, koord);

    graphTitle = lv_label_create(t2);
    lv_label_set_text(graphTitle, "");
    lv_obj_set_style_text_font(graphTitle, &arial_32, LV_PART_MAIN);
    lv_obj_align(graphTitle,LV_ALIGN_TOP_MID, 0, 95);
    graphDescription = lv_label_create(t2);
    lv_label_set_text(graphDescription, "");
    lv_obj_set_style_text_font(graphDescription, &arial_16, LV_PART_MAIN);
    lv_obj_align(graphDescription,LV_ALIGN_BOTTOM_MID, 0, -5);
  }
  
  {
    //Start tile
    t0_label = lv_label_create(t0);
    lv_label_set_text(t0_label, "Weather app");
    lv_obj_set_style_text_font(t0_label, &arial_32, LV_PART_MAIN);
    lv_obj_align(t0_label,LV_ALIGN_TOP_MID, 0, 95);

    t0_text1 = lv_label_create(t0);
    lv_label_set_text(t0_text1, "v4.0");
    lv_obj_set_style_text_color(t0_text1, lv_color_white(), LV_PART_MAIN);
    lv_obj_align(t0_text1, LV_ALIGN_BOTTOM_LEFT, 10, -30);

    t0_text2 = lv_label_create(t0);
    lv_label_set_text(t0_text2, "Grupp 9");
    lv_obj_set_style_text_color(t0_text2, lv_color_white(), LV_PART_MAIN);
    lv_obj_align(t0_text2, LV_ALIGN_BOTTOM_LEFT, 10, -10);

    lv_obj_set_style_bg_opa(t0, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(t0, lv_color_black(), 0);

    lv_obj_set_style_text_color(t0_label, lv_color_white(), 0);
  }
lv_obj_set_tile_id(tileview, 0, 0, LV_ANIM_ON);

  settings = new SettingsScreen();

  //Adds listener to settings chaning station, updates the forecast screen with data
  settings->add_listener([&](DropdownStation newLocation)
  {
    stationKey = newLocation.real_station->key;
    forecast_elements[0]->set_location(newLocation.real_station->name);
    forecast_elements[1]->set_location(newLocation.real_station->name);
    forecast_elements[2]->set_location(newLocation.real_station->name);
    forecast_elements[3]->set_location(newLocation.real_station->name);
    forecast_elements[4]->set_location(newLocation.real_station->name);
    forecast_elements[5]->set_location(newLocation.real_station->name);
    forecast_elements[6]->set_location(newLocation.real_station->name);
    //At least changes name above here in case getting data fails

  JsonDocument doc;
  std::stringstream jsonStr;
  //Builds url str for forecast data, would be in a function if we had more time and developed sweden map
  jsonStr <<  "https://opendata-download-metfcst.smhi.se/api/category/snow1g/version/1/geotype/point/lon/" << newLocation.real_station->longitude << "/lat/" << newLocation.real_station->latitude << "/data.json";
  bool res = https_get_json(jsonStr.str().c_str(), doc);

  if(res)
  {
    Serial.println("Doc built");

    SMHIForecastParser forecast;
    if(!forecast.get_data_from_json(doc))
    {
      Serial.println("Failed getting data");
      return;
    }
    Serial.println("Fetched data!");

    const std::vector<ForecastDataPoint>& data = forecast.get_all_data();

    for(int i = 0; i < forecast_elements.size(); i++)
    {
      forecast_elements[i]->set_values(data[i].temperature, newLocation.real_station->name, (std::to_string(data[i].month) + "-" + std::to_string(data[i].day)), static_cast<WeatherType>(data[i].weather_symbol));
    }
    Serial.println("Done");
  }

  });

  //Adds listener to settings chaning parameter, updates the graph with values
  settings->add_listener([&](DropdownParameter newParam)
  {
        selectedParam = static_cast<SupportedParameter>(newParam.real_parameter->enum_parameter_key);
        lv_label_set_text(graphTitle, newParam.real_parameter->title.c_str());
        lv_label_set_text(graphDescription, newParam.real_parameter->description.c_str());

        const SMHIParameter& param = SMHIStationsAndParameters::instance().get_parameter(selectedParam);
        std::vector<lv_coord_t> values;

        std::vector<DropdownStation> stations = SMHIStationsAndParameters::instance().get_eligible_stations(selectedParam);
        SMHIStation* station = stations[0].real_station;

        for(int i = 0; i < stations.size(); i++)
        {
          if(stations[i].real_station->key == stationKey)
          {
            station = stations[i].real_station;
            break;
          }
        }
        JsonDocument doc;
        bool res = build_url(doc, param, *station, true);

        if(res)
        {
          SMHIHistoricalParser parser;
          parser.get_data_from_json(doc);
          
          mygrafobj->replace_data(parser.get_value_data(), true);
        }
  });

  //Adds listener to settings chaning station, updates the graph with values
  settings->add_listener([&](DropdownStation newStation)
  {
        const SMHIParameter& param = SMHIStationsAndParameters::instance().get_parameter(selectedParam);
        JsonDocument doc;

        bool res = build_url(doc, param, *newStation.real_station, true);

        if(res)
        {
          SMHIHistoricalParser parser;
          parser.get_data_from_json(doc);
          
          mygrafobj->replace_data(parser.get_value_data(), true);
        }
  });

  settings->hide_on_tiles();
}

// Must have function: Setup is run once on startup
void setup()
{
  Serial.begin(115200);
  delay(200);
  Serial.print("Free heap at start of setup(): ");
  Serial.println(ESP.getFreeHeap());  
  Serial.println(ESP.getFreePsram());


  if (!amoled.begin()) {
    Serial.println("Failed to init LilyGO AMOLED.");
    while (true) delay(1000);
  }

  //Needs to be before UI
  SMHIStationsAndParameters::instance().Init();

  beginLvglHelper(amoled);

  create_ui();

  // bootscreen start here
  // Boot screen sequence
  boot.init();
  boot.show();

  unsigned long start = millis();
  while (millis() - start < 3000) {
    lv_timer_handler();
    delay(5);
  }

  boot.hide();
  bootDone = true;

// initiate wifi here
wifi.create_WiFi_status_icon();
  if (wifi.connect()) {
    Serial.println("connected to WiFi");
  }
  else { 
    Serial.println("Failed to connect to WiFi");
  }

  //ADD ALL THE STUFF TO REACT TO SETTINGS BEFORE THIS. Otherwise default might not apply etc
  settings->load_default_values();
}

// Must have function: Loop runs continously on device after setup
void loop()
{
wifi.update_WiFi_status_icon();

  lv_timer_handler();
}
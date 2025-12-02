//#include "SMHIAPI/SMHIAPI.h"
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
#include "Graph.h"
#include "WeatherForecastElement.h"
#include "WiFiHandler.h"
#include "Linegraf.h"
#include "SettingsScreen.h"
#include "SMHIStationsAndParameters/SMHIStationsAndParameters.h"

// test system includes
#include "ParserStuff/SMHIFileManager.h"
#include "lib/SMHITestdata/SMHITestRunner.h"

using namespace std;

// Wi-Fi credentials (Delete these before commiting to GitHub)
WiFiHandler wifi("BTH_Guest","papaya21turkos", 2000);

LilyGo_Class amoled;

static lv_obj_t* tileview;

static lv_obj_t* t0;
static lv_obj_t* t1;
static lv_obj_t* t2;

static lv_obj_t* t2_label;
static bool t2_dark = false;  // start tile #2 in light mode
static lv_obj_t* t0_text1;
static lv_obj_t* t0_text2;
static lv_obj_t* t0_label;
//OUR variables

static std::vector<WeatherForecastElement*> forecast_elements;
//static Dropdown<string>* dropdownobj;
//static Dropdown <string> * dropdownobj2;
static lv_obj_t* grafobj;
static SettingsScreen* settings;
static Linegraf* mygrafobj;
static lv_obj_t* forecast_parent;

//END of our variables

// Function: Tile #2 Color change
static void apply_tile_colors(lv_obj_t* tile, lv_obj_t* label, bool dark)
{
  // Background
  lv_obj_set_style_bg_opa(tile, LV_OPA_COVER, 0);
  lv_obj_set_style_bg_color(tile, dark ? lv_color_black() : lv_color_white(), 0);

  // Text
  lv_obj_set_style_text_color(label, dark ? lv_color_white() : lv_color_black(), 0);
}

static void OnTileChanged(_lv_event_t* event)
{
  if(event->code == LV_EVENT_VALUE_CHANGED)
  {
    if(lv_tileview_get_tile_act(tileview) == t0)
    {
      settings->HideOnTiles();
    }
    else
    {
      settings->ShowOnTiles();
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
      forecast_elements[i]->SetPosition(i * 0.60f, 0); //-0.5f because it is centered, meaning left side is -0.5f
    }

    forecast_elements[0]->SetValues(25, "Karlskrona", "11-01", WeatherType::Clear);
    forecast_elements[1]->SetValues(15, "Karlskrona", "11-02", WeatherType::Thunder);
    forecast_elements[2]->SetValues(-10, "Karlskrona", "11-03", WeatherType::Thunderstorm);
    forecast_elements[3]->SetValues(-36, "Karlskrona", "11-04", WeatherType::NearlyClear);
    forecast_elements[4]->SetValues(13, "Karlskrona", "11-05", WeatherType::LightSleet);
    forecast_elements[5]->SetValues(15, "Karlskrona", "11-06", WeatherType::HeavySnowShowers);
    forecast_elements[6]->SetValues(12, "Karlskrona", "11-07", WeatherType::LightSnowShowers);
  }

  // Tile #2
  {
    t2_label = lv_label_create(t2);
    lv_label_set_text(t2_label, "");
    lv_obj_set_style_text_font(t2_label, &lv_font_montserrat_28, 0);
    lv_obj_center(t2_label);

    apply_tile_colors(t2, t2_label, /*dark=*/false);
    lv_obj_add_flag(t2, LV_OBJ_FLAG_CLICKABLE);

    grafobj = lv_obj_create(t2);
    lv_obj_align(grafobj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_content_height(grafobj, lv_obj_get_content_height(t2) * 0.85f);
    lv_obj_set_content_width(grafobj, lv_obj_get_content_width(t2) * 0.65f);

    vector<float> koord = {30, 10, 50, 40, 20, 24, 85, 74, 26, 45, 56, 78, 90, 65, 98};
    mygrafobj = new Linegraf(grafobj, koord);
  }
  
  {
    t0_label = lv_label_create(t0);
    lv_label_set_text(t0_label, "Weather app");
    lv_obj_set_style_text_font(t0_label, &lv_font_montserrat_32, LV_PART_MAIN);
    lv_obj_align(t0_label,LV_ALIGN_TOP_MID, 0, 95);

    t0_text1 = lv_label_create(t0);
    lv_label_set_text(t0_text1, "v1.0");
    lv_obj_set_style_text_color(t0_text1, lv_color_white(), LV_PART_MAIN);
    lv_obj_align(t0_text1, LV_ALIGN_BOTTOM_LEFT, 10, -30);

    t0_text2 = lv_label_create(t0);
    lv_label_set_text(t0_text2, "Grupp 9");
    lv_obj_set_style_text_color(t0_text2, lv_color_white(), LV_PART_MAIN);
    lv_obj_align(t0_text2, LV_ALIGN_BOTTOM_LEFT, 10, -10);

    apply_tile_colors(t0, t0_label, /*dark=*/true);
  }
// Sätt start-tile till t0 som ligger i kolumn 0, rad 0 utan animation
//lv_tileview_set_act(tileview, 0, 0, LV_ANIM_OFF);

// Ladda tileview som aktiv skärm så det syns direkt
lv_obj_set_tile_id(tileview, 0, 0, LV_ANIM_ON);

  settings = new SettingsScreen();

  settings->AddListenerToLocation([&](DropdownStation newLocation)
  {
    forecast_elements[0]->SetLocation(newLocation.realStation->name);
    forecast_elements[1]->SetLocation(newLocation.realStation->name);
    forecast_elements[2]->SetLocation(newLocation.realStation->name);
    forecast_elements[3]->SetLocation(newLocation.realStation->name);
    forecast_elements[4]->SetLocation(newLocation.realStation->name);
    forecast_elements[5]->SetLocation(newLocation.realStation->name);
    forecast_elements[6]->SetLocation(newLocation.realStation->name);
  });

  settings->HideOnTiles();

  //ADD ALL THE STUFF TO REACT TO SETTINGS BEFORE THIS. Otherwise default might not apply etc
  settings->LoadDefaultValues();
}


// Must have function: Setup is run once on startup
BootScreen boot;
bool bootDone = false;
//Dropdown <string> *myDropdown;


void setup()
{
  Serial.begin(115200);
  delay(200);
  Serial.print("Free heap at start of setup(): ");
  Serial.println(ESP.getFreeHeap());  


  if (!amoled.begin()) {
    Serial.println("Failed to init LilyGO AMOLED.");
    while (true) delay(1000);
  }

  //Needs to be before UI
  SMHIStationsAndParameters::GetInstance().Init();

  beginLvglHelper(amoled);// bootscreen start here
// Boot screen sequence
  create_ui();
  boot.init();
  boot.show();

  unsigned long start = millis(); // old val 2500 måste ta tiden och bestämma vad 3 sekunder är. // ta bort en nolla 
  while (millis() - start < 3000) {
    lv_timer_handler();
    delay(5);
  }

  boot.hide();
  bootDone = true;

// initiate wifi here
wifi.createWiFiStatusIcon();
  if (wifi.connect()) {
    Serial.println("connected to WiFi");
  }
  else { 
    Serial.println("Failed to connect to WiFi");
  }
//////////////////////////////////////////////////////////////// smhi grejer

  Serial.print("Free heap before test: ");
  Serial.println(ESP.getFreeHeap());

  Serial.println("\n=== Testing CSV Data ===");
  
  
//   String testCSVUrl = buildURL("lufttemperatur", "karlskrona");
  
//   String csvData;
//   if (httpsGetCSV(testCSVUrl, csvData)) {
//     Serial.println("SUCCESS - Got CSV data:");
//     Serial.println(csvData);  // Just print everything
//   } else {
//     Serial.println("FAILED to get CSV data");
//   }

//   Serial.print("Free heap after test: ");
//   Serial.println(ESP.getFreeHeap());
// ////////////////////////////////////////////////////////////////
}

// Must have function: Loop runs continously on device after setup
void loop()
{
wifi.UpdateWiFiStatusIcon();

  lv_timer_handler();
}

//Jag gör en liten ändring så jag kan pusha
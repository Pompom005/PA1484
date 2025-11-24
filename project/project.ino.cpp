# 1 "C:\\Users\\Jebr\\AppData\\Local\\Temp\\tmpc3_k5gpl"
#include <Arduino.h>
# 1 "D:/Progg/PA1484/PA1484/project/project.ino"
#include "SMHIAPI.h"
#include <TFT_eSPI.h>
#include <time.h>
#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>
#include <lvgl.h>
#include <Dropdown.h>
#include "BootScreen.h"
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include "Graph.h"
#include "WeatherForecastElement.h"
#include "WiFiHandler.h"
#include<Dropdown.h>
#include<Linegraf.h>
#include "SettingsScreen.h"
#include "SMHIStationsAndParameters/SMHIStationsAndParameters.h"

using namespace std;


WiFiHandler wifi("OWNIT_5GHz_C1EB51","TP4YG3ANFJ", 2000);

LilyGo_Class amoled;

static lv_obj_t* tileview;

static lv_obj_t* t0;
static lv_obj_t* t1;
static lv_obj_t* t2;

static lv_obj_t* t1_label;
static lv_obj_t* t2_label;
static bool t2_dark = false;
static lv_obj_t* t3_label;
static lv_obj_t* t0_text1;
static lv_obj_t* t0_text2;
static lv_obj_t* t0_label;


static std::vector<WeatherForecastElement*> forecast_elements;


static Linegraf* grafobj;static SettingsScreen* settings;

static lv_obj_t* forecast_parent;
static void apply_tile_colors(lv_obj_t* tile, lv_obj_t* label, bool dark);
static void on_tile2_clicked(lv_event_t* e);
static void create_ui();
void setup();
void loop();
#line 53 "D:/Progg/PA1484/PA1484/project/project.ino"
static void apply_tile_colors(lv_obj_t* tile, lv_obj_t* label, bool dark)
{

  lv_obj_set_style_bg_opa(tile, LV_OPA_COVER, 0);
  lv_obj_set_style_bg_color(tile, dark ? lv_color_black() : lv_color_white(), 0);


  lv_obj_set_style_text_color(label, dark ? lv_color_white() : lv_color_black(), 0);
}

static void on_tile2_clicked(lv_event_t* e)
{
  LV_UNUSED(e);
  t2_dark = !t2_dark;
  apply_tile_colors(t2, t2_label, t2_dark);
}


static void create_ui()
{

  tileview = lv_tileview_create(lv_scr_act());
  lv_obj_set_size(tileview, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
  lv_obj_set_scrollbar_mode(tileview, LV_SCROLLBAR_MODE_OFF);


  t0 = lv_tileview_add_tile(tileview, 0, 0, LV_DIR_HOR);
  t1 = lv_tileview_add_tile(tileview, 1, 0, LV_DIR_HOR);
  t2 = lv_tileview_add_tile(tileview, 2, 0, LV_DIR_HOR);



  {

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
      forecast_elements[i] = new WeatherForecastElement(forecast_parent);
      forecast_elements[i]->SetPosition(i * 0.60f, 0);
    }

    forecast_elements[0]->SetValues(25, "Karlskrona", "11-01", WeatherType::Sunny);
    forecast_elements[1]->SetValues(15, "Karlskrona", "11-02", WeatherType::Thunder);
    forecast_elements[2]->SetValues(-10, "Karlskrona", "11-03", WeatherType::Snow);
    forecast_elements[3]->SetValues(-36, "Karlskrona", "11-04", WeatherType::Snow);
    forecast_elements[4]->SetValues(13, "Karlskrona", "11-05", WeatherType::Rain);
    forecast_elements[5]->SetValues(15, "Karlskrona", "11-06", WeatherType::Thunder);
    forecast_elements[6]->SetValues(12, "Karlskrona", "11-07", WeatherType::Cloudy);
  }


  {
    t2_label = lv_label_create(t2);
    lv_label_set_text(t2_label, "");
    lv_obj_set_style_text_font(t2_label, &lv_font_montserrat_28, 0);
    lv_obj_center(t2_label);

    apply_tile_colors(t2, t2_label, false);
    lv_obj_add_flag(t2, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(t2, on_tile2_clicked, LV_EVENT_CLICKED, NULL);
    vector<float> koord = {30, 10, 50, 40, 20};
    grafobj = new Linegraf(t2, koord, "Lund");
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

    apply_tile_colors(t0, t0_label, true);
  }




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
}



BootScreen boot;
bool bootDone = false;



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


  SMHIStationsAndParameters::GetInstance().Init();

  beginLvglHelper(amoled);

  create_ui();
  boot.init();
  boot.show();

  unsigned long start = millis();
  while (millis() - start < 3000) {
    lv_timer_handler();
    delay(5);
  }

  boot.hide();
  bootDone = true;


wifi.createWiFiStatusIcon();
  if (wifi.connect()) {
    Serial.println("connected to WiFi");
  }
  else {
    Serial.println("Failed to connect to WiFi");
  }


  Serial.print("Free heap before test: ");
  Serial.println(ESP.getFreeHeap());

  Serial.println("\n=== Testing CSV Data ===");

  String testCSVUrl = buildURL("lufttemperatur", "karlskrona");

  String csvData;
  if (httpsGetCSV(testCSVUrl, csvData)) {
    Serial.println("SUCCESS - Got CSV data:");
    Serial.println(csvData);
  } else {
    Serial.println("FAILED to get CSV data");
  }

  Serial.print("Free heap after test: ");
  Serial.println(ESP.getFreeHeap());

}


void loop()
{
wifi.UpdateWiFiStatusIcon();

  lv_timer_handler();
}
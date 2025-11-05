# 1 "C:\\Users\\hanne\\AppData\\Local\\Temp\\tmpso6pyutn"
#include <Arduino.h>
# 1 "C:/Users/hanne/OneDrive/Dokument/GitHub/PA1484/project/project.ino"
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <time.h>
#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>
#include <lvgl.h>
#include "BootScreen.h"
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include "Graph.h"
#include "WeatherForecastElement.h"
using namespace std;


static const char* WIFI_SSID = "SSID";
static const char* WIFI_PASSWORD = "PWD";

LilyGo_Class amoled;

static lv_obj_t* tileview;
static lv_obj_t* t1;
static lv_obj_t* t2;
static lv_obj_t* t3;
static lv_obj_t* t1_label;
static lv_obj_t* t2_label;
static bool t2_dark = false;
static lv_obj_t* t3_label;



static std::vector<WeatherForecastElement*> forecast_elements;
static void apply_tile_colors(lv_obj_t* tile, lv_obj_t* label, bool dark);
static void on_tile2_clicked(lv_event_t* e);
static void create_ui();
static void connect_wifi();
void setup();
void loop();
#line 41 "C:/Users/hanne/OneDrive/Dokument/GitHub/PA1484/project/project.ino"
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


  t1 = lv_tileview_add_tile(tileview, 0, 0, LV_DIR_HOR);
  t2 = lv_tileview_add_tile(tileview, 1, 0, LV_DIR_HOR);
  t3 = lv_tileview_add_tile(tileview, 2, 0, LV_DIR_HOR);


  {

    int amount = 7;
    forecast_elements = std::vector<WeatherForecastElement*>();
    forecast_elements.resize(amount);

    float element_size = 1.0 / ((float)amount);
    for(int i = 0; i < amount; i++)
    {
      forecast_elements[i] = new WeatherForecastElement(t1);
      forecast_elements[i]->SetPosition(i * 0.60f, 0);
    }

    forecast_elements[0]->SetValues(49, "Karlskrona", "11-01", WeatherType::Sunny);
    forecast_elements[1]->SetValues(11, "Karlskrona", "11-02", WeatherType::Thunder);
    forecast_elements[2]->SetValues(99, "Karlskrona", "11-03", WeatherType::Snow);
    forecast_elements[3]->SetValues(-36, "Karlskrona", "11-04", WeatherType::Snow);
    forecast_elements[4]->SetValues(13, "Karlskrona", "11-05", WeatherType::Rain);
    forecast_elements[5]->SetValues(15, "Karlskrona", "11-06", WeatherType::Thunder);
    forecast_elements[6]->SetValues(12, "Karlskrona", "11-07", WeatherType::Cloudy);
  }


  {
    t2_label = lv_label_create(t2);
    lv_label_set_text(t2_label, "Welcome to the jungle");
    lv_obj_set_style_text_font(t2_label, &lv_font_montserrat_28, 0);
    lv_obj_center(t2_label);

    apply_tile_colors(t2, t2_label, false);
    lv_obj_add_flag(t2, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(t2, on_tile2_clicked, LV_EVENT_CLICKED, NULL);
  }


  {

  }
}


static void connect_wifi()
{
  Serial.printf("Connecting to WiFi SSID: %s\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  const uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - start) < 15000) {
    delay(250);
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi connected.");
  } else {
    Serial.println("WiFi could not connect (timeout).");
  }
}


BootScreen boot;
bool bootDone = false;



void setup()
{
  Serial.begin(115200);
  delay(200);


  if (!amoled.begin()) {
    Serial.println("Failed to init LilyGO AMOLED.");
    while (true) delay(1000);
  }

  beginLvglHelper(amoled);

boot.init();
  boot.show();

  unsigned long start = millis();
  while (millis() - start < 3000) {
    lv_timer_handler();
    delay(5);
  }

  boot.hide();
  bootDone = true;


  create_ui();
  connect_wifi();
}


void loop()
{
  lv_timer_handler();
}
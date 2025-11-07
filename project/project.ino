#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <time.h>
#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>
#include <lvgl.h>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ===========================
// --- SMHI ADDITIONS ---
// ===========================
#include <WiFiClientSecure.h>

//Definierar SmhiObs structen här
struct SmhiObs {
  bool ok;
  float value;
  String unit;
  String station;
  String timestampUTC;
};

// Vi struntar i att hämta “latest version json” och att leta upp parameter via namn.
// Lufttemperatur i SMHI metobs = parameter 1.
static const int SMHI_PARAM_LUFTTEMP = 1;

// Generic HTTPS → JSON helper (ESP32)
static bool httpsGetJson(const String& url, DynamicJsonDocument& doc) {
  WiFiClientSecure client;
  client.setInsecure();  // simplify TLS on ESP32
  HTTPClient http;

  Serial.printf("[httpsGetJson] GET %s\n", url.c_str());

  if (!http.begin(client, url)) {
    Serial.printf("[httpsGetJson] HTTP begin failed: %s\n", url.c_str());
    return false;
  }
  const int code = http.GET();
  Serial.printf("[httpsGetJson] HTTP code: %d\n", code);
  if (code != HTTP_CODE_OK) {
    Serial.printf("[httpsGetJson] HTTP GET %s -> %d\n", url.c_str(), code);
    http.end();
    return false;
  }
  const String payload = http.getString();
  http.end();

  DeserializationError err = deserializeJson(doc, payload);
  if (err) {
    Serial.printf("[httpsGetJson] JSON parse error on %s: %s\n", url.c_str(), err.c_str());
    return false;
  }
  return true;
}

// Build URL to get latest-hour for ALL stations under a parameter (we'll filter for Karlskrona)
static String smhiAllStationsLatestHourForParam(int paramId) {
  String url = "https://opendata-download-metobs.smhi.se/api/version/latest/parameter/";
  url += String(paramId);
  url += "/station-set/all/period/latest-hour/data.json";
  return url;
}

// Find first station with name containing "Karlskrona" and return its latest value/unit/timestamp
static bool smhiFindKarlskronaValueForParam(int paramId, float& outValue, String& outUnit, String& outStation, String& outTimestamp) {
  const String url = smhiAllStationsLatestHourForParam(paramId);
  DynamicJsonDocument doc(950 * 1024);
  if (!httpsGetJson(url, doc)) {
    Serial.println("[smhiFindKarlskronaValueForParam] could not fetch JSON");
    return false;
  }

  // Try compact "data" array schema first
  if (doc.containsKey("data")) {
    for (JsonObject o : doc["data"].as<JsonArray>()) {
      String sname = (const char*) (o["station"]["name"] | "");
      String sL = sname; sL.toLowerCase();
      if (sL.indexOf("karlskrona") >= 0) {
        outStation   = sname;
        outUnit      = (const char*) (o["parameter"]["unit"] | "");
        outValue     = o["value"] | NAN;
        if (o.containsKey("date")) outTimestamp = (const char*) (o["date"] | "");
        else if (o.containsKey("to")) outTimestamp = (const char*) (o["to"] | "");
        else outTimestamp = "";
        Serial.printf("[smhiFindKarlskronaValueForParam] found station=%s value=%.2f %s\n",
                      sname.c_str(), outValue, outUnit.c_str());
        return true;
      }
    }
    Serial.println("[smhiFindKarlskronaValueForParam] no Karlskrona in 'data' array");
    return false;
  }

  // Legacy separated arrays: "station"[], "value"[], and top-level "parameter"
  if (doc.containsKey("station") && doc.containsKey("value")) {
    JsonArray stations = doc["station"].as<JsonArray>();
    JsonArray values   = doc["value"].as<JsonArray>();
    String unit        = (const char*) (doc["parameter"]["unit"] | "");
    size_t n = min(stations.size(), values.size());
    for (size_t i = 0; i < n; ++i) {
      JsonObject s = stations[i];
      String sname = (const char*) (s["name"] | "");
      String sL = sname; sL.toLowerCase();
      if (sL.indexOf("karlskrona") >= 0) {
        JsonObject v = values[i];
        outStation   = sname;
        outUnit      = unit;
        outValue     = v["value"] | NAN;
        if (v.containsKey("date")) outTimestamp = (const char*) (v["date"] | "");
        else if (doc.containsKey("to")) outTimestamp = (const char*) (doc["to"] | "");
        else outTimestamp = "";
        Serial.printf("[smhiFindKarlskronaValueForParam] found station(legacy)=%s value=%.2f %s\n",
                      sname.c_str(), outValue, outUnit.c_str());
        return true;
      }
    }
  }
  Serial.println("[smhiFindKarlskronaValueForParam] no Karlskrona found at all");
  return false;
}

// Nu förenklar vi: denna IGNORERAR parameternamn och tar alltid param 1 (lufttemperatur)
static SmhiObs smhiGetKarlskrona() {
  SmhiObs out{false, NAN, "", "", ""};

  float val; String unit, st, ts;
  if (smhiFindKarlskronaValueForParam(SMHI_PARAM_LUFTTEMP, val, unit, st, ts)) {
    out.ok          = true;
    out.value       = val;
    out.unit        = unit;
    out.station     = st;
    out.timestampUTC= ts;
  } else {
    Serial.println("[smhiGetKarlskrona] FAILED to get Karlskrona temp");
  }
  return out;
}

// ================= UI / RESTEN AV DIN KOD =================

template <typename T>
class Dropdown{
  private:
    vector<T> choices;
    lv_obj_t * dropdownBox;
  public:
    Dropdown(const vector<T>& cities, lv_obj_t * parent): choices(cities){

        if(cities.size()==0){
            Serial.println("Empty list");
            return;
        }
        dropdownBox = lv_dropdown_create(parent);
        string optionStr;
        for(size_t i = 0; i < cities.size(); i++){
            stringstream ss;
            ss << cities[i];
            optionStr += ss.str();
            if (i < cities.size() - 1){
            optionStr += "\n";
            }
        }
        lv_dropdown_set_options(dropdownBox,optionStr.c_str());
        lv_obj_align(dropdownBox, LV_ALIGN_BOTTOM_MID, 20, 100);
        lv_dropdown_set_selected(dropdownBox, 0);
        lv_obj_add_event_cb(dropdownBox, event_handler, LV_EVENT_VALUE_CHANGED, NULL);
    }

    static void event_handler(lv_event_t * e){
      lv_event_code_t code = lv_event_get_code(e);
      lv_obj_t * obj = lv_event_get_target(e);
      if(code == LV_EVENT_VALUE_CHANGED) {
          char buf[32];
          lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
          LV_LOG_USER("Option: %s", buf);
      }
    }
};

// Wi-Fi credentials
static const char* WIFI_SSID     = "Salleh";
static const char* WIFI_PASSWORD = "00000000";

LilyGo_Class amoled;

static lv_obj_t* tileview;
static lv_obj_t* t1;
static lv_obj_t* t2;
static lv_obj_t* t1_label;
static lv_obj_t* t2_label;
static bool t2_dark = false;
lv_obj_t *slider;

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

  {
    t1_label = lv_label_create(t1);
    lv_label_set_text(t1_label, "Hello WORKING");
    lv_obj_set_style_text_font(t1_label, &lv_font_montserrat_28, 0);
    lv_obj_center(t1_label);
    apply_tile_colors(t1, t1_label, false);
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

// slider
void slider_event_cb(lv_event_t *e) {
  lv_obj_t *slider = lv_event_get_target(e);
  int val = lv_slider_get_value(slider);
  (void)val;
}

void setup()
{
  Serial.begin(115200);
  delay(200);
  delay(2000);
  Serial.println("=== BOOT ===");

  if (!amoled.begin()) {
    Serial.println("Failed to init LilyGO AMOLED.");
    while (true) delay(1000);
  }

  beginLvglHelper(amoled);
  create_ui();
  vector<string> stader = {"Lund", "karlskrona", "Malmö", "Stockholm"};
  Dropdown<string> myDropdown(stader, t2);

  connect_wifi();

  // slider
  slider = lv_slider_create(lv_scr_act());
  lv_obj_set_width(slider, 300);
  lv_slider_set_range(slider, 0, 100);
  lv_slider_set_value(slider, 50, LV_ANIM_OFF);
  lv_obj_align(slider, LV_ALIGN_BOTTOM_MID, 0, -10);
  lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

  // --- TESTA SMHI HÄR ---
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("[setup] WiFi OK, fetching SMHI (Karlskrona, lufttemp)...");
    SmhiObs temp = smhiGetKarlskrona();
    if (temp.ok) {
      char buf[64];
      snprintf(buf, sizeof(buf), "Karlskrona: %.1f %s", temp.value, temp.unit.c_str());
      lv_label_set_text(t2_label, buf);
      Serial.println(buf);
    } else {
      lv_label_set_text(t2_label, "SMHI: inget svar");
      Serial.println("[setup] SMHI: inget svar");
    }
  } else {
    lv_label_set_text(t2_label, "WiFi ej ansluten");
  }
}

void loop()
{
  lv_timer_handler();
}

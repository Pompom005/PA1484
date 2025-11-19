#include<Dropdown.h>

template <typename T>
Dropdown <T>::Dropdown(const vector<T>& cities, lv_obj_t * parent, int x, int y): choices(cities){
        if(cities.size()==0){
            Serial.println("Empty list");
            return;
        }
        if (!parent){
            Serial.println("No ptr found");
            return;
        }
        dropdownBox = lv_dropdown_create(parent);
        optionStr = makeittostring(cities);
        lv_dropdown_set_options(dropdownBox,optionStr.c_str());
        // i need to do a if sats som kollar om a bix is already created or not
        screenpos(x, y);//lv_obj_align(dropdownBox, LV_ALIGN_BOTTOM_MID, 0, 10);// have a function to itself that can change these numbers 20 and 10
        lv_dropdown_set_selected(dropdownBox, 0);
        lv_obj_add_event_cb(dropdownBox, event_handler, LV_EVENT_VALUE_CHANGED, this);
}

template <typename T>
void Dropdown<T>::event_handler(lv_event_t * e){
lv_event_code_t code = lv_event_get_code(e);
lv_obj_t * obj = lv_event_get_target(e);

if(code == LV_EVENT_VALUE_CHANGED) {
    char buf[64];
    lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
    Dropdown<T>* instance = static_cast<Dropdown<T>*>(lv_event_get_user_data(e));
    instance->selectedValue = buf;
    LV_LOG_USER("Selected: %s", buf);
}
}

template <typename T>
string Dropdown<T>:: makeittostring(const vector<T> & stad){
    string option;
    for(size_t i = 0; i < stad.size(); i++){
        stringstream ss;
        ss << stad[i];
        option += ss.str();
        if (i < stad.size() - 1){
        option += "\n";
        }
    }
    return option;
}

template <typename T>
void Dropdown<T>::screenpos(int x, int y){
    lv_obj_align(dropdownBox, LV_ALIGN_BOTTOM_MID, x, y);
}

template <typename T>
string Dropdown<T>:: getValue(){
    return selectedValue;
}
template class Dropdown <string>;
template class Dropdown <int>;
template class Dropdown <float>;
template class Dropdown <bool>;
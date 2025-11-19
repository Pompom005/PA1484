#include<Linegraf.h>
Linegraf:: Linegraf(lv_obj_t*parent, const vector<float>& chosen, string name){ // test chosen
    obj1 = lv_chart_create(parent);
    lv_obj_set_size(obj1, 500, 400);
    lv_obj_center(obj1);
    lv_chart_set_type(obj1, LV_CHART_TYPE_LINE);
    changelocation(name);
    //first fill in the list
    // getminmaxsize
    int result = getminmaxsize(chosen);
    // replacedata
    replacedata(mx, mn, result);
}
void Linegraf:: replacedata(float max_value, float min_value, int numberofdots){
    lv_chart_series_t* serie = lv_chart_add_series(obj1, lv_palette_main(LV_PALETTE_ORANGE), LV_CHART_AXIS_SECONDARY_Y);
    uint32_t i;
    for(i =0; i< numberofdots; i++){
        lv_chart_set_next_value(obj1, serie, (int32_t)lv_rand(min_value, max_value));

    }
    lv_chart_refresh(obj1);
}

void Linegraf:: adddatatolist(float data){
    valuesforthegraf.push_back(data);

}

int Linegraf:: getminmaxsize(const vector<float>& grafvalues){
    if (grafvalues.empty()){
        Serial.println("Empty");
        return 0;
    }
    auto largest = max_element(grafvalues.begin(), grafvalues.end());
    auto smallest = min_element(grafvalues.begin(), grafvalues.end());
    mx = *largest;
    mn = *smallest;
    return grafvalues.size();
}


void Linegraf:: changelocation(string changed){
    location = changed;
    // either create a helpfunction or (adddatatolist directly?)
    //adddatatolist();
}

void Linegraf:: thehelpfunction(){
    //adddatatolist()
}


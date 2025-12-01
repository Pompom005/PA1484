#include<Linegraf.h>
Linegraf:: Linegraf(lv_obj_t*parent_tile, const vector<float>& chosen){
    obj1 = lv_chart_create(parent_tile);
    //lv_obj_t* chart = lv_chart_create(parent_tile);
    int graph_width = lv_obj_get_content_width(lv_scr_act());
    int graph_height = lv_obj_get_content_height(lv_scr_act());
    
    lv_obj_set_size(obj1, graph_width-50, graph_height); //graph_width is a temp example. TODO: test for better value 
    //lv_obj_center(chart);
    //lv_chart_set_type(chart, LV_CHART_TYPE_LINE); 
    //lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    //lv_obj_set_size(obj1, 500, 400);
    lv_chart_set_type(obj1, LV_CHART_TYPE_LINE);
    
    //first fill in the list
    // getminmaxsize
    int result = getminmaxsize(chosen);
    lv_chart_set_point_count(obj1, result);
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





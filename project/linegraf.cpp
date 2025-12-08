#include<Linegraf.h>
Linegraf:: Linegraf(lv_obj_t*parent_tile, const vector<lv_coord_t>& chosen){
    obj1 = lv_chart_create(parent_tile);
    //lv_obj_t* chart = lv_chart_create(parent_tile);
    int graph_width = lv_obj_get_content_width(lv_scr_act());
    int graph_height = lv_obj_get_content_height(lv_scr_act());
    
    lv_obj_set_size(obj1, graph_width-50, graph_height); //graph_width is a temp example. TODO: test for better value 

    lv_chart_set_type(obj1, LV_CHART_TYPE_LINE);
    serie1 = lv_chart_add_series(obj1, lv_palette_main(LV_PALETTE_ORANGE), LV_CHART_AXIS_SECONDARY_Y);
    replacedata(chosen, false);

}
// this is the newone
// void Linegraf:: replacedata(int numberofdots, const vector<float>& chosen){
//     lv_chart_series_t* serie1 = lv_chart_add_series(obj1, lv_palette_main(LV_PALETTE_ORANGE), LV_CHART_AXIS_SECONDARY_Y);
//     for(int i = 0; i < numberofdots ; i++) {
//     serie1->x_points[i] = chosen[i];  // replace old with new value you want
// }
//     lv_chart_refresh(obj1);
// }
// this is the old replacedata
void Linegraf:: replacedata(const vector<lv_coord_t>& chosen, bool clearolddata){
    // if(clearolddata){
    //     lv_chart_remove_series(obj1, serie1);
    // }

    // if(!serie1){
    //     serie1 = lv_chart_add_series(obj1, lv_palette_main(LV_PALETTE_ORANGE), LV_CHART_AXIS_SECONDARY_Y);
    // }
    values = chosen;
    lv_chart_set_point_count(obj1, chosen.size());

    // for(int i =0; i< chosen.size(); i++){
    //     lv_chart_set_next_value(obj1, serie1, chosen[i]);

    // }
    lv_chart_set_ext_y_array(obj1, serie1, values.begin().base());
    lv_chart_refresh(obj1);

}



// void Linegraf::setDataFromSMHI(vector<HistoricalDataPoint> & history){

//    lv_chart_set_point_count(obj1, history.size());
//     // replacedata
//     replacedata(history.size(), history);
//     // reset


// }










#include<Linegraf.h>
#include "LV_Helper.h"

void Linegraph::DrawCallbackEvent(lv_event_t* e)
{
    //Get the event descriptor
    lv_obj_draw_part_dsc_t* dsc = lv_event_get_draw_part_dsc(e);

    //Check what part we are updating. Only proceed if we are updating one of the tick label(s)
    if (!lv_obj_draw_part_check_type(dsc, &lv_chart_class, LV_CHART_DRAW_PART_TICK_LABEL))
        return;

    //Check this is a callback for a major tick (minor ticks have 0 here)
    if (dsc->text == NULL)
        return;

    //If we are editing the Y axis
    if (dsc->id == LV_CHART_AXIS_PRIMARY_Y)
    {
        //update the label with the modified multiplier
        lv_snprintf(dsc->text, dsc->text_length, "%.2f", (float)dsc->value / 100.0f);
    }
}

Linegraph:: Linegraph(lv_obj_t*parent_tile, const vector<lv_coord_t>& chosen){
    parent = parent_tile;
    obj1 = lv_chart_create(parent_tile);
    //lv_obj_t* chart = lv_chart_create(parent_tile);
    lv_obj_set_pos(obj1, 50, 0);
    int graph_width = lv_obj_get_content_width(parent_tile);
    int graph_height = lv_obj_get_content_height(parent_tile);
    lv_chart_set_axis_tick(obj1, LV_CHART_AXIS_PRIMARY_Y, 5, 2, 10, 3, true, 50);
    lv_obj_set_size(obj1, graph_width * 0.95f, graph_height); //graph_width is a temp example. TODO: test for better value 

    lv_obj_set_style_size(obj1, 0, LV_PART_INDICATOR);
    lv_chart_set_type(obj1, LV_CHART_TYPE_LINE);
    lv_obj_add_event_cb(obj1, DrawCallbackEvent, LV_EVENT_DRAW_PART_BEGIN, 0);
    serie1 = lv_chart_add_series(obj1, lv_palette_main(LV_PALETTE_ORANGE), LV_CHART_AXIS_PRIMARY_Y);
    replace_data(chosen, false);

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
void Linegraph:: replace_data(const vector<lv_coord_t>& chosen, bool clearolddata){
    // if(clearolddata){
    //     lv_chart_remove_series(obj1, serie1);
    // }

    // if(!serie1){
    //     serie1 = lv_chart_add_series(obj1, lv_palette_main(LV_PALETTE_ORANGE), LV_CHART_AXIS_SECONDARY_Y);
    // }
    values = chosen;
    for(int i = 0; i < values.size(); i++)
    {
        values[i] *= 100.0f;
    }
    lv_chart_set_point_count(obj1, values.size());

    int graph_width = lv_obj_get_content_width(parent);
    int graph_height = lv_obj_get_content_height(parent);

    get_the_max_min_value(values);
    lv_chart_set_range(obj1, LV_CHART_AXIS_PRIMARY_Y,minvalue, maxvalue );

    int width = values.size()*2;
    if(width < graph_width * 0.95f)
    {
        width = graph_width * 0.95f;
    }
    
    lv_obj_set_size(obj1, width, graph_height); //graph_width is a temp example. TODO: test for better value 

    // for(int i =0; i< chosen.size(); i++){
    //     lv_chart_set_next_value(obj1, serie1, chosen[i]);

    // }
    lv_chart_set_ext_y_array(obj1, serie1, values.begin().base());
    lv_chart_refresh(obj1);

}

void Linegraph::get_the_max_min_value(const vector<lv_coord_t>& chosen)
{

    auto anotherminvalue = min_element(chosen.begin(), chosen.end());
    auto anothermaxvalue = max_element(chosen.begin(), chosen.end());
    minvalue = *anotherminvalue;
    maxvalue = *anothermaxvalue;
}




// void Linegraf::setDataFromSMHI(vector<HistoricalDataPoint> & history){

//    lv_chart_set_point_count(obj1, history.size());
//     // replacedata
//     replacedata(history.size(), history);
//     // reset


// }










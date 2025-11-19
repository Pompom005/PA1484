#include <LilyGo_AMOLED.h>
#include "Graph.h"

Graph::Graph(lv_obj_t* parent_tile, vector<string> data)
{
    //Creates the graph
    lv_obj_t* chart = lv_chart_create(parent_tile);
    int graph_width = lv_obj_get_content_width(lv_scr_act());
    int graph_height = lv_obj_get_content_height(lv_scr_act());
    
    lv_obj_set_size(chart, graph_width + 100, graph_height); //graph_width is a temp example. TODO: test for better value 
    lv_obj_center(chart);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE); 
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    //Adds dataseries and values(data)
    lv_chart_series_t *series = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

    for(int i = 0; i < 10; i++) { //have to cycle through the dates, i < amount_of_dates ?
        int temperature = stof(data.back()); 
        series->y_points[i] = temperature;
    }
    lv_chart_refresh(chart);
}

void Graph::AddDataseries(vector<int> value_vector)
{
    for(int value: value_vector) {
        lv_chart_set_next_value(chart, series, value_vector[value]); 
    }
    lv_chart_refresh(chart);
}

void Graph::ReplaceDataseries(vector<int> value_vector)
{
    //Rough example code, not final
    lv_chart_series_t *series = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

    for(int value: value_vector) { 
        int temperature = value_vector[value]; 
        series->y_points[value] = temperature;
    }
    lv_chart_refresh(chart);
}

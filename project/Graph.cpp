#include <LilyGo_AMOLED.h>
#include "Graph.h"
#include <sstream> // Remove?
#include <iomanip> // Remove?

Graph::Graph(lv_obj_t* parent_tile)
{
    //Creates the graph
    lv_obj_t* chart = lv_chart_create(parent_tile);
    int graph_width = lv_obj_get_content_width(lv_scr_act());
    int graph_height = lv_obj_get_content_height(lv_scr_act());
    
    lv_obj_set_size(chart, graph_width + 100, graph_height); //graph_width is a temp example. TODO: test for better value 
    lv_obj_center(chart);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE); 
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    //Adds dataseries and values
    lv_chart_series_t *series = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

    //Example code, should use weather data 
    for(int i = 0; i < 10; i++) {
        series->y_points[i] = lv_rand(50,90);
    }
    lv_chart_refresh(chart);
}

void Graph::AddDataseries()
{
    

    lv_chart_refresh(chart);
}

void Graph::ReplaceDataseries(int value_array)
{
    for(int i = 0; i < value_array.size(); i++) { ///???
        lv_chart_set_next_value(chart, series, value_array[i]);
    }
}

/*
  //Original chart code, lacks data

  //Chart with historic data
  lv_obj_t* chart = lv_chart_create(t3); 
  int graph_width = lv_obj_get_content_width(lv_scr_act());
  int graph_height = lv_obj_get_content_height(lv_scr_act());
  lv_obj_set_size(chart, graph_width + 100, graph_height);
  lv_obj_center(chart);
  lv_chart_set_type(chart, LV_CHART_TYPE_LINE); 
  lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
*/  
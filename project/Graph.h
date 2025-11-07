#pragma once
#include <string>
#include <LV_Helper.h>

class Graph 
{
    private:

    lv_obj_t* chart;
    lv_chart_series_t *series;

    int value_array;

    int graph_width;
    int graph_height;

    public:
    
    Graph(lv_obj_t* parent_tile);

    void AddDataseries();
    void ReplaceDataseries(int value_array); //parameter?
};
#pragma once
#include <string>
#include <vector>
#include <LV_Helper.h>
using namespace std;

class Graph 
{
    private:

    lv_obj_t* chart;
    lv_chart_series_t *series;

    vector<float> value_vector; 

    int graph_width;
    int graph_height;

    public:
    
    Graph(lv_obj_t* parent_tile, vector<float> data); 

    void AddDataseries(vector<float> value_vector);
    void ReplaceDataseries(vector<float> value_vector); //parameter?
};
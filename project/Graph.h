#pragma once
#include <string>
#include <LV_Helper.h>

class Graph {
    private:

    //Screen dimensions for graph. Maybe unneccesary?
    int graph_width;
    int graph_height;

    public:
    
    Graph(lv_obj_t* parent_tile);
};
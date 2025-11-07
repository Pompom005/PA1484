#include<linegraf.h>
Linegraf:: Linegraf(lv_obj_t*parent, int num){
    obj1 = lv_chart_create(parent);
    lv_obj_set_size(obj1, 100, 100);
    lv_obj_center(obj1);

    chart = lv_chart_create(obj1);
    lv_obj_set_width(chart, lv_pct(100));
    lv_obj_set_flex_grow(chart, 1);
    lv_chart_set_type(chart, LV_CHART_TYPE_BAR);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y,0, 50);
    lv_chart_set_range(chart, LV_CHART_AXIS_SECONDARY_Y,0, 100);
    lv_chart_set_point_count(chart, 12);
    lv_obj_set_style_radius(chart, 0, 0);
    // create scale


}
void Linegraf:: percentfunction(){}
#include<linegraf.h>
Linegraf:: Linegraf(lv_obj_t*parent, int num){
    percentage = num;
    if (percentage == 0){
    obj1 = lv_chart_create(parent);
}
}
void Linegraf:: percentfunction(){}
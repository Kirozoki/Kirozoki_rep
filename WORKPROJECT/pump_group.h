#pragma once
#include <iostream>
#include "pressure.h"
#include "stuf_box_pressure_function.h"
#include <vector>
#include <utility>
 
class PumpGroup {
public:
PumpGroup()=default;

PumpGroup(int index, Pressure suction_pressure_rated, Pressure suction_pressure_max) : 
group_index_(index), suction_pressure_rated_(suction_pressure_rated), 
suction_pressure_max_(suction_pressure_max) 
{

    switch (index)
    {
case 1: //Ps + 0.15*Pdiff
factor_of_diff_pressure_ = 0.15;
InputHydraulicData(); 
stuf_box_pressure_.push_back({StufBoxPressureFunction(suction_pressure_rated_, factor_of_diff_pressure_, Pressure(density_rated, head_rated)),
StufBoxPressureFunction(suction_pressure_max_, factor_of_diff_pressure_, Pressure(density_max, head_max))});
break;
case 2: //Ps + 0.10*Pdiff
factor_of_diff_pressure_ = 0.1;
InputHydraulicData(); 
stuf_box_pressure_.push_back({StufBoxPressureFunction(suction_pressure_rated_, factor_of_diff_pressure_, Pressure(density_rated, head_rated)),
StufBoxPressureFunction(suction_pressure_max_, factor_of_diff_pressure_, Pressure(density_max, head_max))});
break;
case 3: //Pseal = Ps 
stuf_box_pressure_.push_back({StufBoxPressureFunction(suction_pressure_rated_),
StufBoxPressureFunction(suction_pressure_max_)});
break;
case 4: //Ps + 0.70*Pdiff
factor_of_diff_pressure_ = 0.7;
InputHydraulicData();
stuf_box_pressure_.push_back({StufBoxPressureFunction(suction_pressure_rated_, factor_of_diff_pressure_, Pressure(density_rated, head_rated)),
StufBoxPressureFunction(suction_pressure_max_, factor_of_diff_pressure_, Pressure(density_max, head_max))});
break;
case 5: //Pseal = Ps
stuf_box_pressure_.push_back({StufBoxPressureFunction(suction_pressure_rated_),
StufBoxPressureFunction(suction_pressure_max_)});
stuf_box_pressure_.push_back({StufBoxPressureFunction(suction_pressure_rated_),
StufBoxPressureFunction(suction_pressure_max_)});
break;
case 6: //Pseal Suction Side=Ps, Pseal 2nd Stage=Ps+0.5*PDiff
stuf_box_pressure_.push_back({StufBoxPressureFunction(suction_pressure_rated_),
StufBoxPressureFunction(suction_pressure_max_)});
factor_of_diff_pressure_ = 0.5; // 2nd stage factor
InputHydraulicData(); 
stuf_box_pressure_.push_back({StufBoxPressureFunction(suction_pressure_rated_, factor_of_diff_pressure_, Pressure(density_rated, head_rated)),
StufBoxPressureFunction(suction_pressure_max_, factor_of_diff_pressure_, Pressure(density_max, head_max))});
break;
case 7: //Ps+2~3bar (both sides)
stuf_box_pressure_.push_back({StufBoxPressureFunction(suction_pressure_rated_, 0.0, {0.0, MU::Pa}, {2.0, MU::bar}),
StufBoxPressureFunction(suction_pressure_max_, 0.0, {0.0, MU::Pa}, {3.0, MU::bar})});
stuf_box_pressure_.push_back({StufBoxPressureFunction(suction_pressure_rated_, 0.0, {0.0, MU::Pa}, {2.0, MU::bar}),
StufBoxPressureFunction(suction_pressure_max_, 0.0, {0.0, MU::Pa}, {3.0, MU::bar})});
break;
case 8: //shaft driven end =  Ps, non-driven end = Ps + 0.3bar
stuf_box_pressure_.push_back({StufBoxPressureFunction(suction_pressure_rated_),
StufBoxPressureFunction(suction_pressure_max_)});
stuf_box_pressure_.push_back({StufBoxPressureFunction(suction_pressure_rated_, 0.0, {0.0, MU::Pa}, {0.3, MU::bar}),
StufBoxPressureFunction(suction_pressure_max_, 0.0, {0.0, MU::Pa}, {0.3, MU::bar})});
break;
case 9: //Pseal=Ps
stuf_box_pressure_.push_back({StufBoxPressureFunction(suction_pressure_rated_),
StufBoxPressureFunction(suction_pressure_max_)});
break;
case 10: //Pseal=Ps+Pdif
factor_of_diff_pressure_ = 1.0;
InputHydraulicData(); 
stuf_box_pressure_.push_back({StufBoxPressureFunction(suction_pressure_rated_, factor_of_diff_pressure_, Pressure(density_rated, head_rated)),
StufBoxPressureFunction(suction_pressure_max_, factor_of_diff_pressure_, Pressure(density_max, head_max))});
break;
case 11: //Pseal=Ps+0.4*PDiff
factor_of_diff_pressure_ = 0.4;
InputHydraulicData();  
stuf_box_pressure_.push_back({StufBoxPressureFunction(suction_pressure_rated_, factor_of_diff_pressure_, Pressure(density_rated, head_rated)),
StufBoxPressureFunction(suction_pressure_max_, factor_of_diff_pressure_, Pressure(density_max, head_max))});
break;
case 12: //Pseal=Ps+0.3*PDiff
factor_of_diff_pressure_ = 0.3;
InputHydraulicData();  
stuf_box_pressure_.push_back({StufBoxPressureFunction(suction_pressure_rated_, factor_of_diff_pressure_, Pressure(density_rated, head_rated)),
StufBoxPressureFunction(suction_pressure_max_, factor_of_diff_pressure_, Pressure(density_max, head_max))});
break;
case 13: //Pseal= Ps+1.1*Pdiff
factor_of_diff_pressure_ = 1.1; 
InputHydraulicData();
stuf_box_pressure_.push_back({StufBoxPressureFunction(suction_pressure_rated_, factor_of_diff_pressure_, Pressure(density_rated, head_rated)),
StufBoxPressureFunction(suction_pressure_max_, factor_of_diff_pressure_, Pressure(density_max, head_max))});
break;
default: std::cout << "You input incorrect value, please try again" << std::endl; 
// include error lib instead of above
break;
    }
}

~PumpGroup() {}

void GetData() {
    int counter = 0;
    for(auto [rated, max] : stuf_box_pressure_) {
        std::string text_adder;  
        if (counter % 2 == 0) 
        text_adder = "DE"; 
        std::cout << "\n Stuffing box chamber pressure " << text_adder << " side, rated: " << rated << " and max: " << max; 
        ++counter;
    }
}

private:
int group_index_ = 1;
double factor_of_diff_pressure_=0.0; 
Pressure suction_pressure_rated_;
Pressure suction_pressure_max_;
std::vector<std::pair<Pressure, Pressure>> stuf_box_pressure_ = {};
Density density_rated = {0, MU_density::kg_m3}, density_max = {0, MU_density::kg_m3};
Head head_rated = {0, MU_head::m}, head_max = {0, MU_head::m};

void InputHydraulicData() {
std::cout << "Please enter rated conditions (density, head): " << std::endl;
std::cin >> density_rated >> head_rated;
std::cout << "Please enter max conditions (density, head): " << std::endl;
std::cin >> density_max >> head_max; 
}
};
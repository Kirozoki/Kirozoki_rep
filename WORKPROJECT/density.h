#pragma once
#include <utility>
#include <iostream>
#include <algorithm>

// 0.9999720 kg/l water density at +4C
enum class MU_density {kg_m3, SG, kg_l, g_l, mg_l, g_cm3, mg_cm3};

class Density {

public:
    Density() = default;
    
    Density(double value, MU_density index) : value_(value), unit_(index)
    {
        // write a definition of pa_value
        switch(index) {
            case MU_density::kg_m3:
            kg_m3_value_=value;
            break;
            case MU_density::SG:
            kg_m3_value_ = value*999.9720;
            break;
            case MU_density::kg_l:
            kg_m3_value_ = value*1000;
            break;
            case MU_density::g_l:
            kg_m3_value_ = value;
            break;
            case MU_density::mg_l:
            kg_m3_value_ = value/1000;
            break;
            case MU_density::g_cm3:
            kg_m3_value_ = value*1000;
            break;
            case MU_density::mg_cm3:
            kg_m3_value_ = value;
            break;
        }
    }

    ~Density()  {}

    double GetDensityKgM3() {
        return kg_m3_value_;
    }

    std::pair<double, MU_density> GetDensity() {
        return {value_, unit_};
    }

    void ChangeDensity(double value, MU_density unit) {
        value_= value;
        unit_ = unit;
        switch(unit) {
            case MU_density::kg_m3:
            kg_m3_value_=value;
            break;
            case MU_density::SG:
            kg_m3_value_ = value*999.9720;
            break;
            case MU_density::kg_l:
            kg_m3_value_ = value*1000;
            break;
            case MU_density::g_l:
            kg_m3_value_ = value;
            break;
            case MU_density::mg_l:
            kg_m3_value_ = value/1000;
            break;
            case MU_density::g_cm3:
            kg_m3_value_ = value*1000;
            break;
            case MU_density::mg_cm3:
            kg_m3_value_ = value;
            break;
        }
    }

    void ChangeUnits(MU_density unit) {
    switch(unit) {
            case MU_density::kg_m3:
            value_=kg_m3_value_;
            break;
            case MU_density::SG:
            value_=kg_m3_value_/999.9720;
            break;
            case MU_density::kg_l:
            value_=kg_m3_value_/1000;
            break;
            case MU_density::g_l:
            value_=kg_m3_value_;
            break;
            case MU_density::mg_l:
            value_=kg_m3_value_*1000;
            break;
            case MU_density::g_cm3:
            value_=kg_m3_value_/1000;
            break;
            case MU_density::mg_cm3:
            value_=kg_m3_value_;
            break;
        }
    unit_ = unit;
    }

    Density& operator+=(Density right) {
        kg_m3_value_ = kg_m3_value_ + right.GetDensityKgM3();
        MU_density unit = std::max(unit_, right.GetDensity().second); 
        this->ChangeUnits(unit);
        return *this;
    }

    Density& operator-=(Density right) {
        kg_m3_value_ = kg_m3_value_ - right.GetDensityKgM3();
        MU_density unit = std::min(unit_, right.GetDensity().second); 
        this->ChangeUnits(unit);
        return *this;
    }  

    Density& operator*=(double right) {
        kg_m3_value_ = kg_m3_value_ * right;
        value_ = value_ * right;
        return *this;
    }

    Density& operator/=(double right) {
        kg_m3_value_ = kg_m3_value_ / right;
        value_ = value_ / right;
        return *this;
    }

    private:
    double value_= 0.0;
    double kg_m3_value_=0.0;
    MU_density unit_ = MU_density::kg_m3;
};

std::ostream& operator<<(std::ostream& output, Density density) {
    std::string unit_text;
    switch (density.GetDensity().second) 
    {
            case MU_density::kg_m3:
            unit_text = "kg/m3";
            break;
            case MU_density::SG:
            unit_text = "SG";
            break;
            case MU_density::kg_l:
            unit_text = "kg/l";
            break;
            case MU_density::g_l:
            unit_text = "g/l";
            break;
            case MU_density::mg_l:
            unit_text = "mg/l";
            break;
            case MU_density::g_cm3:
            unit_text = "g/cm3";
            break;
            case MU_density::mg_cm3:
            unit_text = "mg/cm3";
            break;    
    }
    output << density.GetDensity().first << " " << unit_text;
    return output;
}

std::istream& operator>>(std::istream& input, Density& density) {
    std:: cout << "please enter a density value: ";
    double density_value;
    MU_density m_unit;
    int unit_index;
    input >> density_value;
    std::cout << "Please enter measuring units for input (0 = kg/m3, 1 = SG, 2 = kg/l, 3 = g/l, "
<< "4 = mg/l, 5 = g/cm3, 6 = mg/cm3)" << std::endl;
    input >> unit_index;
    switch (unit_index)
    {
    case 0: m_unit = MU_density::kg_m3;
        break;
    case 1: m_unit = MU_density::SG;
        break;
    case 2: m_unit = MU_density::kg_l;
        break;
    case 3: m_unit = MU_density::g_l;
        break;
    case 4: m_unit = MU_density::mg_l;
        break;
    case 5: m_unit = MU_density::g_cm3;
        break;
    case 6: m_unit = MU_density::mg_cm3;
        break;
    default:
        break;
    }
    density = Density(density_value, m_unit);
    return input;
}

Density operator+(Density left, Density right) {
    return left+=right;
}

Density operator-(Density left, Density right) {
    return left-=right;
}

Density operator*(Density left, double right) {
    return left*=right;
}

Density operator/(Density left, double right) {
    return left /= right;
}

bool operator==(Density left, Density right) {
    return left.GetDensityKgM3()==right.GetDensityKgM3();
}

bool operator!=(Density left, Density right) {
    return !(left == right);
}
#pragma once
#include "density.h"
#include "head.h"

const double Gravitational_Acceleration = 9.81;

enum class MU{Pa, kPa, bar, MPa, kg_cm2, m_H2O, m_Hg};

class Pressure {
    public:
    Pressure() = default;
    
    Pressure(double value, MU index) : value_(value), unit_(index)
    {
        // write a definition of pa_value
        switch(unit_) {
            case MU::Pa:
            pa_value_=value;
            break;
            case MU::kPa:
            pa_value_ = value*1000;
            break;
            case MU::bar:
            pa_value_ = value*100000;
            break;
            case MU::MPa:
            pa_value_ = value*1000000;
            break;
            case MU::kg_cm2:
            pa_value_ = value*98066.5;
            break;
            case MU::m_H2O:
            pa_value_ = value*9.81*998.4;
            break;
            case MU::m_Hg:
            pa_value_ = value*133.322;
            break;
        }
    }

    Pressure(Density density, Head head) : 
    pa_value_(density.GetDensityKgM3() * head.GetHeadM() * Gravitational_Acceleration),
    unit_(MU::Pa)
    {
    value_ = pa_value_; 
    }

    ~Pressure() {}

    
    double GetPressurePa() {
        return pa_value_;
    }

    std::pair<double, MU> GetPressure() {
        return {value_, unit_};
    }

    void ChangeUnits(MU unit) {
    switch(unit) {
            case MU::Pa:
            value_=pa_value_;
            break;
            case MU::kPa:
            value_=pa_value_/1000;
            break;
            case MU::bar:
            value_=pa_value_/100000;
            break;
            case MU::MPa:
            value_=pa_value_/1000000;
            break;
            case MU::kg_cm2:
            value_=pa_value_/98066.5;
            break;
            case MU::m_H2O:
            value_=pa_value_/(9.81*998.4);
            break;
            case MU::m_Hg:
            value_=pa_value_/133.322;
            break;
        } 
unit_ = unit;
    }

    Pressure& operator+=(Pressure right) {
        pa_value_ = pa_value_ + right.GetPressurePa();
        MU unit = std::max(unit_, right.GetPressure().second); 
        this->ChangeUnits(unit);
        return *this;
    }

    Pressure& operator-=(Pressure right) {
        pa_value_ = pa_value_ - right.GetPressurePa();
        MU unit = std::min(unit_, right.GetPressure().second); 
        this->ChangeUnits(unit);
        return *this;
    }  

    Pressure& operator*=(double right) {
        pa_value_ = pa_value_ * right;
        value_ = value_ * right;
        return *this;
    }

Pressure& operator/=(double right) {
        pa_value_ = pa_value_ / right;
        value_ = value_ / right;
        return *this;
    }

    private:
    double value_= 0.0;
    double pa_value_=0.0;
    MU unit_ = MU::Pa;
};

std::ostream& operator<<(std::ostream& output, Pressure pressure) {
    std::string unit_text;
    switch (pressure.GetPressure().second) 
    {
            case MU::Pa:
            unit_text = "Pa";
            break;
            case MU::kPa:
            unit_text = "kPa";
            break;
            case MU::bar:
            unit_text = "bar";
            break;
            case MU::MPa:
            unit_text = "MPa";
            break;
            case MU::kg_cm2:
            unit_text = "kg/cm2";
            break;
            case MU::m_H2O:
            unit_text = "m H2O";
            break;
            case MU::m_Hg:
            unit_text = "m Hg";
            break;    
    }
    output << pressure.GetPressure().first << " " << unit_text;
    return output;
}

std::istream& operator>>(std::istream& input, Pressure& pressure) {
    std:: cout << "please enter a pressure value: ";
    double pressure_value;
    MU m_unit;
    int unit_index;
    input >> pressure_value;
    std::cout << "Please enter measuring units for input (0 = Pa, 1 = kPa, 2 = bar, 3 = MPa, "
<< "4 = kg/cm2, 5 = meter H2O, 6 = mm Hg)" << std::endl;
    input >> unit_index;
    switch (unit_index)
    {
    case 0: m_unit = MU::Pa;
        break;
    case 1: m_unit = MU::kPa;
        break;
    case 2: m_unit = MU::bar;
        break;
    case 3: m_unit = MU::MPa;
        break;
    case 4: m_unit = MU::kg_cm2;
        break;
    case 5: m_unit = MU::m_H2O;
        break;
    case 6: m_unit = MU::m_Hg;
        break;
    default:
        break;
    }
    pressure = Pressure(pressure_value, m_unit);
    return input;
}

Pressure operator+(Pressure left, Pressure right) {
    return left+=right;
}

Pressure operator-(Pressure left, Pressure right) {
    return left-=right;
}

Pressure operator*(Pressure left, double right) {
    return left*=right;
}

Pressure operator/(Pressure left, double right) {
    return left /= right;
}

bool operator==(Pressure left, Pressure right) {
    return left.GetPressurePa()==right.GetPressurePa();
}

bool operator!=(Pressure left, Pressure right) {
    return !(left == right);
}
#pragma once
#include <utility>
#include <iostream>

enum class MU_head {m, ft};

class Head {
 public:
    Head() = default;
    
    Head(double value, MU_head index) : value_(value), unit_(index)
    {
        // write a definition of pa_value
        switch(index) {
            case MU_head::m:
            m_value_=value;
            break;
            case MU_head::ft:
            m_value_ = value*0.3048;
            break;
        }
    }

    ~Head() {}

    double GetHeadM() const {
        return m_value_;
    }

    std::pair<double, MU_head> GetHead() const {
        return {value_, unit_};
    }

    void ChangeHead(const double& value, const MU_head& unit) {
        value_= value;
        unit_ = unit;
        switch(unit) {
            case MU_head::m:
            m_value_=value;
            break;
            case MU_head::ft:
            m_value_ = value*0.3048;
            break;
        }
    }

    void ChangeUnits(const MU_head& unit) {
    switch(unit) {
            case MU_head::m:
            value_=m_value_;
            break;
            case MU_head::ft:
            value_=m_value_/0.3048;
            break;
        }
    unit_ = unit;
    }

    Head& operator+=(const Head& right) {
        m_value_ = m_value_ + right.GetHeadM();
        MU_head unit = std::min(unit_, right.GetHead().second); 
        this->ChangeUnits(unit);
        return *this;
    }

    Head& operator-=(const Head& right) {
        m_value_ = m_value_ - right.GetHeadM();
        MU_head unit = std::min(unit_, right.GetHead().second); 
        this->ChangeUnits(unit);
        return *this;
    }  

    Head& operator*=(const double& right) {
        m_value_ = m_value_ * right;
        value_ = value_ * right;
        return *this;
    }

    Head& operator/=(const double& right) {
        m_value_ = m_value_ / right;
        value_ = value_ / right;
        return *this;
    }

    private:
    double value_= 0.0;
    double m_value_=0.0;
    MU_head unit_ = MU_head::m;
};

std::ostream& operator<<(std::ostream& output, const Head& head) {
    std::string unit_text;
    switch (head.GetHead().second) 
    {
            case MU_head::m:
            unit_text = "m";
            break;
            case MU_head::ft:
            unit_text = "ft";
            break;  
    }
    output << head.GetHead().first << " " << unit_text;
    return output;
}

std::istream& operator>>(std::istream& input, Head& head) {
    std:: cout << "please enter a head value: ";
    double head_value;
    MU_head m_unit;
    int unit_index;
    input >> head_value;
    std::cout << "Please enter measuring units for input (0 = m, 1 = ft) " << std::endl;
    input >> unit_index;
    switch (unit_index)
    {
    case 0: m_unit = MU_head::m;
        break;
    case 1: m_unit = MU_head::ft;
        break;
    default:
        break;
    }
    head = Head(head_value, m_unit);
    return input;
}

Head operator+(Head left, Head right) {
    return left+=right;
}

Head operator-(Head left, Head right) {
    return left-=right;
}

Head operator*(Head left, double right) {
    return left*=right;
}

Head operator/(Head left, double right) {
    return left /= right;
}

bool operator==(const Head& left, const Head& right) {
    return left.GetHeadM()==right.GetHeadM();
}

bool operator!=(const Head& left, const Head& right) {
    return !(left == right);
}

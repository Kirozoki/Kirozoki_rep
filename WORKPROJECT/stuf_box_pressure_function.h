#pragma once
#include "pressure.h"

Pressure StufBoxPressureFunction(const Pressure& suction_pressure, double factor = 0.0, 
const Pressure& dif_pressure = Pressure(0.0, MU::Pa), const Pressure& added_pressure = Pressure(0.0, MU::Pa))
{
return (suction_pressure + added_pressure + dif_pressure * factor);
};
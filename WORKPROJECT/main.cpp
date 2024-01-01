#include <iostream>
#include <string>
#include "pressure.h"
#include "pump_group.h"

int main () {

Pressure suction_pressure_rated, suction_pressure_max;
std::cout << "Enter suction pressure rated: " << std::endl;
std::cin >> suction_pressure_rated;
std::cout << "Enter suction pressure max: " << std::endl;
std::cin >> suction_pressure_max;
std::cout << "Please enter a pump group from following list (i.e. integer number 1 or etc): "
<< "\nOverhung" 
<< "\nGroup #1 - SWP, CPP, CPO, IPP, CRP, SD, SK, SKo, SKV, PS, SHD, SO, GSD - Ps + 0.15*Pdiff"
<< "\nGroup #2 - SCE, SPI, SPN, IIL, IVP, (CWHC, Combitube &seal plan 11/21)- Ps + 0.10*Pdiff"
<< "\nGroup #3 - CWHC, Combitube (seal plan others) - Pseal = Ps" 
<< "\nGroup #4 - SCE-L - Ps + 0.70*Pdiff"
<< "\nBB"
<< "\nGroup #5 - RON &RON-D (balancing line), HVN, J, ZM, ZLM/ZMS, HSL, HSR, HSD, HSC, ZW & (1 Stage) - Pseal = Ps"
<< "\nGroup #6 - JTN, RON, RON-D (w/out balancing line), ZM, ZLM/ZMS, HSM (2 stage) - Pseal Suction Side=Ps, Pseal 2nd Stage=Ps+0.5*PDiff"
<< "\nGroup #7 - SM - Ps+2~3bar (both sides)"
<< "\nGroup #8 - A, AB, GP – shaft driven end =  Ps, non-driven end = Ps + 0.3bar"
<< "\nVertical"
<< "\nGroup #9 - VLT-Radial & balancing drum, VSP/ VSP-Chem (gas seal or seal plan >70), VLT-HP, or Cryo - Pseal=Ps"
<< "\nGroup #10 - VLT-Radial & no balancing drum, All others (VS1/VS6) & seal plan 02 II 23 - Pseal=Ps+Pdif"
<< "\nGroup #11 - VSP/ VSP-Chem (wet seal or seal plan 11,14), All others (VS1/VS6) & seal plan 14, 21, 31, 32 - Pseal=Ps+0.4*PDiff"
<< "\nGroup #12 - All others (VS1/VS6) & seal plan 11 or 13 - Pseal=Ps+0.3*PDiff" 
<< "\nPlunger pump"
<< "\nGroup #13 - RDP Pseal= Ps+1.1*Pdiff"
<< std::endl;
int index;
std::cin >> index;
PumpGroup pump_group(index, suction_pressure_rated, suction_pressure_max);
pump_group.GetData();
return 0;
}



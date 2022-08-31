#include "IFstream.H"
#include "OFstream.H"
#include "IOstream.H"


// 1. output time-voltage-Eeq curve
std::ofstream file_timeVoltage("time_voltage",ios_base::app);
file_timeVoltage << "time" << " " << "voltage"  << " " << "Eeq_avg" << std::endl;
file_timeVoltage.close();

// 2. output soc-voltage curve
std::ofstream file_SOC_voltage("SOC_voltage",ios_base::app);
file_SOC_voltage << "X_avg" << " " << "voltage" << std::endl;
file_SOC_voltage.close();




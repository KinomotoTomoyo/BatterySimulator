#include "IFstream.H"
#include "OFstream.H"
#include "IOstream.H"


// 1. output time-voltage curve
std::ofstream file_timeVoltage("time_voltage",ios_base::app);
file_timeVoltage << "time" << " " << "voltage" << " " << "theta_WE" << " " << "U_rev_WE" << " " << "eta_WE" << " " << "fai_s_WE" << " " << "fai_e_WE" << std::endl;
file_timeVoltage.close();

// 2. output Iapp at different boundaries and reactions
std::ofstream file_timeCurrent("time_current",ios_base::app);
file_timeCurrent << "time" << " " << "Iapp_WE_CC"  << " " << "Iapp_WEReact" << " " << "Iapp_REReact" << " " << "net_J_current" << std::endl;
file_timeCurrent.close();

// 3. output information at the RE
std::ofstream file_timeRE("time_RE",ios_base::app);
file_timeRE << "time" << " " << "Iapp_RE"  << " " << "fai_e_RE" << std::endl;
file_timeRE.close();

// 4. output time-species
std::ofstream file_timeSpecies("time_species",ios_base::app);
file_timeSpecies << "time" << " " << "Cs_WE"  << " " << "C_se_WE" << " " << "Ce_WE" << std::endl;
file_timeSpecies.close();

// 5. output time-flags
std::ofstream file_timeFlags("time_Flags",ios_base::app);
file_timeFlags << "time" << " " << "flag_SOC"  << " " << "flag_voltage" << std::endl;
file_timeFlags.close();





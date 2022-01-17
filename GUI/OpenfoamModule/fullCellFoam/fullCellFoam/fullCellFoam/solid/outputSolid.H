#include "IFstream.H"
#include "OFstream.H"
#include "IOstream.H"


scalar t = runTime.value();  //time index of the case

// 1. output time-voltage curve
std::ofstream file_timeVoltage("time_voltage",ios_base::app);
file_timeVoltage << t << " " << V << " " << theta_a << " " << theta_c << " " << U_rev_a << " " << U_rev_c << " " << eta_a << " " << eta_c  << " " << fai_s_a  << " " << fai_s_c  << " " << fai_e_a  << " " << fai_e_c << std::endl;
file_timeVoltage.close();

// 2. output Iapp at different boundaries and reactions
std::ofstream file_timeCurrent("time_current",ios_base::app);
file_timeCurrent << t << " " << Iapp_a_React  << " " << Iapp_c_React << std::endl;
file_timeCurrent.close();

// 3. output time-species
std::ofstream file_timeSpecies("time_species",ios_base::app);
file_timeSpecies << t << " " << Cs_a  << " " << C_se_a << " " << Ce_a << " " << Cs_c  << " " << C_se_c << " " << Ce_c << std::endl;
file_timeSpecies.close();

// 4. output time-flags
std::ofstream file_timeFlags("time_Flags",ios_base::app);
file_timeFlags << t << " " << flag_SOC_a  << " " << flag_SOC_c << " " << flag_voltage << std::endl;
file_timeFlags.close();


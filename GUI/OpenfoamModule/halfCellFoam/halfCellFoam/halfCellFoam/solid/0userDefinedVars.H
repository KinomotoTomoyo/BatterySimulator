scalarField i_current_RE; // current density at the interface of RE(Li Metal)/sep, A m-2
scalarField fai_e_RE; // ionic potential at the interface of RE(Li Metal)/sep, V
scalarField eta_RE; // overpotential at the interface of RE(Li Metal)/sep, V
scalarField Ce_RE; // electrolyte concentration at the interface of RE(Li Metal)/sep, mol m-3

scalar V             = 0; // output voltage, V
scalar fai_s_WE_CC   = 0;
scalar fai_s_WE      = 0; // averaged fai_s in the WE
scalar fai_e_WE      = 0; // averaged fai_e in the WE
scalar U_rev_WE      = 0; // averaged U_rev in the WE
scalar eta_WE        = 0; // averaged eta in the WE
scalar theta_WE      = 0; // averaged SOC in the WE
scalar Cs_WE         = 0; // averaged Cs in the WE
scalar Ce_WE         = 0; // averaged Ce in the WE
scalar C_se_WE        = 0; // averaged C_se in the WE

scalar Iapp_WEReact  = 0; // current density, A m-2
scalar Iapp_REReact  = 0; // current density, A m-2
scalar Iapp_WE_CC    = 0; // current density, A m-2
scalar net_J_current = 0; // calculate the net J_current in the whole cell

label  flag_SOC      = 0; // flag to check whether SOC exceeds its lower or higher limits: 0 for within the range; 1 for over-lithiated; -1 for over-delithiated
label  flag_voltage  = 0; // flag to check whether voltage is over 0V: 1 for voltage is lower than 0V
label  flag_init     = 0; // flag to check whether initialization finished: 1 for yes; 0 for no

label  count             = 0; // count the calculation loop times

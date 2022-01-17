
/* user-defined global variables */
scalarField Eeq_solid2ele; //
scalarField iRef_solid2ele; //
scalarField eta; // overpotential at the sruface, V
scalarField fai; // potential at the sruface, V

scalar V = 0; //output voltage
scalar X_avg; //Averaged state of charge for the whole particle
scalar Eeq_avg; //Averaged equilibrium potential at the surface 

label  flag_SOC      = 0; // flag to check whether SOC exceeds its lower or higher limits: 0 for within the range; 1 for over-lithiated; -1 for over-delithiated
label  flag_voltage  = 0; // flag to check whether voltage is over 0V: 1 for voltage is lower than 0V

label  count = 0; // count the calculation loop times

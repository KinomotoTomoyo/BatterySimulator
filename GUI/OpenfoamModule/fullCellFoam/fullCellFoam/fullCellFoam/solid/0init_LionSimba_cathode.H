
if (flag_init_c == 0) // if initialisation never happens,do
{
  Info<<"enter 0init_LionSimba_Cathode.H !"<<endl;

  if(solidRegions[i].name() == "cathode")
  {
    forAll(fai_s,celli)
    {
      C_se[celli] = Cs[celli];

      //state of charge in the solid phase
      theta[celli] = C_se[celli] / Cs_max_c.value();

      //equilibrium potential of solid phase
  //data source: Torchio M, Magni L, Gopaluni R B, et al. Lionsimba: a matlab framework based on a finite volume model suitable for li-ion battery design, simulation, and control[J]. Journal of The Electrochemical Society, 2016, 163(7): A1192.
      fai_e[celli] = -U_rev_a_init; // cathode fai_e_init = anode fai_e_init
      scalar U_rev_temp = (
                           - 4.656
                           + 88.669*Foam::pow(theta[celli],2)
                           - 401.119*Foam::pow(theta[celli],4)
                           + 342.909*Foam::pow(theta[celli],6)
                           - 462.471*Foam::pow(theta[celli],8)
                           + 433.434*Foam::pow(theta[celli],10)
                          )
                          / 
                          (
                           - 1 
                           + 18.933*Foam::pow(theta[celli],2)
                           - 79.532*Foam::pow(theta[celli],4)
                           + 37.311*Foam::pow(theta[celli],6)
                           - 73.083*Foam::pow(theta[celli],8) 
                           + 95.96*Foam::pow(theta[celli],10)
                          );
      fai_s[celli] = U_rev_temp - U_rev_a_init; // cathode fai_s-fai_e-Urev = 0
    }

    flag_init_c = 1;
    Info<< "Min/max initial theta in cathode:" << min(theta) << ' ' << max(theta) << endl;
    Info<< "Min/max initial fai_s in cathode:" << min(fai_s) << ' ' << max(fai_s) << endl;
    Info<< "Min/max initial fai_e in cathode:" << min(fai_e) << ' ' << max(fai_e) << endl;
  }

}

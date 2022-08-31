
if (flag_init_c == 0) // if initialisation never happens,do
{
  Info<<"enter 0init_LFP.H !"<<endl;

  if(solidRegions[i].name() == "cathode")
  {
    forAll(fai_s,celli)
    {
      C_se[celli] = Cs[celli];

      //state of charge in the solid phase
      theta[celli] = C_se[celli] / Cs_max_c.value();

      //equilibrium potential of solid phase
  //data source: https://www.dandeliion.com/simulation
      fai_e[celli]       = -U_rev_a_init; // cathode fai_e_init = anode fai_e_init
      scalar U_rev_temp  = 3.114559 + 4.438792 * Foam::atan(-71.7352 * theta[celli] + 70.85337) 
                         - 4.240252 * Foam::atan(-68.5605 * theta[celli] + 67.730082);
      fai_s[celli] = U_rev_temp - U_rev_a_init; // cathode fai_s-fai_e-Urev = 0
    }

    flag_init_c = 1;// once initialisation is done, flag needs to be toggled
    Info<< "Min/max initial theta in cathode:" << min(theta) << ' ' << max(theta) << endl;
    Info<< "Min/max initial fai_s in cathode:" << min(fai_s) << ' ' << max(fai_s) << endl;
    Info<< "Min/max initial fai_e in cathode:" << min(fai_e) << ' ' << max(fai_e) << endl;
  }

}

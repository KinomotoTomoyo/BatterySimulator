
if (flag_init_c == 0) // if initialisation never happens,do
{
  Info<<"enter 0init_NCA.H !"<<endl;

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
      scalar U_rev_temp  = -1.30202 * theta[celli] 
                         - 0.214712 * Foam::tanh(23.01 * (theta[celli] + 0.00350287)) 
                         + 2.45808 * Foam::tanh(2.90232 * (theta[celli] - 0.215657)) 
                         - 1.26644 * Foam::tanh(4.30574 * (theta[celli] - 0.329193)) 
                         - 0.40112 * Foam::tanh(9.07273 * (theta[celli] - 0.148644)) 
                         - 0.0532656 * Foam::exp(47.0417 * (theta[celli] - 0.95703)) 
                         + 4.17032;
      fai_s[celli] = U_rev_temp - U_rev_a_init; // cathode fai_s-fai_e-Urev = 0

    }

    flag_init_c = 1;
    Info<< "Min/max initial theta in cathode:" << min(theta) << ' ' << max(theta) << endl;
    Info<< "Min/max initial fai_s in cathode:" << min(fai_s) << ' ' << max(fai_s) << endl;
    Info<< "Min/max initial fai_e in cathode:" << min(fai_e) << ' ' << max(fai_e) << endl;
  }

}

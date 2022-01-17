
Info<<"enter OCV_NCA.H !"<<endl;

if(solidRegions[i].name() == "cathode")
{
  forAll(U_rev,celli)
  {

    //state of charge in the solid phase
    theta[celli] = C_se[celli] / Cs_max_c.value();

    //equilibrium potential of cathode material-NCA
    //data source: https://www.dandeliion.com/simulation
     U_rev[celli] = -1.30202 * theta[celli] 
                  - 0.214712 * Foam::tanh(23.01 * (theta[celli] + 0.00350287)) 
                  + 2.45808 * Foam::tanh(2.90232 * (theta[celli] - 0.215657)) 
                  - 1.26644 * Foam::tanh(4.30574 * (theta[celli] - 0.329193)) 
                  - 0.40112 * Foam::tanh(9.07273 * (theta[celli] - 0.148644)) 
                  - 0.0532656 * Foam::exp(47.0417 * (theta[celli] - 0.95703)) 
                  + 4.17032;

  }
  Info<< "Min/max theta in cathode:" << min(theta) << ' ' << max(theta) << endl;
  Info<< "Min/max U_rev in cathode:" << min(U_rev) << ' ' << max(U_rev) << endl;
}


U_rev.correctBoundaryConditions();

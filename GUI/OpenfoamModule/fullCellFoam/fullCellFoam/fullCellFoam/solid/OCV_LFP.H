
Info<<"enter OCV_LFP.H !"<<endl;

if(solidRegions[i].name() == "cathode")
{
  forAll(U_rev,celli)
  {

    //state of charge in the solid phase
    theta[celli] = C_se[celli] / Cs_max_c.value();

    //equilibrium potential of cathode material-LFP
//data source: https://www.dandeliion.com/simulation
     U_rev[celli] = 3.114559 + 4.438792 * Foam::atan(-71.7352 * theta[celli] + 70.85337) 
                  - 4.240252 * Foam::atan(-68.5605 * theta[celli] + 67.730082);

  }
  Info<< "Min/max theta in cathode:" << min(theta) << ' ' << max(theta) << endl;
  Info<< "Min/max U_rev in cathode:" << min(U_rev) << ' ' << max(U_rev) << endl;
}


U_rev.correctBoundaryConditions();

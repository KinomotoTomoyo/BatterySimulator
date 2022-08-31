
Info<<"enter OCV_LionSimba_cathode.H !"<<endl;

if(solidRegions[i].name() == "cathode")
{
  forAll(U_rev,celli)
  {

    //state of charge in the solid phase
    theta[celli] = C_se[celli] / Cs_max_c.value();

    //equilibrium potential of cathode material
//data source: Torchio M, Magni L, Gopaluni R B, et al. Lionsimba: a matlab framework based on a finite volume model suitable for li-ion battery design, simulation, and control[J]. Journal of The Electrochemical Society, 2016, 163(7): A1192.
     U_rev[celli] = (
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

  }
  Info<< "Min/max theta in cathode:" << min(theta) << ' ' << max(theta) << endl;
  Info<< "Min/max U_rev in cathode:" << min(U_rev) << ' ' << max(U_rev) << endl;
}


U_rev.correctBoundaryConditions();

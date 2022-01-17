

Info<<"enter OCV_Gr.H !"<<endl;

if(solidRegions[i].name() == "WE")
{
  forAll(U_rev,celli)
  {

    //state of charge in the solid phase
    theta[celli] = C_se[celli] / Cs_max_WE.value();

    //equilibrium potential of solid phase
//data source: Mercer M P, Peng C, Soares C, et al. Voltage hysteresis during lithiation/delithiation of graphite associated with meta-stable carbon stackings[J]. Journal of Materials Chemistry A, 2021, 9(1): 492-504.

    scalar p1 = -1.031044745141316e+03;
    scalar p2 = 4.705957629441347e+03;
    scalar p3 = -8.994928528706332e+03;
    scalar p4 = 9.321254530787901e+03;
    scalar p5 = -5.664051502639276e+03;
    scalar p6 = 2.039265768656875e+03;
    scalar p7 = -4.203985227846820e+02;
    scalar p8 = 46.673532522233998;
    scalar p9 = -3.026252647513182;
    scalar p10 = 0.315992994754579;

    U_rev[celli] = p1 * Foam::pow(theta[celli],9)
                 + p2 * Foam::pow(theta[celli],8)
                 + p3 * Foam::pow(theta[celli],7)
                 + p4 * Foam::pow(theta[celli],6)
                 + p5 * Foam::pow(theta[celli],5)
                 + p6 * Foam::pow(theta[celli],4)
                 + p7 * Foam::pow(theta[celli],3)
                 + p8 * Foam::pow(theta[celli],2)
                 + p9 * theta[celli]
                 + p10;

  }
  Info<< "Min/max theta in WE:" << min(theta) << ' ' << max(theta) << endl;
  Info<< "Min/max U_rev in WE:" << min(U_rev) << ' ' << max(U_rev) << endl;
}


U_rev.correctBoundaryConditions();

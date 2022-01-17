

Info<<"enter OCV_Si.H !"<<endl;

if(solidRegions[i].name() == "anode")
{
  forAll(U_rev,celli)
  {

    //state of charge in the Si
    theta[celli] = C_se[celli] / Cs_max_a.value();

    //equilibrium potential of Si
    // data source: Verbrugge M, Baker D, Xiao X. Formulation for the treatment of multiple electrochemical reactions and associated speciation for the Lithium-Silicon electrode[J]. Journal of The Electrochemical Society, 2015, 163(2): A262.
    scalar p1_lit = -96.63;
    scalar p2_lit = 372.6;
    scalar p3_lit = -587.6;
    scalar p4_lit = 489.9;
    scalar p5_lit = -232.8;
    scalar p6_lit = 62.99;
    scalar p7_lit = -9.286;
    scalar p8_lit = 0.8633;
    scalar U_rev_lit = p1_lit * Foam::pow(theta[celli],7)
                     + p2_lit * Foam::pow(theta[celli],6)
                     + p3_lit * Foam::pow(theta[celli],5)
                     + p4_lit * Foam::pow(theta[celli],4)
                     + p5_lit * Foam::pow(theta[celli],3)
                     + p6_lit * Foam::pow(theta[celli],2)
                     + p7_lit * theta[celli]
                     + p8_lit;


    scalar p1_delit = -51.02;
    scalar p2_delit = 161.3;
    scalar p3_delit = -205.7;
    scalar p4_delit = 140.2;
    scalar p5_delit = -58.76;
    scalar p6_delit =16.87;
    scalar p7_delit = -3.792;
    scalar p8_delit = 0.9937;
    scalar U_rev_delit = p1_delit * Foam::pow(theta[celli],7)
                       + p2_delit * Foam::pow(theta[celli],6)
                       + p3_delit * Foam::pow(theta[celli],5)
                       + p4_delit * Foam::pow(theta[celli],4)
                       + p5_delit * Foam::pow(theta[celli],3)
                       + p6_delit * Foam::pow(theta[celli],2)
                       + p7_delit * theta[celli]
                       + p8_delit;


    U_rev[celli] = U_rev_lit * (I_app.value() > 0) + U_rev_delit * (I_app.value() <= 0);

  }

  Info<< "Min/max theta in Si:" << min(theta) << ' ' << max(theta) << endl;
  Info<< "Min/max U_rev:" << min(U_rev) << ' ' << max(U_rev) << endl;
}


U_rev.correctBoundaryConditions();

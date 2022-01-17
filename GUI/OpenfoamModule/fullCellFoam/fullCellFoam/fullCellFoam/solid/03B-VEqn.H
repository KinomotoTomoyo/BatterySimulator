if(solidRegions[i].name() =="anode")
{
    forAll(J_current, celli)
   {
     eta[celli] = fai_s[celli] - fai_e[celli] - U_rev[celli];
     iRef[celli] = kReact_a.value()
                 * F.value()
                 * Foam::pow(Ce[celli], alphaA_a.value())
                 * Foam::pow((Cs_max_a.value() - C_se[celli]),alphaA_a.value())
                 * Foam::pow(C_se[celli], alphaC_a.value());
     i_current[celli] = iRef[celli]
                      * (
                           Foam::exp(alphaA_a.value() *F.value()/R.value()/T_temp[celli]*eta[celli])
                         - 
                           Foam::exp(-alphaC_a.value() *F.value()/R.value()/T_temp[celli]*eta[celli])
                        );
     J_current[celli] = spArea_a[celli] * i_current[celli];

   }

    Info<< "Min/max theta in anode:" << min(theta) << ' '<< max(theta) << endl;
    Info<< "Min/max eta in anode:" << min(eta) << ' '<< max(eta) << endl;
    Info<< "Min/max i_current in anode:" << min(i_current) << ' '<< max(i_current) << endl;
    Info<< "Min/max J_current in anode:" << min(J_current) << ' '<< max(J_current) << endl;

}  




if(solidRegions[i].name() =="cathode")
{
   forAll(J_current,celli)
   {
     eta[celli] = fai_s[celli] - fai_e[celli] - U_rev[celli];
     iRef[celli] = kReact_c.value()
                 * F.value()
                 * Foam::pow(Ce[celli],alphaA_c.value())
                 * Foam::pow((Cs_max_c.value() - C_se[celli]),alphaA_c.value())
                 * Foam::pow(C_se[celli],alphaC_c.value());

     i_current[celli] = iRef[celli]
                      * (
                           Foam::exp(alphaA_c.value() * F.value() / R.value() / T_temp[celli] * eta[celli])
                         - Foam::exp(-alphaC_c.value() * F.value() /R.value() / T_temp[celli] * eta[celli])
                        );
     J_current[celli] = spArea_c[celli] * i_current[celli];

   }

     Info<< "Min/max theta in cathode:" << min(theta) << ' '<< max(theta) << endl;
     Info<< "Min/max eta in cathode:" << min(eta) << ' '<< max(eta) << endl;
     Info<< "Min/max i_current in cathode:" << min(i_current) << ' '<< max(i_current) << endl; 
     Info<< "Min/max J_current in cathode:" << min(J_current) << ' '<< max(J_current) << endl;
    
}








 


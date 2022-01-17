Info<<"enter 02B-VEqn.H !"<<endl;

if(solidRegions[i].name() =="sep")
{
   //call for fai_e at the interface
   label ID_sep_RE = solidRegions[i].boundaryMesh().findPatchID("sep_RE");
   fai_e_RE        = fai_eSolid[i].boundaryFieldRef()[ID_sep_RE];
   Ce_RE           = CeSolid[i].boundaryFieldRef()[ID_sep_RE];
   eta_RE          = fai_e_RE; // initialize eta_RE to be the same size of fai_e_RE
   i_current_RE    = fai_e_RE; // initialize i_current_RE to be the same size of fai_e_RE

   // calculate the current density at the boundary sep_RE
   forAll(i_current_RE, facei)
   {
     eta_RE[facei] = fai_s_RE.value() - fai_e_RE[facei] - U_rev_RE.value();
     //i_current_RE[facei] = I_app.value();
     i_current_RE[facei] = kReact_RE.value() 
                         * F.value()
                         * Foam::pow(Ce_RE[facei], 0.5)
                         * (
                              Foam::exp(alphaA_RE.value()*F.value()/R.value()/T_temp_ref.value()*eta_RE[facei])
                            - 
                              Foam::exp(-alphaC_RE.value()*F.value()/R.value()/T_temp_ref.value()*eta_RE[facei])
                           );
   }

   i_e = -k_fai_e * fvc::grad(fai_e) - k_D_eff * fvc::grad(Ce);// calculate the electrolyte current density
   Info<< "Min/max i_current_RE in RE:" << min(i_current_RE) << ' '<< max(i_current_RE) << endl;
   Info<< "Min/max eta_RE in RE:" << min(eta_RE) << ' '<< max(eta_RE) << endl;
}

if(solidRegions[i].name() =="WE")
{
    forAll(J_current, celli)
   {
     eta[celli] = fai_s[celli] - fai_e[celli] - U_rev[celli];
     iRef[celli] = kReact_WE.value()
                 * F.value()
                 * Foam::pow(Ce[celli], alphaA_WE.value())
                 * Foam::pow((Cs_max_WE.value() - C_se[celli]),alphaA_WE.value())
                 * Foam::pow(C_se[celli], alphaC_WE.value());
     i_current[celli] = iRef[celli]
                      * (
                           Foam::exp(alphaA_WE.value() *F.value()/R.value()/T_temp[celli]*eta[celli])
                         - 
                           Foam::exp(-alphaC_WE.value() *F.value()/R.value()/T_temp[celli]*eta[celli])
                        );
     J_current[celli] = spArea_WE[celli] * i_current[celli];

   }

    i_e = -k_fai_e * fvc::grad(fai_e) - k_D_eff * fvc::grad(Ce);// calculate the electrolyte current density

Info<< "Min/max theta in WE:" << min(theta) << ' '<< max(theta) << endl;
Info<< "Min/max eta in WE:" << min(eta) << ' '<< max(eta) << endl;
Info<< "Min/max i_current in WE:" << min(i_current) << ' '<< max(i_current) << endl;
Info<< "Min/max J_current in WE:" << min(J_current) << ' '<< max(J_current) << endl;

}  









 


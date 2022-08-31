

Info<<"enter 01solid_set.H !"<<endl;

    if (solidRegions[i].name() == "anode")
    {
       forAll(fai_e,celli)
      {

          por_Ce[celli] = por_Ce_a.value();
          por_Cs[celli] = 1 - por_Ce[celli] - por_f_a.value();

         //coefficients for fai_e equations
          k_fai_e[celli] = -10.5 
                         + 0.668e-3 * Ce[celli] 
                         + 0.494e-6 * Ce[celli] * Ce[celli] 
                         + (0.074 - 1.78e-5 * Ce[celli] - 8.86e-10 * Ce[celli] * Ce[celli]) * T_temp[celli]
                         + (-6.96e-5 + 2.8e-8 * Ce[celli]) * T_temp[celli] * T_temp[celli];
          k_fai_e[celli] = Foam::pow(k_fai_e[celli],2) * Ce[celli] * 1.0e-4; // electrolyte conductivity, S/m
          k_fai_e[celli] = k_fai_e[celli] * Foam::pow(por_Ce[celli],brugg.value());
          k_D_eff[celli] = 2 * R.value() * T_temp[celli] * k_fai_e[celli] / F.value() * (tNo.value() - 1) * (1+0);//act_e=const,d(ln(act_e))/d(ln(Ce))=0
          k_D_eff[celli] = k_D_eff[celli] / Ce[celli];

         //coefficients for fai_s equations
          k_fai_s[celli] = k0_fai_s_a.value() * por_Cs[celli];

         //coefficients for Ce equations
          D_Ce[celli] = D0_Ce.value() * Foam::pow(por_Ce[celli],brugg.value());


          k_T_temp[celli] = 1e-4;


          S_Ce[celli] = (1-tNo.value()) * J_current[celli] / F.value();
          S_Cs[celli] = -J_current[celli] / F.value();
          S_T_temp[celli] = 0;

          spArea_a[celli] = 3 * por_Cs[celli] / r_s_a.value();
          C_se[celli] = Cs[celli] - J_current[celli] / spArea_a[celli] / F.value() * r_s_a.value()/5 / Ds_a.value();

          if (C_se[celli] <= 0)
          {
               Info<<"C_se is"<<C_se[celli]<<endl;
               C_se[celli] = 100;
               Info<<"anode C_se too low !"<<endl;
          }

          if (C_se[celli] > Cs_max_a.value())
          {
               Info<<"C_se is"<<C_se[celli]<<endl;
               C_se[celli] = 0.99 * Cs_max_a.value();
               Info<<"anode C_se too high !"<<endl;
          }

      }
      Info<<"anode done !"<<endl;
    }
   else if (solidRegions[i].name() == "cathode")
    {
       forAll(fai_e,celli)
      {
          por_Ce[celli] = por_Ce_c.value();
          por_Cs[celli] = 1 - por_Ce[celli] - por_f_c.value();

         //coefficients for fai_e equations
          k_fai_e[celli] = -10.5 
                         + 0.668e-3 * Ce[celli] 
                         + 0.494e-6 * Ce[celli] * Ce[celli] 
                         + (0.074 - 1.78e-5 * Ce[celli] - 8.86e-10 * Ce[celli] * Ce[celli]) * T_temp[celli]
                         + (-6.96e-5 + 2.8e-8 * Ce[celli]) * T_temp[celli] * T_temp[celli];
          k_fai_e[celli] = Foam::pow(k_fai_e[celli],2) * Ce[celli] * 1.0e-4; // electrolyte conductivity, S/m
          k_fai_e[celli] = k_fai_e[celli] * Foam::pow(por_Ce[celli],brugg.value());
          //k_fai_e[celli] = 0.03;//1.0
          k_D_eff[celli] = 2 * R.value() * T_temp[celli] * k_fai_e[celli] / F.value() * (tNo.value() - 1) * (1+0);//act_e=const,d(ln(act_e))/d(ln(Ce))=0
          k_D_eff[celli] = k_D_eff[celli] / Ce[celli];

         //coefficients for fai_s equations
          k_fai_s[celli] = k0_fai_s_c.value() * por_Cs[celli];

         //coefficients for Ce equations
          D_Ce[celli] = D0_Ce.value() * Foam::pow(por_Ce[celli],brugg.value());


          k_T_temp[celli] = 1e-4;

          S_Ce[celli] = (1-tNo.value()) * J_current[celli] / F.value();
          S_Cs[celli] = -J_current[celli] / F.value();
          S_T_temp[celli] = 0;

          spArea_c[celli] = 3 * por_Cs[celli] / r_s_c.value();
          C_se[celli] = Cs[celli] - J_current[celli] / spArea_c[celli] / F.value() * r_s_c.value()/5 / Ds_c.value();

          if (C_se[celli] <= 0.427 * Cs_max_c.value())
          {
               Info<<"C_se is"<<C_se[celli]<<endl;
               C_se[celli] = 0.427 * Cs_max_c.value();
               Info<<"cathode C_se too low !"<<endl;
          }

          if (C_se[celli] > Cs_max_c.value())
          {
               Info<<"C_se is"<<C_se[celli]<<endl;
               C_se[celli] = 0.99 * Cs_max_c.value();
               Info<<"cathode C_se too high !"<<endl;
          }

      }

      Info<<"cathode done !"<<endl;
    }
   else if (solidRegions[i].name() == "seperator")
    {
       forAll(fai_e,celli)
      {
          por_Ce[celli] = por_Ce_sp.value();
          por_Cs[celli] = 1; //there is no diffusion of solid-phase in the seperator, but should assign por_Cs to be 1 to avoid 0/0 in CsSolidEqn

         //coefficients for fai_e equations
          k_fai_e[celli] = -10.5 
                         + 0.668e-3 * Ce[celli] 
                         + 0.494e-6 * Ce[celli] * Ce[celli] 
                         + (0.074 - 1.78e-5 * Ce[celli] - 8.86e-10 * Ce[celli] * Ce[celli]) * T_temp[celli]
                         + (-6.96e-5 + 2.8e-8 * Ce[celli]) * T_temp[celli] * T_temp[celli];
          k_fai_e[celli] = Foam::pow(k_fai_e[celli],2) * Ce[celli] * 1.0e-4; // electrolyte conductivity, S/m
          k_fai_e[celli] = k_fai_e[celli] * Foam::pow(por_Ce[celli],brugg.value());
          //k_fai_e[celli] = 0.03;//1.0
          k_D_eff[celli] = 2 * R.value() * T_temp[celli] * k_fai_e[celli] / F.value() * (tNo.value() - 1) * (1+0);//act_e=const,d(ln(act_e))/d(ln(Ce))=0
          k_D_eff[celli] = k_D_eff[celli] / Ce[celli];

         //coefficients for fai_s equations
          k_fai_s[celli] = 0;

         //coefficients for Ce equations
          D_Ce[celli] = D0_Ce.value() * Foam::pow(por_Ce[celli],brugg.value());


          k_T_temp[celli] = 1e-4;

          S_Ce[celli] = 0;
          S_T_temp[celli] = 0;

      }

      Info<<"seperator done !"<<endl;
    }

k_fai_e.correctBoundaryConditions();
k_D_eff.correctBoundaryConditions();
k_fai_s.correctBoundaryConditions();
D_Ce.correctBoundaryConditions();
por_Ce.correctBoundaryConditions();
por_Cs.correctBoundaryConditions();
k_T_temp.correctBoundaryConditions();


  if (runTime.outputTime())
  {
   
  }

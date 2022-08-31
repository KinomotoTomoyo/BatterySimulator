
Info<<"enter 03modify_boundary.H !"<<endl;

if(solidRegions[i].name() =="sep")
{
  label ID_sep_RE = solidRegions[i].boundaryMesh().findPatchID("sep_RE");

  fixedGradientFvPatchScalarField& fai_e_sep_RE = refCast<fixedGradientFvPatchScalarField>(fai_e.boundaryFieldRef()[ID_sep_RE]);
  fixedGradientFvPatchScalarField& Ce_sep_RE    = refCast<fixedGradientFvPatchScalarField>(Ce.boundaryFieldRef()[ID_sep_RE]);

  scalarField& fai_e_sep_RE_grad = fai_e_sep_RE.gradient();
  scalarField& Ce_sep_RE_grad    = Ce_sep_RE.gradient();

  volVectorField Ce_grad(fvc::grad(Ce)); // calculate the gradient of Ce over the whole domain
  volScalarField CeX_grad = Ce_grad.component(vector::X); // the gradient component towards x direction
  volVectorField fai_e_grad(fvc::grad(fai_e)); // calculate the gradient of fai_e over the whole domain
  volScalarField fai_eX_grad = fai_e_grad.component(vector::X); // the gradient component towards x direction

  // Call for parameter values at the boundary "sep_RE"
  scalarField k_fai_e_sep_RE         = k_fai_e.boundaryFieldRef()[ID_sep_RE]; // sep_RE boundary values of k_fai_e 
  scalarField k_D_eff_sep_RE         = k_D_eff.boundaryFieldRef()[ID_sep_RE]; // sep_RE boundary values of k_D_eff 
  scalarField Ce_sep_RE_gradValue    = CeX_grad.boundaryFieldRef()[ID_sep_RE];// sep_RE boundary values of laplacian(Ce)
  scalarField D_Ce_sep_RE            = D_Ce.boundaryFieldRef()[ID_sep_RE]; // sep_RE boundary values of D_Ce 
  scalarField fai_e_sep_RE_gradValue = fai_eX_grad.boundaryFieldRef()[ID_sep_RE];// sep_RE boundary values of laplacian(fai_e) 

  forAll(fai_e_sep_RE_grad, faceI)
  {
     fai_e_sep_RE_grad[faceI] = -(-i_current_RE[faceI] + k_D_eff_sep_RE[faceI] * Ce_sep_RE_gradValue[faceI]) / k_fai_e_sep_RE[faceI];// flux in for disch
  }

  forAll(Ce_sep_RE_grad, faceI)
  {
     scalar coeff_Ce = D_Ce_sep_RE[faceI] + k_D_eff_sep_RE[faceI] * tNo.value() / F.value();
     Ce_sep_RE_grad[faceI] = -(  -i_current_RE[faceI]/F.value() 
                             + 
                                k_fai_e_sep_RE[faceI]*tNo.value()/F.value()*fai_e_sep_RE_gradValue[faceI]
                             )
                           /coeff_Ce;// flux in for disch

     //Ce_sep_RE_grad[faceI] = -(  -I_app.value()/F.value() 
     //                         + 
     //                           k_fai_e_sep_RE[faceI]*tNo.value()/F.value()*fai_e_sep_RE_gradValue[faceI]
     //                        )
     //                      /coeff_Ce;// flux in for disch


     //Ce_sep_RE_grad[faceI] = (1 - tNo.value())*i_current_RE[faceI]/F.value();// flux in for disch
  }

}

if(solidRegions[i].name() =="WE")
{
  label ID_WE_CC = solidRegions[i].boundaryMesh().findPatchID("WE_CC");
  fixedGradientFvPatchScalarField& fai_s_WE_CC = refCast<fixedGradientFvPatchScalarField>(fai_s.boundaryFieldRef()[ID_WE_CC]);

  scalarField& fai_s_WE_CC_grad = fai_s_WE_CC.gradient();

  scalarField k_fai_s_WE_CC = k_fai_s.boundaryFieldRef()[ID_WE_CC]; // WE_CC boundary values of k_fai_s 

  forAll(fai_s_WE_CC_grad, faceI)
  {
     fai_s_WE_CC_grad[faceI] = -I_app.value()/k_fai_s_WE_CC[faceI];// flux out for disch
  }
}

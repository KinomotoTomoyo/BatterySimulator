
Info << "enter 03modify_boundary" << endl;

if(solidRegions[i].name() =="solidPhase")
{
  /* Call for the boundary ID in solid phase */
  label ID_solid2ele = solidRegions[i].boundaryMesh().findPatchID("solidPhase_to_ele");

  /* Call for the boundary variables at the particle surface */
  scalarField Ds_int                      = Ds.boundaryFieldRef()[ID_solid2ele];
  fixedGradientFvPatchScalarField& Cs_int = refCast<fixedGradientFvPatchScalarField>(Cs.boundaryFieldRef()[ID_solid2ele]);

  /* Call for the boundary variable gradients in anode_AM */
  scalarField& Cs_int_grad = Cs_int.gradient();


  forAll(Cs_int_grad, faceI)// solidPhase_to_ele, flux = i_current/F/Ds, flux in for disch
  {
     Cs_int_grad[faceI] = I_app.value()/Ds_int[faceI]/F.value();// disch-flux in, i_current>0
  }

}



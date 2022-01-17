
if(solidRegions[i].name() =="cathode")
{
  label patchID_c = solidRegions[i].boundaryMesh().findPatchID("cathodeCC");

  fixedGradientFvPatchScalarField& fai_s_c_patch = refCast<fixedGradientFvPatchScalarField>(fai_s.boundaryFieldRef()[patchID_c]);

  scalarField& gradpatch_fai_s_c = fai_s_c_patch.gradient();
  

  forAll(gradpatch_fai_s_c, faceI)
  {
     // Perform calculations to determine the new gradient "newGrad"
     // Store the new value in the scalar field
     gradpatch_fai_s_c[faceI] = -I_app.value()/(k0_fai_s_c.value() * (1 -por_Ce_c.value() - por_f_c.value()));
  }
}

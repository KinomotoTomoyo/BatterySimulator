// 1. extract the open circuit voltage (equilibrium potential) of solid phase at the surface: Eeq = f(X)
// 2. extract the referece current density at the surface
// 3. calculate the overpotential at the surface, eta = f(i_current), i_current = Iapp = cRate * cap_tot
// output voltage, V = fai = Eeq - eta


Info << "enter 02B-VEqn.H" << endl;

if(solidRegions[i].name() =="solidPhase")
{
// extract the value of X at the surface
label ID_solid2ele = solidRegions[i].boundaryMesh().findPatchID("solidPhase_to_ele");
Eeq_solid2ele      = Eeq.boundaryFieldRef()[ID_solid2ele];
iRef_solid2ele     = iRef.boundaryFieldRef()[ID_solid2ele];

eta = Eeq_solid2ele; //initialize eta
fai = Eeq_solid2ele; //initialize fai

forAll(eta, faceI)
{
  scalar log_term   = I_app.value() / 2 / iRef_solid2ele[faceI];
  eta[faceI] = 2 * R.value() * T_temp.value() / F.value()
                    * Foam::log(log_term + Foam::sqrt(Foam::pow(log_term,2)+1));

  fai[faceI] = Eeq_solid2ele[faceI] - eta[faceI];
}

// calculate the averaged output voltage
  const scalarField& A = mesh.magSf().boundaryField()[ID_solid2ele];// patch of mesh area at the particle surface
                     V = sum(fai * A) / sum(A);

// calculate the averaged state of charge
   const scalarField& Volume = mesh.V();// patch of volume of the solid phase
   X_avg = sum(X * Volume) / sum(Volume);

// calculate the averaged equilibrium potential at the surface
   Eeq_avg = sum(Eeq_solid2ele * A) / sum(A);
 
}




 


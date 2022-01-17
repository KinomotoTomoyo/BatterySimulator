

Info<<"enter 01solid_set.H !"<<endl;

if(solidRegions[i].name() == "solidPhase")
{
  forAll(Cs,celli)
  {
    //assign the value to Ds
    Ds[celli] = Ds_value.value();

    // reference current density
     iRef[celli] = kReact.value()
                 * F.value()
                 * Foam::pow(Ce.value(), alphaA.value())
                 * Foam::pow((Cs_max.value() - Cs[celli]),alphaA.value())
                 * Foam::pow(Cs[celli], alphaC.value());

  }
  Info<< "Min/max X in solid phase:" << min(X) << ' ' << max(X) << endl;
  Info<< "Min/max Eeq in solid phase:" << min(Eeq) << ' ' << max(Eeq) << endl;
  Info<< "Min/max iRef in solid phase:" << min(iRef) << ' ' << max(iRef) << endl;
  Info<<"solid phase region done !"<<endl;

}

//update the boundary values of all the variables
Ds.correctBoundaryConditions();
X.correctBoundaryConditions();
iRef.correctBoundaryConditions();
Cs.correctBoundaryConditions();


  if (runTime.outputTime())
  {
   
  }

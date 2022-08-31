
Info<<"enter 07CsSolidsEqn.H !"<<endl;

if (solidRegions[i].name() == "WE")
{
   
    fvScalarMatrix CsEqn
    (
       fvm::ddt(por_Cs, Cs)
     == S_Cs
                          
    );


    CsEqn.relax();

    CsEqn.solve(mesh.solver(Cs.select(pimples.finalIter())));


Info<< "Min/max Cs:" << min(Cs) << ' '
    << max(Cs) << endl;

}

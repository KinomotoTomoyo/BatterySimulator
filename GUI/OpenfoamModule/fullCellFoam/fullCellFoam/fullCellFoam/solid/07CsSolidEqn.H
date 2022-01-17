
if(solidRegions[i].name() == "anode" || solidRegions[i].name() == "cathode")
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

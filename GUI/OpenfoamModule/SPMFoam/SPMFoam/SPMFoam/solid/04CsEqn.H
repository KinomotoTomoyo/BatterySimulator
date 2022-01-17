
Info << "enter 04CsEqn.H" << endl;

if(solidRegions[i].name() == "solidPhase")
{

    fvScalarMatrix CsEqn
    (
       fvm::ddt(1, Cs)
     - fvm::laplacian(Ds, Cs)
                          
    );


    CsEqn.relax();
    CsEqn.solve(mesh.solver(Cs.select(pimples.finalIter())));


Info<< "Min/max Cs in solidPhase:" << min(Cs) << ' '
    << max(Cs) << endl;

}

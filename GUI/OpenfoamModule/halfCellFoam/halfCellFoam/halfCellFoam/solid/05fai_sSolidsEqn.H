

Info<<"enter 05fai_sSolidsEqn.H !"<<endl;

if (solidRegions[i].name() == "WE")
{
    fvScalarMatrix fai_sEqn
    (
       //fvm::ddt(1.0e-2,fai_s)
     - fvm::laplacian(k_fai_s, fai_s)
     == -J_current
                          
    );


    fai_sEqn.relax();

    fai_sEqn.solve(mesh.solver(fai_s.select(pimples.finalIter())));


Info<< "Min/max fai_s:" << min(fai_s) << ' '
    << max(fai_s) << endl;

}

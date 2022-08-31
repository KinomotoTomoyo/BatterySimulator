
fvScalarMatrix fai_eEqn
    (
       //fvm::ddt(1.0,fai_e)
     - fvm::laplacian(k_fai_e, fai_e)
     - fvc::laplacian(k_D_eff,Ce)
     == J_current
                          
    );


    fai_eEqn.relax();

    fai_eEqn.solve(mesh.solver(fai_e.select(pimples.finalIter())));


Info<< "Min/max fai_e:" << min(fai_e) << ' '
    << max(fai_e) << endl;



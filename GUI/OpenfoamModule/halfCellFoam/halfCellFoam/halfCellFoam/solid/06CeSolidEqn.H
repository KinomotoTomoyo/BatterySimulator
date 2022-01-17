
Info<<"enter 06CeSolidsEqn.H !"<<endl;

// Mass transport of Li+ in the electrolyte
fvScalarMatrix CeEqn
    (
       fvm::ddt(por_Ce, Ce)
     - fvm::laplacian(D_Ce, Ce)
     == S_Ce
                          
    );


    CeEqn.relax();

    CeEqn.solve(mesh.solver(Ce.select(pimples.finalIter())));


Info<< "Min/max Ce:" << min(Ce) << ' '
    << max(Ce) << endl;




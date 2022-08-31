PtrList<volScalarField> fai_eSolid(solidRegions.size());   //ionic potential,V
PtrList<volScalarField> fai_sSolid(solidRegions.size());   //electronic potential,V
PtrList<volScalarField> k_fai_eSolid(solidRegions.size());   //ionic conductivity,S m-1
PtrList<volScalarField> k_fai_sSolid(solidRegions.size());   //electronic conductivity,S m-1
PtrList<volScalarField> J_currentSolid(solidRegions.size());   //Source terms of ionic and electronic potential equation, A m-3

PtrList<volScalarField> thetaSolid(solidRegions.size());   //state of charge at the surface of the electrode particles
PtrList<volScalarField> SOCSolid(solidRegions.size());   //state of charge in the electrode
PtrList<volScalarField> C_seSolid(solidRegions.size());   //Li concentration at the interface of solid electrode and electrolyte,mol m-3
PtrList<volScalarField> U_revSolid(solidRegions.size());   //reversible voltage,V
PtrList<volScalarField> etaSolid(solidRegions.size());   //over-potential,V
PtrList<volScalarField> iRefSolid(solidRegions.size());   //reference exchange current density,A m-2
PtrList<volScalarField> i_currentSolid(solidRegions.size());   //current density,A m-2
PtrList<volScalarField> T_tempSolid(solidRegions.size());   //user-defined temperature, K
PtrList<volScalarField> CeSolid(solidRegions.size());   //Li+ concentration in the electrolyte, mol m-3
PtrList<volScalarField> k_T_tempSolid(solidRegions.size());   //thermal conductivity of user-fined temperature, W m-1 K-1
PtrList<volScalarField> D_CeSolid(solidRegions.size());   //diffusion coefficient of Ce,m2 s-1
PtrList<volScalarField> S_CeSolid(solidRegions.size());   //source term of Ce,mol m-3
PtrList<volScalarField> S_T_tempSolid(solidRegions.size());   //source term of S_T_temp,W m-3
PtrList<volScalarField> CsSolid(solidRegions.size());   //Li+ concentration in the granular particles, mol m-3
PtrList<volScalarField> S_CsSolid(solidRegions.size());   //source term of Cs, mol m-3 s-1
PtrList<volScalarField> por_CeSolid(solidRegions.size());   //porosity (proportion of electrolyte) por_Ce
PtrList<volScalarField> por_CsSolid(solidRegions.size());   //solid-phase fraction por_Cs
PtrList<volScalarField> k_D_effSolid(solidRegions.size());   //diffusional conductivity k_D_eff = k_D_eff/Ce, (A m-1)/(mol m-3) = A m2 mol-1
PtrList<volScalarField> spArea_WESolid(solidRegions.size());   //reaction surface area per volume WE, m-1
PtrList<volVectorField> i_eSolid(solidRegions.size());   //electrolyte current density, A m-2



PtrList<dimensionedScalar> por_Ce_WESolid(solidRegions.size()); //porosity in WE
PtrList<dimensionedScalar> por_Ce_spSolid(solidRegions.size()); //porosity in seperator
PtrList<dimensionedScalar> por_f_WESolid(solidRegions.size()); //filler fraction in WE
PtrList<dimensionedScalar> Cs_max_WESolid(solidRegions.size()); //maximum li concentration in WE materials, mol m-3
PtrList<dimensionedScalar> bruggSolid(solidRegions.size()); //bruggman coefficient
PtrList<dimensionedScalar> RSolid(solidRegions.size()); //universal gas constant, J mol-1 K-1
PtrList<dimensionedScalar> FSolid(solidRegions.size()); //Faraday constant, C mol-1
PtrList<dimensionedScalar> tNoSolid(solidRegions.size());   //transference number
PtrList<dimensionedScalar> k0_fai_s_WESolid(solidRegions.size()); //intrinsic solid-phase conductivity in WE,S m-1
PtrList<dimensionedScalar> D0_CeSolid(solidRegions.size()); //intrinsic electrolyte diffusion coefficient,m2 s-1
PtrList<dimensionedScalar> r_sSolid(solidRegions.size());   //particle radius, m
PtrList<dimensionedScalar> Ds_WESolid(solidRegions.size());   //diffusivity of Li in solid-phase of WE, m2 s-1
PtrList<dimensionedScalar> kReact_WESolid(solidRegions.size()); //reaction rate constant in WE, m2.5 mol-0.5 s-1
PtrList<dimensionedScalar> kReact_RESolid(solidRegions.size()); //reaction rate constant in RE, m2.5 mol-0.5 s-1
PtrList<dimensionedScalar> alphaA_WESolid(solidRegions.size()); //anodic transference coefficient in the WE
PtrList<dimensionedScalar> alphaC_WESolid(solidRegions.size()); //cathodic transference coefficient in the WE
PtrList<dimensionedScalar> alphaA_RESolid(solidRegions.size()); //anodic transference coefficient in the RE
PtrList<dimensionedScalar> alphaC_RESolid(solidRegions.size()); //cathodic transference coefficient in the RE
PtrList<dimensionedScalar> fai_s_RESolid(solidRegions.size()); //electronic potential in the WE,V
PtrList<dimensionedScalar> U_rev_RESolid(solidRegions.size()); //equilibrium potential in the WE, V
PtrList<dimensionedScalar> iRef_RESolid(solidRegions.size()); //reference current density in the WE, A m-2
PtrList<dimensionedScalar> T_temp_refSolid(solidRegions.size()); //reference temperature, K
PtrList<dimensionedScalar> I_appSolid(solidRegions.size()); //applied current density, A m-2


forAll(solidRegions, i)
{
//V1    ionic potential 
Info<< " Adding to fai_eSolid\n" << endl;
   fai_eSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "fai_e",
             runTime.timeName(),
             solidRegions[i],
             IOobject::MUST_READ,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i]
        )
    );

//V2   electronic potential 
Info<< " Adding to fai_sSolid\n" << endl;
   fai_sSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "fai_s",
             runTime.timeName(),
             solidRegions[i],
             IOobject::MUST_READ,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i]
        )
    );

//V3    ionic conductivity
Info<< " Adding to k_fai_e\n" << endl;
   k_fai_eSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "k_fai_e",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("k_fai_e", dimless, 0.1),
            zeroGradientFvPatchScalarField::typeName
       )
    );

//V4    electronic conductivity
Info<< " Adding to k_fai_s\n" << endl;
   k_fai_sSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "k_fai_s",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("k_fai_s", dimless, 100),
            zeroGradientFvPatchScalarField::typeName
       )
    );

//V4    Source term of fai_e and fai_s, decide plus/minus in the Eqn later
Info<< " Adding to J_current\n" << endl;
   J_currentSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "J_current",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("J_current", dimless, 0),
            zeroGradientFvPatchScalarField::typeName
       )
    );

//V5    state of charge in electrodes
Info<< " Adding to theta\n" << endl;
   thetaSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "theta",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("theta", dimless, 0),
            zeroGradientFvPatchScalarField::typeName
       )
    );

//V6    Li concentration at the interface of solid electrode and electrolyte
Info<< " Adding to C_se\n" << endl;
   C_seSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "C_se",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("C_se", dimless, 0),
            zeroGradientFvPatchScalarField::typeName
       )
    );

//V7    reversible voltage
Info<< " Adding to U_rev\n" << endl;
   U_revSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "U_rev",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("U_rev", dimless, 0),
            zeroGradientFvPatchScalarField::typeName
       )
    );

//V8    overpotential
Info<< " Adding to eta\n" << endl;
   etaSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "eta",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("eta", dimless, 0),
            zeroGradientFvPatchScalarField::typeName
       )
    );

//V9    reference exchange current density
Info<< " Adding to iRef\n" << endl;
   iRefSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "iRef",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("iRef", dimless, 0),
            zeroGradientFvPatchScalarField::typeName
       )
    );

//V10   current density
Info<< " Adding to i_current\n" << endl;
   i_currentSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "i_current",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("i_current", dimless, 0),
            zeroGradientFvPatchScalarField::typeName
       )
    );

//V11   user-defined temperature
Info<< " Adding to T_tempSolid\n" << endl;
   T_tempSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "T_temp",
             runTime.timeName(),
             solidRegions[i],
             IOobject::MUST_READ,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i]
        )
    );

//V12    Li+ concentration in the electrolyte
Info<< " Adding to CeSolid\n" << endl;
   CeSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "Ce",
             runTime.timeName(),
             solidRegions[i],
             IOobject::MUST_READ,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i]
        )
    );

//V13   thermal conductivity
Info<< " Adding to k_T_temp\n" << endl;
   k_T_tempSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "k_T_temp",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("k_T_temp", dimless, 0),
            zeroGradientFvPatchScalarField::typeName
       )
    );

//V14   diffusion coefficient of Ce
Info<< " Adding to D_Ce\n" << endl;
   D_CeSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "D_Ce",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("D_Ce", dimless, 1e-10),
            zeroGradientFvPatchScalarField::typeName
       )
    );

//V15   source term of Ce
Info<< " Adding to S_Ce\n" << endl;
   S_CeSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "S_Ce",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("S_Ce", dimless, 0),
            zeroGradientFvPatchScalarField::typeName
       )
    );


//V16   source term of T_temp
Info<< " Adding to S_T_temp\n" << endl;
   S_T_tempSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "S_T_temp",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("S_T_temp", dimless, 0),
            zeroGradientFvPatchScalarField::typeName
       )
    );

//V17    Li+ concentration in the granular particles  
Info<< " Adding to Cs\n" << endl;
   CsSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "Cs",
             runTime.timeName(),
             solidRegions[i],
             IOobject::MUST_READ,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i]
        )
    );

//V18   source term of S_Cs
Info<< " Adding to S_Cs\n" << endl;
   S_CsSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "S_Cs",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("S_Cs", dimless, 0),
            zeroGradientFvPatchScalarField::typeName
       )
    );

//V19   porosity por_Ce
Info<< " Adding to por_Ce\n" << endl;
   por_CeSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "por_Ce",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("por_Ce", dimless, 0),
            zeroGradientFvPatchScalarField::typeName
       )
    );

//V20   solid-phase fraction por_Cs
Info<< " Adding to por_Cs\n" << endl;
   por_CsSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "por_Cs",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("por_Cs", dimless, 0),
            zeroGradientFvPatchScalarField::typeName
       )
    );

//V21   diffusional conductivity k_D_eff
Info<< " Adding to k_D_eff\n" << endl;
   k_D_effSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "k_D_eff",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("k_D_eff", dimless, -1e-7),// it is of the order of the magnitude of -1e-7
            zeroGradientFvPatchScalarField::typeName
       )
    );

//V22   specific area spArea_WE
Info<< " Adding to spArea_WE\n" << endl;
   spArea_WESolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "spArea_WE",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("spArea_WE", dimless, 723600),// it is of the order of the magnitude of 723600
            zeroGradientFvPatchScalarField::typeName
       )
    );

//V23   current density in the solution, electrolyte current density
Info<< " Adding to i_e\n" << endl;
   i_eSolid.set
   (
        i,
        new volVectorField
       (
            IOobject
           (
             "i_e",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedVector("i_e", dimless, vector (0, 0, 0)),
            fixedValueFvPatchVectorField::typeName
       )
    );


Info<< "    Adding transport properties to  Solid field\n" << endl;   
Info<< "Reading transportProperties\n" << endl;

         IOdictionary LiProperties
        (
             IOobject
           (
              "LiProperties",
              runTime.constant(),
              solidRegions[i],
              IOobject::MUST_READ_IF_MODIFIED,
              IOobject::NO_WRITE
           )
         );


Info<< " Reading por_Ce_WE\n" << endl;
        por_Ce_WESolid.set (i, new dimensionedScalar (LiProperties.lookup("por_Ce_WE")));

Info<< " Reading por_Ce_sp\n" << endl;
        por_Ce_spSolid.set (i, new dimensionedScalar (LiProperties.lookup("por_Ce_sp")));

Info<< " Reading por_f_WE\n" << endl;
        por_f_WESolid.set (i, new dimensionedScalar (LiProperties.lookup("por_f_WE")));

Info<< " Reading Cs_max_WE\n" << endl;
        Cs_max_WESolid.set (i, new dimensionedScalar (LiProperties.lookup("Cs_max_WE")));

Info<< " Reading brugg\n" << endl;
       bruggSolid.set (i, new dimensionedScalar (LiProperties.lookup("brugg")));

Info<< " Reading F\n" << endl;
        FSolid.set (i, new dimensionedScalar (LiProperties.lookup("F")));

Info<< " Reading R\n" << endl;
        RSolid.set (i, new dimensionedScalar (LiProperties.lookup("R")));

Info<< " Reading tNo\n" << endl;
        tNoSolid.set (i, new dimensionedScalar (LiProperties.lookup("tNo")));

Info<< " Reading k0_fai_s_WE\n" << endl;
       k0_fai_s_WESolid.set (i, new dimensionedScalar (LiProperties.lookup("k0_fai_s_WE")));

Info<< " Reading D0_Ce\n" << endl;
       D0_CeSolid.set (i, new dimensionedScalar (LiProperties.lookup("D0_Ce")));

Info<< " Reading r_s\n" << endl;
        r_sSolid.set (i, new dimensionedScalar (LiProperties.lookup("r_s")));

Info<< " Reading Ds_WE\n" << endl;
        Ds_WESolid.set (i, new dimensionedScalar (LiProperties.lookup("Ds_WE")));

Info<< " Reading kReact_WE\n" << endl;
        kReact_WESolid.set (i, new dimensionedScalar (LiProperties.lookup("kReact_WE")));

Info<< " Reading kReact_RE\n" << endl;
        kReact_RESolid.set (i, new dimensionedScalar (LiProperties.lookup("kReact_RE")));


Info<< " Reading alphaA_WE\n" << endl;
        alphaA_WESolid.set (i, new dimensionedScalar (LiProperties.lookup("alphaA_WE")));

Info<< " Reading alphaC_WE\n" << endl;
        alphaC_WESolid.set (i, new dimensionedScalar (LiProperties.lookup("alphaC_WE")));

Info<< " Reading alphaA_RE\n" << endl;
        alphaA_RESolid.set (i, new dimensionedScalar (LiProperties.lookup("alphaA_RE")));

Info<< " Reading alphaC_RE\n" << endl;
        alphaC_RESolid.set (i, new dimensionedScalar (LiProperties.lookup("alphaC_RE")));

Info<< " Reading fai_s_RE\n" << endl;
        fai_s_RESolid.set (i, new dimensionedScalar (LiProperties.lookup("fai_s_RE")));

Info<< " Reading U_rev_RE\n" << endl;
        U_rev_RESolid.set (i, new dimensionedScalar (LiProperties.lookup("U_rev_RE")));

Info<< " Reading iRef_RE\n" << endl;
        iRef_RESolid.set (i, new dimensionedScalar (LiProperties.lookup("iRef_RE")));

Info<< " Reading T_temp_ref\n" << endl;
        T_temp_refSolid.set (i, new dimensionedScalar (LiProperties.lookup("T_temp_ref")));

Info<< " Reading I_app\n" << endl;
        I_appSolid.set (i, new dimensionedScalar (LiProperties.lookup("I_app")));


}




PtrList<volScalarField> fai_eSolid(solidRegions.size());   //ionic potential
PtrList<volScalarField> fai_sSolid(solidRegions.size());   //electronic potential
PtrList<volScalarField> k_fai_eSolid(solidRegions.size());   //ionic conductivity
PtrList<volScalarField> k_fai_sSolid(solidRegions.size());   //electronic conductivity
PtrList<volScalarField> J_currentSolid(solidRegions.size());   //Source terms of ionic and electronic potential equation

PtrList<volScalarField> thetaSolid(solidRegions.size());   //state of charge in electrodes
PtrList<volScalarField> C_seSolid(solidRegions.size());   //Li concentration at the interface of solid electrode and electrolyte
PtrList<volScalarField> U_revSolid(solidRegions.size());   //reversible voltage
PtrList<volScalarField> etaSolid(solidRegions.size());   //over potential
PtrList<volScalarField> iRefSolid(solidRegions.size());   //reference exchange current density
PtrList<volScalarField> i_currentSolid(solidRegions.size());   //current density
PtrList<volScalarField> T_tempSolid(solidRegions.size());   //user-defined temperature
PtrList<volScalarField> CeSolid(solidRegions.size());   //Li+ concentration in the electrolyte
PtrList<volScalarField> k_T_tempSolid(solidRegions.size());   //thermal conductivity of user-fined temperature 
PtrList<volScalarField> D_CeSolid(solidRegions.size());   //diffusion coefficient of Ce
PtrList<volScalarField> S_CeSolid(solidRegions.size());   //source term of Ce
PtrList<volScalarField> S_T_tempSolid(solidRegions.size());   //source term of S_T_temp
PtrList<volScalarField> CsSolid(solidRegions.size());   //Li+ concentration in the granular particles
PtrList<volScalarField> S_CsSolid(solidRegions.size());   //source term of Cs
PtrList<volScalarField> por_CeSolid(solidRegions.size());   //porosity (proportion of electrolyte) por_Ce
PtrList<volScalarField> por_CsSolid(solidRegions.size());   //solid-phase fraction por_Cs
PtrList<volScalarField> k_D_effSolid(solidRegions.size());   //diffusional conductivity k_D_eff
PtrList<volScalarField> spArea_aSolid(solidRegions.size());   //specific surface area of anode particles
PtrList<volScalarField> spArea_cSolid(solidRegions.size());   //specific surface area of cathode particles




PtrList<dimensionedScalar> Cs_max_aSolid(solidRegions.size()); //maximum li concentration in anode materials
PtrList<dimensionedScalar> Cs_max_cSolid(solidRegions.size()); //maximum li concentration in cathode materials
PtrList<dimensionedScalar> kReact_aSolid(solidRegions.size()); //reaction rate constant in anode
PtrList<dimensionedScalar> kReact_cSolid(solidRegions.size()); //reaction rate constant in cathode
PtrList<dimensionedScalar> alphaA_aSolid(solidRegions.size()); //anodic transference coefficient in the anode
PtrList<dimensionedScalar> alphaC_aSolid(solidRegions.size()); //cathodic transference coefficient in the anode
PtrList<dimensionedScalar> alphaA_cSolid(solidRegions.size()); //anodic transference coefficient in the cathode
PtrList<dimensionedScalar> alphaC_cSolid(solidRegions.size()); //cathodic transference coefficient in the cathode
PtrList<dimensionedScalar> RSolid(solidRegions.size()); //universal gas constant
PtrList<dimensionedScalar> FSolid(solidRegions.size()); //Faraday constant 

PtrList<dimensionedScalar> por_Ce_aSolid(solidRegions.size()); //porosity in anode
PtrList<dimensionedScalar> por_Ce_spSolid(solidRegions.size()); //porosity in seperator
PtrList<dimensionedScalar> por_Ce_cSolid(solidRegions.size()); //porosity in cathode
PtrList<dimensionedScalar> por_f_aSolid(solidRegions.size()); //filler fraction in anode
PtrList<dimensionedScalar> por_f_cSolid(solidRegions.size()); //filler fraction in cathode
PtrList<dimensionedScalar> bruggSolid(solidRegions.size()); //bruggman coefficient
PtrList<dimensionedScalar> k0_fai_s_aSolid(solidRegions.size()); //intrinsic solid-phase conductivity in anode
PtrList<dimensionedScalar> k0_fai_s_cSolid(solidRegions.size()); //intrinsic solid-phase conductivity in cathode
PtrList<dimensionedScalar> D0_CeSolid(solidRegions.size()); //intrinsic electrolyte diffusion coefficient

PtrList<dimensionedScalar> tNoSolid(solidRegions.size());   //transference number
PtrList<dimensionedScalar> Ds_aSolid(solidRegions.size());   //diffusivity of Li in solid-phase of anode
PtrList<dimensionedScalar> Ds_cSolid(solidRegions.size());   //diffusivity of Li in solid-phase of cathode
PtrList<dimensionedScalar> r_s_aSolid(solidRegions.size());   //particle radius in anode
PtrList<dimensionedScalar> r_s_cSolid(solidRegions.size());   //particle radius in cathode
PtrList<dimensionedScalar> I_appSolid(solidRegions.size());   //applied current density
//PtrList<dimensionedScalar> V_outSolid(solidRegions.size());   //output voltage


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
            dimensionedScalar("k_fai_s", dimless, 1000),
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

//V22   specific area of anode particles spArea_a
Info<< " Adding to spArea_a\n" << endl;
   spArea_aSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "spArea_a",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("spArea_a", dimless, 0),
            zeroGradientFvPatchScalarField::typeName
       )
    );

//V23   specific area of cathode particles spArea_c
Info<< " Adding to spArea_c\n" << endl;
   spArea_cSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "spArea_c",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("spArea_c", dimless, 0),
            zeroGradientFvPatchScalarField::typeName
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


Info<< " Reading Cs_max_a\n" << endl;
        Cs_max_aSolid.set (i, new dimensionedScalar (LiProperties.lookup("Cs_max_a")));

Info<< " Reading Cs_max_c\n" << endl;
        Cs_max_cSolid.set (i, new dimensionedScalar (LiProperties.lookup("Cs_max_c")));

Info<< " Reading kReact_a\n" << endl;
        kReact_aSolid.set (i, new dimensionedScalar (LiProperties.lookup("kReact_a")));

Info<< " Reading kReact_c\n" << endl;
        kReact_cSolid.set (i, new dimensionedScalar (LiProperties.lookup("kReact_c")));

Info<< " Reading alphaA_a\n" << endl;
        alphaA_aSolid.set (i, new dimensionedScalar (LiProperties.lookup("alphaA_a")));

Info<< " Reading alphaC_a\n" << endl;
        alphaC_aSolid.set (i, new dimensionedScalar (LiProperties.lookup("alphaC_a")));

Info<< " Reading alphaA_c\n" << endl;
        alphaA_cSolid.set (i, new dimensionedScalar (LiProperties.lookup("alphaA_c")));

Info<< " Reading alphaC_c\n" << endl;
        alphaC_cSolid.set (i, new dimensionedScalar (LiProperties.lookup("alphaC_c")));

Info<< " Reading F\n" << endl;
        FSolid.set (i, new dimensionedScalar (LiProperties.lookup("F")));

Info<< " Reading R\n" << endl;
        RSolid.set (i, new dimensionedScalar (LiProperties.lookup("R")));

Info<< " Reading por_Ce_a\n" << endl;
        por_Ce_aSolid.set (i, new dimensionedScalar (LiProperties.lookup("por_Ce_a")));

Info<< " Reading por_Ce_sp\n" << endl;
        por_Ce_spSolid.set (i, new dimensionedScalar (LiProperties.lookup("por_Ce_sp")));

Info<< " Reading por_Ce_c\n" << endl;
        por_Ce_cSolid.set (i, new dimensionedScalar (LiProperties.lookup("por_Ce_c")));

Info<< " Reading por_f_a\n" << endl;
        por_f_aSolid.set (i, new dimensionedScalar (LiProperties.lookup("por_f_a")));

Info<< " Reading por_f_c\n" << endl;
        por_f_cSolid.set (i, new dimensionedScalar (LiProperties.lookup("por_f_c")));

Info<< " Reading brugg\n" << endl;
       bruggSolid.set (i, new dimensionedScalar (LiProperties.lookup("brugg")));

Info<< " Reading k0_fai_s_a\n" << endl;
       k0_fai_s_aSolid.set (i, new dimensionedScalar (LiProperties.lookup("k0_fai_s_a")));

Info<< " Reading k0_fai_s_c\n" << endl;
       k0_fai_s_cSolid.set (i, new dimensionedScalar (LiProperties.lookup("k0_fai_s_c")));

Info<< " Reading D0_Ce\n" << endl;
       D0_CeSolid.set (i, new dimensionedScalar (LiProperties.lookup("D0_Ce")));

Info<< " Reading tNo\n" << endl;
        tNoSolid.set (i, new dimensionedScalar (LiProperties.lookup("tNo")));

Info<< " Reading Ds_a\n" << endl;
        Ds_aSolid.set (i, new dimensionedScalar (LiProperties.lookup("Ds_a")));

Info<< " Reading Ds_c\n" << endl;
        Ds_cSolid.set (i, new dimensionedScalar (LiProperties.lookup("Ds_c")));

Info<< " Reading r_s_a\n" << endl;
        r_s_aSolid.set (i, new dimensionedScalar (LiProperties.lookup("r_s_a")));

Info<< " Reading r_s_c\n" << endl;
        r_s_cSolid.set (i, new dimensionedScalar (LiProperties.lookup("r_s_c")));

Info<< " Reading I_app\n" << endl;
        I_appSolid.set (i, new dimensionedScalar (LiProperties.lookup("I_app")));

//Info<< " Reading V_out\n" << endl;
//        V_outSolid.set (i, new dimensionedScalar (LiProperties.lookup("V_out")));









}


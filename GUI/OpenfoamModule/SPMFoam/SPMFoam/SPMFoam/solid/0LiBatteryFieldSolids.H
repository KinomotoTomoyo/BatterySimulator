

PtrList<volScalarField> CsSolid(solidRegions.size());   //concentration of Li in the solid phase, mol m-3
PtrList<volScalarField> DsSolid(solidRegions.size());   //diffusion coefficeint of Li inside solid phase, m2/s
PtrList<volScalarField> XSolid(solidRegions.size());   //state of charge in the solid phase
PtrList<volScalarField> EeqSolid(solidRegions.size());   // equilibrium potential inside solid phase, V 
PtrList<volScalarField> iRefSolid(solidRegions.size());   //reference exchange current density, A m2





PtrList<dimensionedScalar> Ds_valueSolid(solidRegions.size());   //intrinsic diffusivity of Li in the solid phase, m2/s
PtrList<dimensionedScalar> Cs_maxSolid(solidRegions.size());   //maximum Li concentration in solid phase, mol m-3
PtrList<dimensionedScalar> kReactSolid(solidRegions.size());   //reaction rate constant, m2.5 mol-0.5 s-1
PtrList<dimensionedScalar> RSolid(solidRegions.size());   //Universal constant, J mol-1 K-1
PtrList<dimensionedScalar> FSolid(solidRegions.size());   //Faraday constant, C mol-1
PtrList<dimensionedScalar> CeSolid(solidRegions.size());   //electrolyte concentration, mol m-3
PtrList<dimensionedScalar> alphaASolid(solidRegions.size());   //anodic transfer coefficient
PtrList<dimensionedScalar> alphaCSolid(solidRegions.size());   //cathodic transfer coefficient
PtrList<dimensionedScalar> T_tempSolid(solidRegions.size());   //temperature, K
PtrList<dimensionedScalar> I_appSolid(solidRegions.size());   //applied current density, A m-2



forAll(solidRegions, i)
{


//V1    concentration of Li in the solid phase 
Info<< " Adding to CsSolid\n" << endl;
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

//V2   diffusion coefficeint of Li inside solid phase
Info<< " Adding to Ds\n" << endl;
   DsSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "Ds",
             runTime.timeName(),
             solidRegions[i],
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i],
            dimensionedScalar("Ds", dimless, 3.9e-14),
            zeroGradientFvPatchScalarField::typeName
       )
    );


//V3    state of charge in the solid phase 
Info<< " Adding to XSolid\n" << endl;
   XSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "X",
             runTime.timeName(),
             solidRegions[i],
             IOobject::MUST_READ,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i]
        )
    );


//V4    equilibrium potential of lithiation inside the solid phase, V 
Info<< " Adding to Eeq\n" << endl;
   EeqSolid.set
   (
        i,
        new volScalarField
       (
            IOobject
           (
             "Eeq",
             runTime.timeName(),
             solidRegions[i],
             IOobject::MUST_READ,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i]
        )
    );

//V5    reference exchange current density
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
             IOobject::MUST_READ,
             IOobject::AUTO_WRITE
            ),
            solidRegions[i]
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

Info<< " Reading Ds_value\n" << endl;
        Ds_valueSolid.set (i, new dimensionedScalar (LiProperties.lookup("Ds_value")));

Info<< " Reading Cs_max\n" << endl;
        Cs_maxSolid.set (i, new dimensionedScalar (LiProperties.lookup("Cs_max")));

Info<< " Reading kReact\n" << endl;
        kReactSolid.set (i, new dimensionedScalar (LiProperties.lookup("kReact")));

Info<< " Reading R\n" << endl;
        RSolid.set (i, new dimensionedScalar (LiProperties.lookup("R")));

Info<< " Reading F\n" << endl;
        FSolid.set (i, new dimensionedScalar (LiProperties.lookup("F")));

Info<< " Reading Ce\n" << endl;
        CeSolid.set (i, new dimensionedScalar (LiProperties.lookup("Ce")));

Info<< " Reading alphaA\n" << endl;
        alphaASolid.set (i, new dimensionedScalar (LiProperties.lookup("alphaA")));

Info<< " Reading alphaC\n" << endl;
        alphaCSolid.set (i, new dimensionedScalar (LiProperties.lookup("alphaC")));

Info<< " Reading T_temp\n" << endl;
        T_tempSolid.set (i, new dimensionedScalar (LiProperties.lookup("T_temp")));

Info<< " Reading I_app\n" << endl;
        I_appSolid.set (i, new dimensionedScalar (LiProperties.lookup("I_app")));


}


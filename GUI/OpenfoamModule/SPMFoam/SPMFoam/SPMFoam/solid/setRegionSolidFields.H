fvMesh& mesh = solidRegions[i];
solidThermo& thermo = thermos[i];

tmp<volScalarField> trho = thermo.rho();
const volScalarField& rho = trho();

tmp<volScalarField> tcp = thermo.Cp();
const volScalarField& cp = tcp();

tmp<volSymmTensorField> taniAlpha;
if (!thermo.isotropic())
{
    volSymmTensorField& aniAlpha = aniAlphas[i];
    tmp<volVectorField> tkappaByCp = thermo.Kappa()/cp;
    const coordinateSystem& coodSys = coordinates[i];

    aniAlpha.primitiveFieldRef() =
        coodSys.R().transformVector(tkappaByCp());
    aniAlpha.correctBoundaryConditions();

    taniAlpha = tmp<volSymmTensorField>
    (
        new volSymmTensorField(aniAlpha)
    );
}


volScalarField& h = thermo.he();

const volScalarField& betav = betavSolid[i];

fv::options& fvOptions = solidHeatSources[i];

solidNoLoopControl& pimple = pimples.solid(i);

//added, not original===========================================

volScalarField& Cs    = CsSolid[i];
volScalarField& Ds    = DsSolid[i];
volScalarField& X     = XSolid[i];
volScalarField& Eeq   = EeqSolid[i];
volScalarField& iRef  = iRefSolid[i];

const dimensionedScalar& Ds_value = Ds_valueSolid[i];
const dimensionedScalar& Cs_max   = Cs_maxSolid[i];
const dimensionedScalar& kReact   = kReactSolid[i];
const dimensionedScalar& R        = RSolid[i];
const dimensionedScalar& F        = FSolid[i];
const dimensionedScalar& Ce       = CeSolid[i];
const dimensionedScalar& alphaA   = alphaASolid[i];
const dimensionedScalar& alphaC   = alphaCSolid[i];
const dimensionedScalar& T_temp   = T_tempSolid[i];
const dimensionedScalar& I_app    = I_appSolid[i];


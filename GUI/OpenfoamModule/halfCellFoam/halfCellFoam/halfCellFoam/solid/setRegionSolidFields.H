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
volScalarField& fai_e       = fai_eSolid[i];
volScalarField& fai_s       = fai_sSolid[i];
volScalarField& k_fai_e     = k_fai_eSolid[i];
volScalarField& k_fai_s     = k_fai_sSolid[i];
volScalarField& J_current   = J_currentSolid[i];
volScalarField& theta       = thetaSolid[i];
volScalarField& C_se        = C_seSolid[i];
volScalarField& U_rev       = U_revSolid[i];
volScalarField& eta         = etaSolid[i];
volScalarField& iRef        = iRefSolid[i];
volScalarField& i_current   = i_currentSolid[i];
volScalarField& T_temp      = T_tempSolid[i];
volScalarField& Ce          = CeSolid[i];
volScalarField& k_T_temp    = k_T_tempSolid[i];
volScalarField& D_Ce        = D_CeSolid[i];
volScalarField& S_Ce        = S_CeSolid[i];
volScalarField& S_T_temp    = S_T_tempSolid[i];
volScalarField& Cs          = CsSolid[i];
volScalarField& S_Cs        = S_CsSolid[i];
volScalarField& por_Ce      = por_CeSolid[i];
volScalarField& por_Cs      = por_CsSolid[i];
volScalarField& k_D_eff     = k_D_effSolid[i];
volScalarField& spArea_WE   = spArea_WESolid[i];
volVectorField& i_e         = i_eSolid[i];


const dimensionedScalar& por_Ce_WE     = por_Ce_WESolid[i];
const dimensionedScalar& por_Ce_sp     = por_Ce_spSolid[i];
const dimensionedScalar& por_f_WE      = por_f_WESolid[i];
const dimensionedScalar& Cs_max_WE     = Cs_max_WESolid[i];
const dimensionedScalar& brugg         = bruggSolid[i];
const dimensionedScalar& R             = RSolid[i];
const dimensionedScalar& F             = FSolid[i];
const dimensionedScalar& tNo           = tNoSolid[i];
const dimensionedScalar& k0_fai_s_WE   = k0_fai_s_WESolid[i];
const dimensionedScalar& D0_Ce         = D0_CeSolid[i];
const dimensionedScalar& r_s           = r_sSolid[i];
const dimensionedScalar& Ds_WE         = Ds_WESolid[i];
const dimensionedScalar& kReact_WE     = kReact_WESolid[i];
const dimensionedScalar& kReact_RE     = kReact_RESolid[i];
const dimensionedScalar& alphaA_WE     = alphaA_WESolid[i];
const dimensionedScalar& alphaC_WE     = alphaC_WESolid[i];
const dimensionedScalar& alphaA_RE     = alphaA_RESolid[i];
const dimensionedScalar& alphaC_RE     = alphaC_RESolid[i];
const dimensionedScalar& fai_s_RE      = fai_s_RESolid[i];
const dimensionedScalar& U_rev_RE      = U_rev_RESolid[i];
const dimensionedScalar& iRef_RE       = iRef_RESolid[i];
const dimensionedScalar& T_temp_ref    = T_temp_refSolid[i];
const dimensionedScalar& I_app         = I_appSolid[i];





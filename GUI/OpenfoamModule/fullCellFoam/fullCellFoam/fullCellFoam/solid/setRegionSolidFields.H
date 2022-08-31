fvMesh& mesh = solidRegions[i];
solidThermo& thermo = thermos[i];

tmp<volScalarField> trho = thermo.rho();
const volScalarField& rho = trho();

tmp<volScalarField> tCv = thermo.Cv();
const volScalarField& Cv = tCv();

tmp<volSymmTensorField> taniAlpha;
if (!thermo.isotropic())
{
    volSymmTensorField& aniAlpha = aniAlphas[i];
    tmp<volVectorField> tkappaByCv = thermo.Kappa()/Cv;
    const coordinateSystem& coodSys = coordinates[i];

    aniAlpha.primitiveFieldRef() =
        coodSys.R().transformVector(tkappaByCv());
    aniAlpha.correctBoundaryConditions();

    taniAlpha = tmp<volSymmTensorField>
    (
        new volSymmTensorField(aniAlpha)
    );
}


volScalarField& e = thermo.he();

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
volScalarField& spArea_a    = spArea_aSolid[i];
volScalarField& spArea_c    = spArea_cSolid[i];


const dimensionedScalar& Cs_max_a      = Cs_max_aSolid[i];
const dimensionedScalar& Cs_max_c      = Cs_max_cSolid[i];
const dimensionedScalar& kReact_a      = kReact_aSolid[i];
const dimensionedScalar& kReact_c      = kReact_cSolid[i];
const dimensionedScalar& alphaA_a      = alphaA_aSolid[i];
const dimensionedScalar& alphaC_a      = alphaC_aSolid[i];
const dimensionedScalar& alphaA_c      = alphaA_cSolid[i];
const dimensionedScalar& alphaC_c      = alphaC_cSolid[i];
const dimensionedScalar& R             = RSolid[i];
const dimensionedScalar& F             = FSolid[i];
const dimensionedScalar& por_Ce_a      = por_Ce_aSolid[i];
const dimensionedScalar& por_Ce_sp     = por_Ce_spSolid[i];
const dimensionedScalar& por_Ce_c      = por_Ce_cSolid[i];
const dimensionedScalar& por_f_a       = por_f_aSolid[i];
const dimensionedScalar& por_f_c       = por_f_cSolid[i];
const dimensionedScalar& brugg         = bruggSolid[i];
const dimensionedScalar& k0_fai_s_a    = k0_fai_s_aSolid[i];
const dimensionedScalar& k0_fai_s_c    = k0_fai_s_cSolid[i];
const dimensionedScalar& D0_Ce         = D0_CeSolid[i];
const dimensionedScalar& tNo           = tNoSolid[i];
const dimensionedScalar& Ds_a          = Ds_aSolid[i];
const dimensionedScalar& Ds_c          = Ds_cSolid[i];
const dimensionedScalar& r_s_a         = r_s_aSolid[i];
const dimensionedScalar& r_s_c         = r_s_cSolid[i];
const dimensionedScalar& I_app         = I_appSolid[i];
//const dimensionedScalar& V_out         = V_outSolid[i];






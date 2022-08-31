/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
	This simulator is under GPLv3 (see <http://www.gnu.org/licenses/>) which means you can use it for free, 
	However, you cannot use it for any commercial purpose.

Application
    fullCellFoam

Description
    Solver for P2D model for a full lithium ion battery composed of anode, separator and cathode. 

Citing
	Please notice that any academic work using this code or derivatives thereof, 
	must cite the original paper that this code was released alongside.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "turbulentFluidThermoModel.H"
#include "rhoReactionThermo.H"
#include "CombustionModel.H"
#include "fixedGradientFvPatchFields.H"
#include "regionProperties.H"
#include "compressibleCourantNo.H"
#include "solidRegionDiffNo.H"
#include "solidThermo.H"
#include "radiationModel.H"
#include "fvOptions.H"
#include "coordinateSystem.H"
#include "pimpleMultiRegionControl.H"
#include "pressureControl.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #define NO_CONTROL
    #define CREATE_MESH createMeshesPostProcess.H
    #include "postProcess.H"

    #include "setRootCaseLists.H"
    #include "createTime.H"
    #include "createMeshes.H"
    #include "createFields.H"
    #include "initContinuityErrs.H"
    pimpleMultiRegionControl pimples(fluidRegions, solidRegions);
    #include "createFluidPressureControls.H"
    #include "createTimeControls.H"
    #include "readSolidTimeControls.H"
    #include "compressibleMultiRegionCourantNo.H"
    #include "solidRegionDiffusionNo.H"
    #include "setInitialMultiRegionDeltaT.H"


    #include "0userDefinedVars.H"
    #include "outputSet.H"

    while (pimples.run(runTime))
    {
        #include "readTimeControls.H"
        #include "readSolidTimeControls.H"

        #include "compressibleMultiRegionCourantNo.H"
        #include "solidRegionDiffusionNo.H"
        #include "setMultiRegionDeltaT.H"

        runTime++;

        Info<< "Time = " << runTime.timeName() << nl << endl;

        // --- PIMPLE loop
        while (pimples.loop())
        {
            forAll(fluidRegions, i)
            {
              //  Info<< "\nSolving for fluid region "
             //       << fluidRegions[i].name() << endl;
             //   #include "setRegionFluidFields.H"
            //    #include "solveFluid.H"
            }

            forAll(solidRegions, i)
            {
                Info<< "\nSolving for solid region "
                    << solidRegions[i].name() << endl;
                #include "setRegionSolidFields.H"
                #include "solveSolid.H"
            }

        }

        runTime.write();

        count = count + 1;

        if (count % 10 == 0) // 1/10 s is the time step, write down per second
        {
          count = 0;
          #include "outputSolid.H"
        }


        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;

        if (flag_SOC_a == -1) // anode SOC exceeds its lower limit, which means over-delithiated
        {
          Info<< "Anode over-delithiated!" << nl << endl;
        }

        if (flag_SOC_a == 1) // anode SOC exceeds its higher limit, which means over-lithiated
        {
          Info<< "Anode over-lithiated!" << nl << endl;
        }

        if (flag_SOC_c == -1) // cathode SOC exceeds its lower limit, which means over-delithiated
        {
          Info<< "Cathode over-delithiated!" << nl << endl;
        }

        if (flag_SOC_c == 1) // cathode SOC exceeds its higher limit, which means over-lithiated
        {
          Info<< "Cathode over-lithiated!" << nl << endl;
        }

        if (flag_SOC_a!=0 || flag_SOC_c!=0) // if SOC out of normal range
        {
          Info<< "SOC out of their limits!!" << nl << endl;
          break;
        }

        if (flag_voltage!=0) // if voltage reaches below 0V or SOC out of normal range
        {
          Info<< "Voltage cannot be lower than 0V!" << nl << endl;
          break;
        }

    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //

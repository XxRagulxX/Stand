#include "Commands/Vehicle/LSC/CommandListLosSantosCustoms.hpp"

#include "Commands/Vehicle/LSC/CommandLscShared.hpp"
#include "Commands/Vehicle/LSC/CommandVehmod.hpp"
#include "Commands/Vehicle/LSC/CommandNeonLight.hpp"
#include "Commands/Vehicle/LSC/CommandLscWheels.hpp"
#include "Commands/Vehicle/LSC/CommandLscPlate.hpp"
#include "Commands/Vehicle/LSC/CommandHeadlights.hpp"
#include "Commands/Vehicle/LSC/CommandWindowTint.hpp"
#include "Commands/Vehicle/LSC/CommandVehRoofLivery.hpp"
#include "Commands/Vehicle/LSC/CommandVehicleExtra.hpp"
#include "Commands/Vehicle/LSC/CommandScorched.hpp"
#include "Commands/Vehicle/LSC/CommandVehicleParachute.hpp"
#include "Commands/Vehicle/LSC/CommandLscTuning.hpp"
#include "Commands/Vehicle/LSC/CommandListLscPerformance.hpp"
#include "Commands/Vehicle/LSC/CommandCustomsShowNa.hpp"

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
    CommandListLosSantosCustoms::CommandListLosSantosCustoms(CommandList* parent)
        : CommandList(parent, LIT("Los Santos Customs"), CMDNAMES("lsc", "customs", "lossantoscustoms"))
    {
        createChild<CommandListLscPerformance>();

        auto* appear = createChild<CommandList>(LIT("Appearance"), CMDNAMES_0());

        appear->createChild<CommandVehicleColour>(LIT("Primary Colour"), CMDNAMES("vehprimary", "vehicleprimary"), ColourTarget::Primary);
        appear->createChild<CommandVehicleColour>(LIT("Secondary Colour"), CMDNAMES("vehsecondary", "vehiclesecondary"), ColourTarget::Secondary);

        auto* pearl = appear->createChild<CommandList>(LIT("Pearlescent Colour"), CMDNAMES("vehpearlescent"));
        AddStdColours(pearl, ColourTarget::Pearlescent, true);

        auto* neons = appear->createChild<CommandList>(LIT("Neon Lights"), CMDNAMES("neoncolour"));
        {
            neons->createChild<CommandNeonColour>();
            neons->createChild<CommandNeonAll>();
            neons->createChild<CommandNeonLight>(LIT("Front"),  CMDNAMES("vehneonfront"), 2);
            neons->createChild<CommandNeonLight>(LIT("Back"),   CMDNAMES("vehneonback"),  3);
            neons->createChild<CommandNeonLight>(LIT("Left"),   CMDNAMES("vehneonleft"),  0);
            neons->createChild<CommandNeonLight>(LIT("Right"),  CMDNAMES("vehneonright"), 1);
        }

        auto* wheels = appear->createChild<CommandList>(LIT("Wheels"), CMDNAMES_0());
        {
            wheels->createChild<CommandWheelType>();
            wheels->createChild<CommandVehmodInt>(Mod::FrontWheels);

            auto* wheelCol = wheels->createChild<CommandList>(LIT("Wheel Colour"), CMDNAMES("vehwheelcolour"));
            AddStdColours(wheelCol, ColourTarget::Wheel);

            wheels->createChild<CommandTyreSmokeColour>();
            wheels->createChild<CommandVehDriftTyres>();
            wheels->createChild<CommandVehBulletproofTyres>();
        }

        auto* plate = appear->createChild<CommandList>(LIT("License Plate"), CMDNAMES_0());
        {
            plate->createChild<CommandPlateType>();
            plate->createChild<CommandPlateRandomise>();
            plate->createChild<CommandListScrollingText>();
            plate->createChild<CommandPlateSpeed>();
            plate->createChild<CommandPlateTps>();
            plate->createChild<CommandLockPlate>();
        }

        auto* interior = appear->createChild<CommandList>(LIT("Interior Colour"), CMDNAMES("vehinteriorcolour"));
        AddStdColours(interior, ColourTarget::Interior);

        {
            auto* para = appear->createChild<CommandList>(LIT("Parachute"), CMDNAMES_0());
            para->createChild<CommandParachuteEnable>();
            para->createChild<CommandParachuteModel>();
        }

        auto* extras = appear->createChild<CommandList>(LIT("Extras"), CMDNAMES_0());
        for (int i = 1; i <= 14; ++i)
            extras->createChild<CommandVehicleExtra>(i);

        appear->createChild<CommandScorched>();
        appear->createChild<CommandTonk>();
        appear->createChild<CommandLscShowNonApplicable>();

        appear->createChild<CommandHeadlights>();
        appear->createChild<CommandWindowTint>();
        appear->createChild<CommandVehRoofLivery>();

        for (int t : Mod::visual)
            appear->createChild<CommandVehmodInt>(t);

        createChild<CommandLscPresetTunings>();
        createChild<CommandTune>();
        createChild<CommandPerf>();
        createChild<CommandPerfWithSpoiler>();
        createChild<CommandRandomTune>();
        createChild<CommandUntune>();
    }
}

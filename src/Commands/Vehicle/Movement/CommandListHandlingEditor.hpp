#pragma once
#include "Commands/Vehicle/Movement/CommandMovementShared.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    namespace
    {
        template<size_t Offset>
        class CommandHEField : public CommandSliderFloat
        {
        public:
            CommandHEField(CommandList* parent, Label name, std::vector<CommandName> cmdnames,
                int min, int max, int def, int step)
                : CommandSliderFloat(parent, std::move(name), std::move(cmdnames), NOLABEL, min, max, def, step) {}

            void onChange(Click& click, int) override
            {
                if (click.isAuto()) return;
                float fval = getFloatValue();
                click.ensureScriptThread([fval] {
                    int veh = MovVehicle();
                    if (!veh) return;
                    void* cveh = GetVehPtr(veh);
                    if (!cveh) return;
                    void* hdata = GetHandlingData(cveh);
                    if (!hdata) return;
                    FieldAt<float>(hdata, Offset) = fval;
                });
            }
        };

        class CommandListHEBase : public CommandList
        {
        public:
            explicit CommandListHEBase(CommandList* parent)
                : CommandList(parent, LIT("Base"), CMDNAMES_0())
            {
                createChild<CommandHEField<0x044>>(LIT("Drive Bias Front"),    CMDNAMES("hedrivebias"),    0,    200,   50,   1);
                createChild<CommandHEField<0x048>>(LIT("Drive Force"),          CMDNAMES("hedriveforce"),   0,    200,   30,   1);
                createChild<CommandHEField<0x04C>>(LIT("Drive Inertia"),        CMDNAMES("hedriveinertia"), 0,    500,  100,   1);
                createChild<CommandHEField<0x058>>(LIT("Clutch Rate Up Shift"), CMDNAMES("heclutchup"),     0,   2000,  100,  10);
                createChild<CommandHEField<0x05C>>(LIT("Clutch Rate Dn Shift"), CMDNAMES("heclutchdown"),   0,   2000,  100,  10);
                createChild<CommandHEField<0x060>>(LIT("Brake Bias Front"),     CMDNAMES("hebrakebias"),    0,    100,   65,   1);
                createChild<CommandHEField<0x064>>(LIT("Steering Lock"),        CMDNAMES("hesteeringlock"), 100, 8000, 3500, 100);
                createChild<CommandHEField<0x09C>>(LIT("Suspension Force"),     CMDNAMES("hesuspforce"),    0,   2000,  200,  10);
                createChild<CommandHEField<0x0A0>>(LIT("Susp Comp Damp"),       CMDNAMES("hesuspcomp"),     0,    500,  100,   5);
                createChild<CommandHEField<0x0A4>>(LIT("Susp Rebound Damp"),    CMDNAMES("hesusprebound"),  0,   2000,  300,  10);
                createChild<CommandHEField<0x0A8>>(LIT("Traction Loss Mult"),   CMDNAMES("hetractionloss"), 0,    300,  100,   1);
                createChild<CommandHEField<0x0B8>>(LIT("Anti Roll Bar Force"),  CMDNAMES("herollbar"),      0,   1000,  100,  10);
            }
        };

        template<size_t Offset>
        class CommandHECarField : public CommandSliderFloat
        {
        public:
            CommandHECarField(CommandList* parent, Label name, std::vector<CommandName> cmdnames,
                int min, int max, int def, int step)
                : CommandSliderFloat(parent, std::move(name), std::move(cmdnames), NOLABEL, min, max, def, step) {}

            void onChange(Click& click, int) override
            {
                if (click.isAuto()) return;
                float fval = getFloatValue();
                click.ensureScriptThread([fval] {
                    int veh = MovVehicle();
                    if (!veh) return;
                    void* cveh = GetVehPtr(veh);
                    if (!cveh) return;
                    void* hdata = GetHandlingData(cveh);
                    void* car = FindSubHandling(hdata, 6);
                    if (!car) return;
                    FieldAt<float>(car, Offset) = fval;
                });
            }
        };

        class CommandListHECar : public CommandList
        {
        public:
            explicit CommandListHECar(CommandList* parent)
                : CommandList(parent, LIT("Car"), CMDNAMES_0())
            {
                createChild<CommandHECarField<0x08>>(LIT("Max Steer Angle"),      CMDNAMES("hecarsteer"),     0,   9000, 3500, 100);
                createChild<CommandHECarField<0x0C>>(LIT("Max Steer Angle Rear"), CMDNAMES("hecarsteerrear"), 0,   9000,    0, 100);
                createChild<CommandHECarField<0x10>>(LIT("Max Handbrake Torque"), CMDNAMES("hecarhbktorque"),  0,  50000, 5000, 100);
                createChild<CommandHECarField<0x14>>(LIT("Max Braking Decel"),    CMDNAMES("hecarbrakdecel"),  0,  10000, 1000, 100);
                createChild<CommandHECarField<0x18>>(LIT("Max Lat Decel"),        CMDNAMES("hecarlat"),        0,  10000, 1500, 100);
            }
        };

        class CommandShowNonApplicable : public CommandToggle
        {
        public:
            explicit CommandShowNonApplicable(CommandList* parent)
                : CommandToggle(parent, LIT("Show Non-Applicable"), CMDNAMES("heshownonapplicable")) {}

            void onChange(Click&) override {}
        };

        class CommandHECurrentPreset : public CommandSlider
        {
        public:
            explicit CommandHECurrentPreset(CommandList* parent)
                : CommandSlider(parent, LIT(""), CMDNAMES_0(), NOLABEL, 0, 0, 0, 1) {}

            std::string getValueText() const override { return "Presets"; }
        };

        class CommandListHEPresets : public CommandList
        {
        public:
            explicit CommandListHEPresets(CommandList* parent)
                : CommandList(parent, LIT("Presets"), CMDNAMES_0()) {}
        };

        class CommandListHandlingEditor : public CommandList
        {
        public:
            explicit CommandListHandlingEditor(CommandList* parent)
                : CommandList(parent, LIT("Handling Editor"), CMDNAMES_0())
            {
                createChild<CommandListHEBase>();
                createChild<CommandListHECar>();
                createChild<CommandShowNonApplicable>();
                createChild<CommandHECurrentPreset>();
                createChild<CommandListHEPresets>();
            }
        };
    }
}

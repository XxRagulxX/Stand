#pragma once
#include "Commands/Vehicle/Movement/CommandMovementShared.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    namespace
    {
        class CommandSuperDriveToggle : public CommandToggle
        {
        public:
            CommandToggle*      m_cam   = nullptr;
            CommandSliderFloat* m_speed = nullptr;
            CommandSlider*      m_btn   = nullptr;

            explicit CommandSuperDriveToggle(CommandList* parent)
                : CommandToggle(parent, LIT("Super Drive"), CMDNAMES("superdrive")) {}

            void onChange(Click& click) override
            {
                if (!m_on) return;
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int veh = MovVehicle();
                    if (!veh) return true;
                    int input = m_btn ? ButtonFromSlider(m_btn->value) : (int)ControllerInputs::INPUT_VEH_ACCELERATE;
                    float normal = PAD::GET_CONTROL_NORMAL(0, input);
                    if (normal == 0.f) return true;
                    Vector3 vec;
                    if (m_cam && m_cam->m_on)
                    {
                        vec = DirFromRot(CAMERA::GET_FINAL_RENDERED_CAM_ROT(2));
                        vec.x /= 2.f; vec.y /= 2.f; vec.z /= 2.f;
                    }
                    else
                    {
                        vec = ENTITY::GET_ENTITY_FORWARD_VECTOR(veh);
                    }
                    float spd = m_speed ? m_speed->getFloatValue() : 0.f;
                    ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(
                        veh, 1,
                        vec.x * spd * normal,
                        vec.y * spd * normal,
                        vec.z * spd * normal,
                        true, false, true, true);
                    return true;
                });
            }
        };

        class CommandSuperDrivePreset : public CommandPhysical
        {
            CommandSuperDriveToggle* m_tog;
            CommandToggle*           m_cam;
            CommandSliderFloat*      m_spd;
            bool                     m_horn;
        public:
            CommandSuperDrivePreset(CommandList* parent, Label name,
                CommandSuperDriveToggle* tog, CommandToggle* cam, CommandSliderFloat* spd, bool horn)
                : CommandPhysical(COMMAND_ACTION, parent, std::move(name), CMDNAMES_0(), NOLABEL)
                , m_tog(tog), m_cam(cam), m_spd(spd), m_horn(horn) {}

            void onClick(Click& click) override
            {
                auto* tog = m_tog; auto* cam = m_cam; auto* spd = m_spd; bool horn = m_horn;
                click.ensureScriptThread([tog, cam, spd, horn] {
                    Click ac(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                    tog->setStateBool(ac, true);
                    cam->setStateBool(ac, false);
                    spd->setValue(horn ? 10.f : 0.f, ac);
                });
            }
        };

        class CommandSuperDriveButton : public CommandSlider
        {
        public:
            explicit CommandSuperDriveButton(CommandList* parent)
                : CommandSlider(parent, LIT("Controller Button"), CMDNAMES("superdrivebutton"),
                    NOLABEL, 0, 3, 0, 1) {}

            std::string getValueText() const override
            {
                switch (value)
                {
                case 1: return "Left Trigger";
                case 2: return "Horn";
                case 3: return "Handbrake";
                default: return "Right Trigger";
                }
            }
        };

        class CommandListSuperDrive : public CommandList
        {
        public:
            explicit CommandListSuperDrive(CommandList* parent)
                : CommandList(parent, LIT("Super Drive"), CMDNAMES_0())
            {
                auto* presetsHolder = createChild<CommandList>(LIT("Presets"), CMDNAMES_0());
                auto* tog  = createChild<CommandSuperDriveToggle>();
                auto* cam  = createChild<CommandToggle>(
                    LIT("Super Drive Uses Camera"), CMDNAMES("superdrivecam"),
                    LIT("Changes the definition of \"forward\" for Super Drive to be where you're looking instead of where your vehicle is going."));
                auto* spd  = createChild<CommandSliderFloat>(
                    LIT("Speed"), CMDNAMES("superdrivespeed"), NOLABEL, -1000000, 1000000, 0, 1000);
                auto* btn  = createChild<CommandSuperDriveButton>();
                tog->m_cam   = cam;
                tog->m_speed = spd;
                tog->m_btn   = btn;
                presetsHolder->createChild<CommandSuperDrivePreset>(LIT("Super Drive"), tog, cam, spd, false);
                presetsHolder->createChild<CommandSuperDrivePreset>(LIT("Horn Boost"),  tog, cam, spd, true);
            }
        };
    }
}

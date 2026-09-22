#include "Commands/Vehicle/Movement/CommandListMovement.hpp"

#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/ControllerInputs.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Joaat.hpp"
#include "Util/Label.hpp"

#include <cmath>

namespace Stand
{
    namespace
    {
        static int MovVehicle()
        {
            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            if (veh && ENTITY::DOES_ENTITY_EXIST(veh)) return veh;
            veh = PED::GET_VEHICLE_PED_IS_IN(ped, true);
            return (veh && ENTITY::DOES_ENTITY_EXIST(veh)) ? veh : 0;
        }

        static Vector3 DirFromRot(Vector3 rot)
        {
            float px = rot.x * 3.14159265f / 180.f;
            float pz = rot.z * 3.14159265f / 180.f;
            return { -sinf(pz) * cosf(px), cosf(pz) * cosf(px), sinf(px) };
        }

        static float Lerp(float a, float b, float t) { return a + (b - a) * t; }

        static float DistXY(float x1, float y1, float x2, float y2)
        {
            float dx = x2 - x1, dy = y2 - y1;
            return sqrtf(dx * dx + dy * dy);
        }

        class CommandSuperDriveToggle : public CommandToggle
        {
        public:
            CommandToggle*      m_cam   = nullptr;
            CommandSliderFloat* m_speed = nullptr;

            explicit CommandSuperDriveToggle(CommandList* parent)
                : CommandToggle(parent, LIT("Super Drive"), CMDNAMES("superdrive")) {}

            void onChange(Click& click) override
            {
                if (!m_on) return;
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int veh = MovVehicle();
                    if (!veh) return true;
                    float normal = PAD::GET_CONTROL_NORMAL(0, (int)ControllerInputs::INPUT_VEH_ACCELERATE);
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
                tog->m_cam   = cam;
                tog->m_speed = spd;
                presetsHolder->createChild<CommandSuperDrivePreset>(LIT("Super Drive"), tog, cam, spd, false);
                presetsHolder->createChild<CommandSuperDrivePreset>(LIT("Horn Boost"),  tog, cam, spd, true);
            }
        };

        class CommandSuperHandbrake : public CommandToggle
        {
        public:
            explicit CommandSuperHandbrake(CommandList* parent)
                : CommandToggle(parent, LIT("Super Handbrake"), CMDNAMES("superhandbrake", "superhandbreak")) {}

            void onChange(Click& click) override
            {
                if (!m_on) return;
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    if (PAD::IS_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_VEH_HANDBRAKE))
                    {
                        int veh = MovVehicle();
                        if (veh) ENTITY::SET_ENTITY_VELOCITY(veh, 0.f, 0.f, 0.f);
                    }
                    return true;
                });
            }
        };

        class CommandDriveOnWater : public CommandToggle
        {
            static constexpr Stand::joaat_t kObjHash = Stand::Joaat("ar_prop_ar_bblock_huge_01");
            int m_obj = 0;

            void Cleanup()
            {
                if (m_obj)
                {
                    if (ENTITY::DOES_ENTITY_EXIST(m_obj))
                        ENTITY::DELETE_ENTITY(&m_obj);
                    m_obj = 0;
                }
                STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(kObjHash);
            }

        public:
            explicit CommandDriveOnWater(CommandList* parent)
                : CommandToggle(parent, LIT("Drive On Water"), CMDNAMES("driveonwater")) {}

            void onChange(Click& click) override
            {
                if (!m_on)
                {
                    click.ensureScriptThread([this] { Cleanup(); });
                    return;
                }
                click.ensureScriptThread([this] { STREAMING::REQUEST_MODEL(kObjHash); });
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) { Cleanup(); return false; }

                    if (!STREAMING::HAS_MODEL_LOADED(kObjHash))
                    {
                        STREAMING::REQUEST_MODEL(kObjHash);
                        return true;
                    }

                    int ped = PLAYER::GET_PLAYER_PED(-1);
                    int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
                    if (!veh || !ENTITY::DOES_ENTITY_EXIST(veh))
                        veh = PED::GET_VEHICLE_PED_IS_IN(ped, true);
                    if (!veh || !ENTITY::DOES_ENTITY_EXIST(veh))
                    {
                        Cleanup();
                        return true;
                    }

                    Vector3 pos = ENTITY::GET_ENTITY_COORDS(veh, true);
                    float waterZ = 0.f;
                    if (!WATER::TEST_VERTICAL_PROBE_AGAINST_ALL_WATER(pos.x, pos.y, pos.z + 5.f, 0, &waterZ))
                    {
                        Cleanup();
                        return true;
                    }

                    float targetZ = waterZ - 0.30f;

                    if (!m_obj || !ENTITY::DOES_ENTITY_EXIST(m_obj))
                    {
                        m_obj = OBJECT::CREATE_OBJECT(kObjHash, pos.x, pos.y, targetZ, false, false, true);
                        if (m_obj)
                        {
                            ENTITY::FREEZE_ENTITY_POSITION(m_obj, true);
                            ENTITY::SET_ENTITY_VISIBLE(m_obj, false, false);
                            ENTITY::SET_ENTITY_NO_COLLISION_ENTITY(m_obj, ped, false);
                        }
                    }
                    else
                    {
                        Vector3 objPos = ENTITY::GET_ENTITY_COORDS(m_obj, false);
                        bool upsideDown = ENTITY::IS_ENTITY_UPSIDEDOWN(veh);

                        if (!upsideDown || DistXY(objPos.x, objPos.y, pos.x, pos.y) > 5.f)
                        {
                            float lerpZ = Lerp(objPos.z, targetZ, 0.1f);
                            ENTITY::SET_ENTITY_COORDS_NO_OFFSET(m_obj, pos.x, pos.y, lerpZ, false, false, false);

                            Vector3 rot = ENTITY::GET_ENTITY_ROTATION(m_obj, 2);
                            rot.z = ENTITY::GET_ENTITY_ROTATION(veh, 2).z;

                            Vector3 fwd = ENTITY::GET_ENTITY_FORWARD_VECTOR(veh);
                            float fwdX = pos.x + fwd.x * 5.f;
                            float fwdY = pos.y + fwd.y * 5.f;
                            float fwdZ = 0.f;
                            if (WATER::TEST_VERTICAL_PROBE_AGAINST_ALL_WATER(fwdX, fwdY, pos.z + 5.f, 0, &fwdZ))
                            {
                                float dx = fwdX - pos.x, dy = fwdY - pos.y, dz = fwdZ - waterZ;
                                float distH = sqrtf(dx * dx + dy * dy);
                                float targetPitch = atan2f(dz, distH) * (180.f / 3.14159265f) - 4.3f;
                                rot.x = Lerp(rot.x, targetPitch, 0.05f);
                            }

                            ENTITY::SET_ENTITY_ROTATION(m_obj, rot.x, rot.y, rot.z, 2, true);
                        }

                        if (pos.z < objPos.z)
                            ENTITY::SET_ENTITY_COORDS_NO_OFFSET(veh, pos.x, pos.y, objPos.z + 0.1f, false, false, false);
                    }

                    return true;
                });
            }
        };

        class CommandSpeedLimit : public CommandSlider
        {
            bool m_ticking = false;
        public:
            explicit CommandSpeedLimit(CommandList* parent)
                : CommandSlider(parent, LIT("Speed Limit (KMH)"), CMDNAMES("speedlimit"),
                    LIT("Sets the speed limit of vehicles that you pilot or drive. (Default: 540)"),
                    0, 10000, 540, 20) {}

            ~CommandSpeedLimit() override
            {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            void onTick() override
            {
                int veh = MovVehicle();
                if (!veh) return;
                float maxSpd = (value == 540) ? 150.f : ((float)value / 3.6f);
                ENTITY::SET_ENTITY_MAX_SPEED(veh, maxSpd);
            }

            void onChange(Click& click, int) override
            {
                if (click.isAuto()) return;
                if (value != 540)
                {
                    if (!m_ticking)
                    {
                        CommandTickDispatch::AddCommand(this);
                        m_ticking = true;
                    }
                }
                else
                {
                    if (m_ticking)
                    {
                        CommandTickDispatch::RemoveCommand(this);
                        m_ticking = false;
                    }
                    click.ensureScriptThread([] {
                        int veh = MovVehicle();
                        if (veh) ENTITY::SET_ENTITY_MAX_SPEED(veh, 150.f);
                    });
                }
            }
        };

        class CommandEnginePowerMult : public CommandSliderFloat
        {
            bool m_ticking = false;
        public:
            explicit CommandEnginePowerMult(CommandList* parent)
                : CommandSliderFloat(parent, LIT("Engine Power Multiplier"), CMDNAMES("enginepowermult"),
                    NOLABEL, 100, 2000, 100, 50) {}

            ~CommandEnginePowerMult() override
            {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            void onTick() override
            {
                int veh = MovVehicle();
                if (!veh) return;
                VEHICLE::MODIFY_VEHICLE_TOP_SPEED(veh, getFloatValue() * 10.f);
            }

            void onChange(Click& click, int) override
            {
                if (click.isAuto()) return;
                float fval = getFloatValue();
                if (fval != 1.f)
                {
                    if (!m_ticking)
                    {
                        CommandTickDispatch::AddCommand(this);
                        m_ticking = true;
                    }
                }
                else
                {
                    if (m_ticking)
                    {
                        CommandTickDispatch::RemoveCommand(this);
                        m_ticking = false;
                    }
                    click.ensureScriptThread([] {
                        int veh = MovVehicle();
                        if (veh) VEHICLE::MODIFY_VEHICLE_TOP_SPEED(veh, 10.f);
                    });
                }
            }
        };

    }

    CommandListMovement::CommandListMovement(CommandList* parent)
        : CommandList(parent, LIT("Movement"), CMDNAMES_0())
    {
        createChild<CommandListSuperDrive>();
        createChild<CommandSuperHandbrake>();
        createChild<CommandDriveOnWater>();
        createChild<CommandSpeedLimit>();
        createChild<CommandEnginePowerMult>();
    }

}

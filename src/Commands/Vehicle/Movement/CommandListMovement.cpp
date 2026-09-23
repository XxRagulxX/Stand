#include "Commands/Vehicle/Movement/CommandListMovement.hpp"

#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Core/Pointers.hpp"
#include "Game/ControllerInputs.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Joaat.hpp"
#include "Util/Label.hpp"

#include <cmath>
#include <unordered_map>

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

        template<typename T>
        static T& FieldAt(void* base, size_t offset)
        {
            return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
        }

        static void* GetVehPtr(int veh)
        {
            return Pointers.HandleToPtr(veh);
        }

        static void* GetHandlingData(void* cveh)
        {
            return FieldAt<void*>(cveh, 0x0960);
        }

        static void* FindSubHandling(void* hdata, int target_type)
        {
            if (!hdata) return nullptr;
            void** arr = FieldAt<void**>(hdata, 0x158);
            uint16_t sz  = FieldAt<uint16_t>(hdata, 0x160);
            for (uint16_t i = 0; i < sz; i++)
            {
                void* sub = arr[i];
                if (!sub) continue;
                using GetTypeFn = int(*)(void*);
                auto** vtbl = *reinterpret_cast<void***>(sub);
                int type = reinterpret_cast<GetTypeFn>(vtbl[3])(sub);
                if (type == target_type)
                    return sub;
            }
            return nullptr;
        }

        static int ButtonFromSlider(int idx)
        {
            switch (idx)
            {
            case 1: return (int)ControllerInputs::INPUT_VEH_BRAKE;
            case 2: return (int)ControllerInputs::INPUT_VEH_HORN;
            case 3: return (int)ControllerInputs::INPUT_VEH_HANDBRAKE;
            default: return (int)ControllerInputs::INPUT_VEH_ACCELERATE;
            }
        }

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

        class CommandDriveUnderwater : public CommandToggle
        {
        public:
            explicit CommandDriveUnderwater(CommandList* parent)
                : CommandToggle(parent, LIT("Drive Underwater"), CMDNAMES("driveunderwater", "waterwheels", "waterdrive")) {}

            void onChange(Click& click) override
            {
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int veh = MovVehicle();
                    if (!veh) return true;
                    void* cveh = GetVehPtr(veh);
                    if (!cveh) return true;
                    unsigned int vtype = FieldAt<unsigned int>(cveh, 0x0C28);
                    if (vtype == 13 || vtype == 15) return true;
                    void* ph_inst = FieldAt<void*>(cveh, 0x30);
                    if (!ph_inst) return true;
                    void* archetype = FieldAt<void*>(ph_inst, 0x10);
                    if (!archetype) return true;
                    FieldAt<float>(archetype, 0x54) = 0.f;
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

        class CommandNoTurbulence : public CommandToggle
        {
        public:
            explicit CommandNoTurbulence(CommandList* parent)
                : CommandToggle(parent, LIT("No Turbulence"), CMDNAMES("smoothflight", "noturbulence")) {}

            void onChange(Click& click) override
            {
                onChangeToggleScriptTickEventHandler(click, [this] {
                    int veh = MovVehicle();
                    if (!m_on)
                    {
                        SetTurbulence(veh, 1.f);
                        return false;
                    }
                    SetTurbulence(veh, 0.f);
                    return true;
                });
            }

        private:
            static void SetTurbulence(int veh, float v)
            {
                if (!veh) return;
                if (!VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(veh))) return;
                void* cveh = GetVehPtr(veh);
                if (!cveh) return;
                FieldAt<float>(cveh, 0x1B00) = v;
                FieldAt<float>(cveh, 0x1B04) = v;
            }
        };

        class CommandHeliAutoStabilisation : public CommandToggle
        {
            struct ModValues
            {
                float orig_thrust;
                float orig_attack_lift;
                float orig_attack_dive;
                float orig_pitch;
                float orig_roll;
                float orig_yaw;
                float orig_wind;
                float mod_thrust;
                float mod_pitch;
                float mod_roll;
                float mod_yaw;
            };
            std::unordered_map<int, ModValues> m_saved;

        public:
            explicit CommandHeliAutoStabilisation(CommandList* parent)
                : CommandToggle(parent, LIT("Disable Heli Auto-Stabilisation"), CMDNAMES("disableheliautostabilisation", "heliautostabilisation")) {}

            void onChange(Click& click) override
            {
                if (!m_on)
                {
                    click.ensureScriptThread([this] { RestoreAll(); });
                    return;
                }
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on)
                    {
                        RestoreAll();
                        return false;
                    }
                    int veh = MovVehicle();
                    if (!veh || !VEHICLE::IS_THIS_MODEL_A_HELI(ENTITY::GET_ENTITY_MODEL(veh)))
                        return true;
                    void* cveh = GetVehPtr(veh);
                    if (!cveh) return true;
                    void* hdata = GetHandlingData(cveh);
                    if (!hdata) return true;
                    void* fly = FindSubHandling(hdata, 1);
                    if (!fly) fly = FindSubHandling(hdata, 2);
                    if (!fly) return true;

                    if (m_saved.find(veh) == m_saved.end())
                    {
                        float t = FieldAt<float>(fly, 0x08);
                        float p = FieldAt<float>(fly, 0x3C);
                        float r = FieldAt<float>(fly, 0x30);
                        float y = FieldAt<float>(fly, 0x20);
                        m_saved[veh] = {
                            t,
                            FieldAt<float>(fly, 0x48),
                            FieldAt<float>(fly, 0x4C),
                            p, r, y,
                            FieldAt<float>(fly, 0x58),
                            t * 1.35f,
                            p * 0.01f,
                            r * 0.01f,
                            y * 0.01f,
                        };
                    }

                    const auto& mv = m_saved[veh];
                    FieldAt<float>(fly, 0x08) = mv.mod_thrust;
                    FieldAt<float>(fly, 0x48) = 0.f;
                    FieldAt<float>(fly, 0x4C) = 0.f;
                    FieldAt<float>(fly, 0x3C) = mv.mod_pitch;
                    FieldAt<float>(fly, 0x30) = mv.mod_roll;
                    FieldAt<float>(fly, 0x20) = mv.mod_yaw;
                    FieldAt<float>(fly, 0x58) = 0.f;
                    return true;
                });
            }

        private:
            void RestoreAll()
            {
                for (auto& [veh, mv] : m_saved)
                {
                    void* cveh = GetVehPtr(veh);
                    if (!cveh) continue;
                    void* hdata = GetHandlingData(cveh);
                    if (!hdata) continue;
                    void* fly = FindSubHandling(hdata, 1);
                    if (!fly) fly = FindSubHandling(hdata, 2);
                    if (!fly) continue;
                    FieldAt<float>(fly, 0x08) = mv.orig_thrust;
                    FieldAt<float>(fly, 0x48) = mv.orig_attack_lift;
                    FieldAt<float>(fly, 0x4C) = mv.orig_attack_dive;
                    FieldAt<float>(fly, 0x3C) = mv.orig_pitch;
                    FieldAt<float>(fly, 0x30) = mv.orig_roll;
                    FieldAt<float>(fly, 0x20) = mv.orig_yaw;
                    FieldAt<float>(fly, 0x58) = mv.orig_wind;
                }
                m_saved.clear();
            }
        };

        class CommandGravityMultSlider : public CommandSliderFloat
        {
            CommandToggle* m_not_in_aircraft = nullptr;
            bool m_ticking = false;

        public:
            explicit CommandGravityMultSlider(CommandList* parent)
                : CommandSliderFloat(parent, LIT("Gravity Multiplier"), CMDNAMES("gravitymultiplier"),
                    NOLABEL, -10000, 10000, 100, 10) {}

            void SetNotInAircraft(CommandToggle* t) { m_not_in_aircraft = t; }

            ~CommandGravityMultSlider() override
            {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            void onTick() override
            {
                int veh = MovVehicle();
                if (!veh) return;
                void* cveh = GetVehPtr(veh);
                if (!cveh) return;
                float grav = (float(value) / 100.f) * 9.8000002f;
                if (m_not_in_aircraft && m_not_in_aircraft->m_on &&
                    VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(veh)))
                    grav = 9.8000002f;
                FieldAt<float>(cveh, 0x0C9C) = grav;
            }

            void onChange(Click& click, int) override
            {
                if (click.isAuto()) return;
                if (value != 100)
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
                        if (!veh) return;
                        void* cveh = GetVehPtr(veh);
                        if (cveh) FieldAt<float>(cveh, 0x0C9C) = 9.8000002f;
                    });
                }
            }
        };

        class CommandListGravityMult : public CommandList
        {
        public:
            explicit CommandListGravityMult(CommandList* parent)
                : CommandList(parent, LIT("Gravity Multiplier"), CMDNAMES_0())
            {
                auto* slider = createChild<CommandGravityMultSlider>();
                auto* notAircraft = createChild<CommandToggle>(
                    LIT("Not In Aircraft"), CMDNAMES("gravitynotinaircraft"),
                    LIT("Skips the gravity multiplier for aircraft, keeping them at normal gravity."));
                slider->SetNotInAircraft(notAircraft);
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

        class CommandLessenTyreBurnouts : public CommandToggle
        {
            float m_saved = 1.f;
            bool m_applied = false;

        public:
            explicit CommandLessenTyreBurnouts(CommandList* parent)
                : CommandToggle(parent, LIT("Lessen Tyre Burnouts"), CMDNAMES("launchcontrol", "lessentyreburnouts", "lessenburnouts")) {}

            void onChange(Click& click) override
            {
                click.ensureScriptThread([this] {
                    int veh = MovVehicle();
                    if (!veh) return;
                    void* cveh = GetVehPtr(veh);
                    if (!cveh) return;
                    void* hdata = GetHandlingData(cveh);
                    if (!hdata) return;
                    float& mult = FieldAt<float>(hdata, 0x0A8);
                    if (m_on)
                    {
                        if (mult != 0.f) m_saved = mult;
                        mult = 0.f;
                        m_applied = true;
                    }
                    else if (m_applied)
                    {
                        mult = m_saved;
                        m_saved = 1.f;
                        m_applied = false;
                    }
                });
            }
        };

        class CommandSmoothCoasting : public CommandToggle
        {
            uint32_t m_saved_flags = 0;
            bool m_applied = false;

        public:
            explicit CommandSmoothCoasting(CommandList* parent)
                : CommandToggle(parent, LIT("Smooth Coasting"), CMDNAMES("smoothcoasting"),
                    LIT("Disables engine braking when no throttle is applied.")) {}

            void onChange(Click& click) override
            {
                click.ensureScriptThread([this] {
                    int veh = MovVehicle();
                    if (!veh) return;
                    void* cveh = GetVehPtr(veh);
                    if (!cveh) return;
                    void* hdata = GetHandlingData(cveh);
                    if (!hdata) return;
                    uint32_t& flags = FieldAt<uint32_t>(hdata, 0x128);
                    if (m_on)
                    {
                        m_saved_flags = flags;
                        flags |= (1u << 8);
                        m_applied = true;
                    }
                    else if (m_applied)
                    {
                        flags = m_saved_flags;
                        m_applied = false;
                    }
                });
            }
        };

        class CommandDriftMode : public CommandToggle
        {
            std::unordered_map<uintptr_t, float> m_saved_floats;
            std::unordered_map<uintptr_t, uint32_t> m_saved_flags;
            bool m_applied = false;

            template<typename T>
            void SaveAndSet(void* base, size_t off, T val,
                std::unordered_map<uintptr_t, T>& store)
            {
                uintptr_t key = reinterpret_cast<uintptr_t>(base) + off;
                T& field = FieldAt<T>(base, off);
                store.emplace(key, field);
                field = val;
            }

        public:
            explicit CommandDriftMode(CommandList* parent)
                : CommandToggle(parent, LIT("Drift Mode"), CMDNAMES("driftmode")) {}

            void onChange(Click& click) override
            {
                click.ensureScriptThread([this] {
                    int veh = MovVehicle();
                    if (!veh) return;
                    void* cveh = GetVehPtr(veh);
                    if (!cveh) return;
                    void* hdata = GetHandlingData(cveh);
                    if (!hdata) return;

                    if (m_on)
                    {
                        if (m_applied) return;
                        m_applied = true;

                        static constexpr uint32_t kDriftFlags =
                            (1u<<4)|(1u<<6)|(1u<<8)|(1u<<9)|(1u<<13)|(1u<<15)|(1u<<18)|(1u<<20)|(1u<<22);

                        void* car_sub = FindSubHandling(hdata, 8);
                        if (car_sub)
                            SaveAndSet(car_sub, 0x3C, kDriftFlags, m_saved_flags);

                        float speed = ENTITY::GET_ENTITY_SPEED(veh);
                        float base_min = 1.625f, base_max = 1.63f;
                        if (speed < 4.5f)       { base_min *= 1.50f; base_max *= 1.50f; }
                        else if (speed < 8.f)   { base_min *= 1.30f; base_max *= 1.30f; }
                        else if (speed < 13.f)  { base_min *= 1.15f; base_max *= 1.15f; }

                        SaveAndSet(hdata, 0x04C, 1.65f,    m_saved_floats);
                        SaveAndSet(hdata, 0x0BC, 2.0f,     m_saved_floats);
                        SaveAndSet(hdata, 0x0D0, 0.025f,   m_saved_floats);
                        SaveAndSet(hdata, 0x0C0, 0.1f,     m_saved_floats);
                        SaveAndSet(hdata, 0x0B8, 1.0f,     m_saved_floats);
                        SaveAndSet(hdata, 0x058, 4.8f,     m_saved_floats);
                        SaveAndSet(hdata, 0x05C, 4.7f,     m_saved_floats);
                        SaveAndSet(hdata, 0x090, base_min, m_saved_floats);
                        SaveAndSet(hdata, 0x088, base_max, m_saved_floats);
                        SaveAndSet(hdata, 0x044, 1.1764f,  m_saved_floats);
                        SaveAndSet(hdata, 0x048, 0.35f,    m_saved_floats);
                        SaveAndSet(hdata, 0x054, 1.0f,     m_saved_floats);
                        SaveAndSet(hdata, 0x060, 0.7059f,  m_saved_floats);
                        SaveAndSet(hdata, 0x064, 44.1666f, m_saved_floats);
                        SaveAndSet(hdata, 0x068, 36.8055f, m_saved_floats);
                        SaveAndSet(hdata, 0x06C, 0.8750f,  m_saved_floats);
                        SaveAndSet(hdata, 0x074, 0.64f,    m_saved_floats);
                        SaveAndSet(hdata, 0x078, 1.36f,    m_saved_floats);
                        SaveAndSet(hdata, 0x07C, 0.9f,     m_saved_floats);
                        SaveAndSet(hdata, 0x080, 1.3482f,  m_saved_floats);
                        SaveAndSet(hdata, 0x084, 0.7416f,  m_saved_floats);
                        SaveAndSet(hdata, 0x08C, 0.6535f,  m_saved_floats);
                        SaveAndSet(hdata, 0x098, 0.5139f,  m_saved_floats);
                        SaveAndSet(hdata, 0x09C, 1.9455f,  m_saved_floats);
                        SaveAndSet(hdata, 0x0A0, 0.1500f,  m_saved_floats);
                        SaveAndSet(hdata, 0x0A4, 6.6666f,  m_saved_floats);
                        SaveAndSet(hdata, 0x0A8, 0.0f,     m_saved_floats);
                        SaveAndSet(hdata, 0x0AC, 0.0f,     m_saved_floats);
                        SaveAndSet(hdata, 0x0B0, 0.9240f,  m_saved_floats);
                        SaveAndSet(hdata, 0x0B4, 1.076f,   m_saved_floats);
                        SaveAndSet(hdata, 0x0C4, 0.155f,   m_saved_floats);
                        SaveAndSet(hdata, 0x0C8, 0.0675f,  m_saved_floats);
                        SaveAndSet(hdata, 0x0CC, -0.1f,    m_saved_floats);
                        SaveAndSet(hdata, 0x0D4, 1.04f,    m_saved_floats);
                        SaveAndSet(hdata, 0x0D8, 0.96f,    m_saved_floats);

                        VEHICLE::MODIFY_VEHICLE_TOP_SPEED(veh, 30.0f);
                    }
                    else if (m_applied)
                    {
                        for (auto& [key, val] : m_saved_floats)
                            *reinterpret_cast<float*>(key) = val;
                        for (auto& [key, val] : m_saved_flags)
                            *reinterpret_cast<uint32_t*>(key) = val;
                        m_saved_floats.clear();
                        m_saved_flags.clear();
                        m_applied = false;
                        VEHICLE::MODIFY_VEHICLE_TOP_SPEED(veh, 10.0f);
                    }
                });
            }
        };

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
                createChild<CommandHECarField<0x08>>(LIT("Max Steer Angle"),     CMDNAMES("hecarsteer"),     0,   9000, 3500, 100);
                createChild<CommandHECarField<0x0C>>(LIT("Max Steer Angle Rear"),CMDNAMES("hecarsteerrear"), 0,   9000,    0, 100);
                createChild<CommandHECarField<0x10>>(LIT("Max Handbrake Torque"),CMDNAMES("hecarhbktorque"),  0,  50000, 5000, 100);
                createChild<CommandHECarField<0x14>>(LIT("Max Braking Decel"),   CMDNAMES("hecarbrakdecel"),  0,  10000, 1000, 100);
                createChild<CommandHECarField<0x18>>(LIT("Max Lat Decel"),       CMDNAMES("hecarlat"),        0,  10000, 1500, 100);
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

        class CommandCustomWingsBehaviour : public CommandToggle
        {
            float m_pos = 0.f;
            bool  m_dir = true;
        public:
            CommandSlider* m_min   = nullptr;
            CommandSlider* m_max   = nullptr;
            CommandSlider* m_speed = nullptr;

            explicit CommandCustomWingsBehaviour(CommandList* parent)
                : CommandToggle(parent, LIT("Custom Wings Behaviour"), CMDNAMES("wingsmod"),
                    LIT("Allows you to customise the behaviour of the wings on the original oppressor.")) {}

            void onChange(Click& click) override
            {
                if (!m_on) return;
                m_pos = m_min ? (float)m_min->value / 15.f : 0.f;
                m_dir = true;
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int veh = MovVehicle();
                    if (!veh) return true;
                    float mn  = m_min  ? (float)m_min->value  / 15.f : 0.f;
                    float mx  = m_max  ? (float)m_max->value  / 15.f : 1.f;
                    float spd = m_speed ? (float)m_speed->value / 300.f : 0.017f;
                    if (mx < mn) mx = mn;
                    if (m_dir)
                    {
                        m_pos += spd;
                        if (m_pos >= mx) { m_pos = mx; m_dir = false; }
                    }
                    else
                    {
                        m_pos -= spd;
                        if (m_pos <= mn) { m_pos = mn; m_dir = true; }
                    }
                    VEHICLE::SET_VEHICLE_FLIGHT_NOZZLE_POSITION(veh, m_pos);
                    return true;
                });
            }
        };

        class CommandWingsSetSpeedFromMax : public CommandPhysical
        {
            CommandSlider* m_max;
            CommandSlider* m_speed;
        public:
            CommandWingsSetSpeedFromMax(CommandList* parent, CommandSlider* max_s, CommandSlider* spd_s)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Set Speed Based On Max Position"), CMDNAMES_0(), NOLABEL)
                , m_max(max_s), m_speed(spd_s) {}

            void onClick(Click& click) override
            {
                auto* mx = m_max; auto* spd = m_speed;
                click.ensureScriptThread([mx, spd] {
                    if (!mx || !spd) return;
                    int new_val = mx->value / 3;
                    if (spd->value != new_val)
                    {
                        int prev = spd->value;
                        spd->value = new_val;
                        Click ac(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                        spd->onChange(ac, prev);
                    }
                });
            }
        };

        class CommandListCustomWingsBehaviour : public CommandList
        {
        public:
            explicit CommandListCustomWingsBehaviour(CommandList* parent)
                : CommandList(parent, LIT("Custom Wings Behaviour"), CMDNAMES_0())
            {
                auto* tog = createChild<CommandCustomWingsBehaviour>();
                auto* mn  = createChild<CommandSlider>(LIT("Min Position"), CMDNAMES("wingsminpos"), NOLABEL, 0, 15,  0, 1);
                auto* mx  = createChild<CommandSlider>(LIT("Max Position"), CMDNAMES("wingsmaxpos"), NOLABEL, 0, 15, 15, 1);
                auto* spd = createChild<CommandSlider>(LIT("Speed"),        CMDNAMES("wingsspeed"),  NOLABEL, 0, 15,  5, 1);
                createChild<CommandWingsSetSpeedFromMax>(mx, spd);
                tog->m_min   = mn;
                tog->m_max   = mx;
                tog->m_speed = spd;
            }
        };

        class CommandSpecialFlight : public CommandSlider
        {
            bool m_ticking = false;
        public:
            explicit CommandSpecialFlight(CommandList* parent)
                : CommandSlider(parent, LIT("Special Flight"), CMDNAMES("specialflight"),
                    NOLABEL, 0, 2, 0, 1) {}

            ~CommandSpecialFlight() override
            {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            std::string getValueText() const override
            {
                switch (value)
                {
                case 1: return "Enabled";
                case 2: return "Disabled";
                default: return "Don't Override";
                }
            }

            void onTick() override
            {
                int veh = MovVehicle();
                if (!veh) return;
                void* cveh = GetVehPtr(veh);
                if (!cveh) return;
                void* model = FieldAt<void*>(cveh, 0x20);
                if (!model) return;
                if (value == 1)
                    FieldAt<uint8_t>(model, 0x57C + 19) |= static_cast<uint8_t>(1u << 0);
                else
                    FieldAt<uint8_t>(model, 0x57C + 19) &= static_cast<uint8_t>(~(1u << 0));
            }

            void onChange(Click& click, int) override
            {
                if (value == 0)
                {
                    if (m_ticking) { CommandTickDispatch::RemoveCommand(this); m_ticking = false; }
                }
                else
                {
                    if (!m_ticking) { CommandTickDispatch::AddCommand(this); m_ticking = true; }
                }
            }
        };

        class CommandJumpAbility : public CommandSlider
        {
            bool m_ticking = false;
        public:
            explicit CommandJumpAbility(CommandList* parent)
                : CommandSlider(parent, LIT("Jump Ability"), CMDNAMES("vehjump"),
                    NOLABEL, 0, 2, 0, 1) {}

            ~CommandJumpAbility() override
            {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            std::string getValueText() const override
            {
                switch (value)
                {
                case 1: return "Enabled";
                case 2: return "Disabled";
                default: return "Don't Override";
                }
            }

            void onTick() override
            {
                int veh = MovVehicle();
                if (!veh) return;
                void* cveh = GetVehPtr(veh);
                if (!cveh) return;
                void* model = FieldAt<void*>(cveh, 0x20);
                if (!model) return;
                if (value == 1)
                    FieldAt<uint8_t>(model, 0x57C + 15) |= static_cast<uint8_t>(1u << 5);
                else
                    FieldAt<uint8_t>(model, 0x57C + 15) &= static_cast<uint8_t>(~(1u << 5));
            }

            void onChange(Click& click, int) override
            {
                if (value == 0)
                {
                    if (m_ticking) { CommandTickDispatch::RemoveCommand(this); m_ticking = false; }
                }
                else
                {
                    if (!m_ticking) { CommandTickDispatch::AddCommand(this); m_ticking = true; }
                }
            }
        };

        class CommandGlideAbility : public CommandSlider
        {
            bool m_ticking = false;
        public:
            explicit CommandGlideAbility(CommandList* parent)
                : CommandSlider(parent, LIT("Glide Ability"), CMDNAMES("vehglide"),
                    NOLABEL, 0, 2, 0, 1) {}

            ~CommandGlideAbility() override
            {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            std::string getValueText() const override
            {
                switch (value)
                {
                case 1: return "Enabled";
                case 2: return "Disabled";
                default: return "Don't Override";
                }
            }

            void onTick() override
            {
                int veh = MovVehicle();
                if (!veh) return;
                void* cveh = GetVehPtr(veh);
                if (!cveh) return;
                void* model = FieldAt<void*>(cveh, 0x20);
                if (!model) return;
                if (value == 1)
                    FieldAt<uint8_t>(model, 0x57C + 17) |= static_cast<uint8_t>(1u << 1);
                else
                    FieldAt<uint8_t>(model, 0x57C + 17) &= static_cast<uint8_t>(~(1u << 1));
            }

            void onChange(Click& click, int) override
            {
                if (value == 0)
                {
                    if (m_ticking) { CommandTickDispatch::RemoveCommand(this); m_ticking = false; }
                }
                else
                {
                    if (!m_ticking) { CommandTickDispatch::AddCommand(this); m_ticking = true; }
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
        createChild<CommandDriveUnderwater>();
        createChild<CommandSpeedLimit>();
        createChild<CommandListHandlingEditor>();
        createChild<CommandSpecialFlight>();
        createChild<CommandJumpAbility>();
        createChild<CommandGlideAbility>();
        createChild<CommandNoTurbulence>();
        createChild<CommandHeliAutoStabilisation>();
        createChild<CommandListGravityMult>();
        createChild<CommandEnginePowerMult>();
        createChild<CommandListCustomWingsBehaviour>();
        createChild<CommandLessenTyreBurnouts>();
        createChild<CommandSmoothCoasting>();
        createChild<CommandDriftMode>();
    }

}

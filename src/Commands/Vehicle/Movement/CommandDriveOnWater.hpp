#pragma once
#include "Commands/Vehicle/Movement/CommandMovementShared.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Util/Joaat.hpp"

namespace Stand
{
    namespace
    {
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
    }
}

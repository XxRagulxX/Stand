#include "Commands/Vehicle/PersonalVehicles/CommandCurrentPV.hpp"

#include "Commands/CommandLegacy.hpp"
#include "Core/Pointers.hpp"
#include "Rendering/Notifications.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/ScriptLocal.hpp"
#include "Util/Joaat.hpp"
#include "Vehicle/PersonalVehicles.hpp"
#include "World/Self.hpp"

namespace Stand
{
    namespace
    {
        class CommandPersonalVehicle : public CommandLegacy
        {
            using CommandLegacy::CommandLegacy;
            void OnCall() override
            {
                if (!*Pointers.IsSessionStarted) return;
                FiberPool::queueJob([] { Features::CallPersonalVehicle(); });
            }
        };
        static CommandPersonalVehicle s_PersonalVehicle{
            "personalvehicle", "Current Personal Vehicle",
            "Requests your current personal vehicle and brings it to you."
        };

        class CommandCallPV : public CommandLegacy
        {
            using CommandLegacy::CommandLegacy;
            void OnCall() override
            {
                if (!*Pointers.IsSessionStarted) return;
                FiberPool::queueJob([] { Features::CallPersonalVehicle(); });
            }
        };
        static CommandCallPV s_CallPV{
            "callpersonalvehicle", "Teleport To Me & Drive",
            "Teleports your personal vehicle to you and puts you in the driver seat."
        };

        class CommandEnterPV : public CommandLegacy
        {
            using CommandLegacy::CommandLegacy;
            void OnCall() override
            {
                if (!*Pointers.IsSessionStarted) return;
                FiberPool::queueJob([] { Features::EnterPersonalVehicle(); });
            }
        };
        static CommandEnterPV s_EnterPV{
            "enterpersonalvehicle", "Enter",
            "Enters the currently spawned personal vehicle."
        };

        class CommandReturnPV : public CommandLegacy
        {
            using CommandLegacy::CommandLegacy;
            void OnCall() override
            {
                if (!*Pointers.IsSessionStarted) return;
                FiberPool::queueJob([] { Features::ReturnPVToStorage(); });
            }
        };
        static CommandReturnPV s_ReturnPV{
            "returnpv", "Return To Storage",
            "Returns your personal vehicle to storage."
        };
    }
}

namespace Stand::Features
{
    void CallPersonalVehicle()
    {
        auto pv = PersonalVehicles::GetCurrent();
        if (!pv)
        {
            Notifications::Show("Personal Vehicle", "No personal vehicle found.");
            return;
        }
        pv->Request(true);
    }

    void EnterPersonalVehicle()
    {
        auto veh = PersonalVehicles::GetCurrentHandle();
        if (!veh.IsValid())
        {
            Notifications::Show("Personal Vehicle", "Personal vehicle is not spawned.");
            return;
        }
        Self::GetPed().SetInVehicle(veh);
    }

    void ReturnPVToStorage()
    {
        auto pv = PersonalVehicles::GetCurrent();
        if (!pv)
        {
            Notifications::Show("Personal Vehicle", "No personal vehicle found.");
            return;
        }
        pv->Despawn();
    }

    void ImaniTechRemoteControl()
    {
        if (!*Pointers.IsSessionStarted) return;

        auto veh = PersonalVehicles::GetCurrentHandle();
        if (!veh.IsValid())
        {
            Notifications::Show("Personal Vehicle", "Personal vehicle is not spawned.");
            return;
        }

        const int h = veh.GetHandle();
        bool hasImaniTech = false;
        for (int slot = 0; slot < 50; ++slot)
        {
            const char* slotName = VEHICLE::GET_MOD_SLOT_NAME(h, slot);
            if (slotName && MISC::GET_HASH_KEY(slotName) == "TOP_IMANI_TECH"_J)
            {
                hasImaniTech = true;
                break;
            }
        }

        if (!hasImaniTech)
        {
            Notifications::Show("Personal Vehicle", "This vehicle does not have Imani Tech.");
            return;
        }

        *ScriptLocal("freemode"_J, 19672).at(179).as<int*>() = 1;
    }
}

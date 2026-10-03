#include "Commands/World/CommandArGps.hpp"

#include "Core/Pointers.hpp"
#include "Game/CGpsSlot.hpp"
#include "Scripting/Natives.hpp"

namespace Stand
{
    CommandArGps::CommandArGps(CommandList* parent)
        : CommandToggle(parent, LIT("AR GPS"), CMDNAMES("argps"),
            LIT("Navigates you to your waypoints."))
    {
    }

    CommandArGps::~CommandArGps()
    {
        if (m_ticking) CommandTickDispatch::RemoveCommand(this);
    }

    void CommandArGps::onEnable(Click& click)
    {
        CommandTickDispatch::AddCommand(this);
        m_ticking = true;
    }

    void CommandArGps::onDisable(Click& click)
    {
        CommandTickDispatch::RemoveCommand(this);
        m_ticking = false;
    }

    void CommandArGps::onTick()
    {
        if (!Pointers.gps_slots)
            return;

        CGpsSlot& slot = Pointers.gps_slots[0];
        if (slot.m_NumNodes == 0)
            return;

        const Ped ped = PLAYER::GET_PLAYER_PED(-1);
        if (!PED::IS_PED_IN_ANY_VEHICLE(ped, FALSE))
            return;

        int i = slot.m_NumNodes - 1;
        rage::vector4 pos = slot.m_NodeCoordinates[i];

        do
        {
            if (*reinterpret_cast<const int*>(&slot.m_NodeCoordinates[i].w) == static_cast<int>(GNI_IGNORE_FOR_NAV))
                continue;

            GRAPHICS::DRAW_LINE(
                pos.x, pos.y, pos.z,
                slot.m_NodeCoordinates[i].x, slot.m_NodeCoordinates[i].y, slot.m_NodeCoordinates[i].z + 1.0f,
                255, 0, 255, 200
            );
            pos = slot.m_NodeCoordinates[i];
            pos.z += 1.0f;
        } while (i-- != 0);
    }
}

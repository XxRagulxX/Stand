#include "Commands/World/CommandArGps.hpp"

#include "Core/Pointers.hpp"
#include "Game/CGpsSlot.hpp"
#include "Scripting/Natives.hpp"

namespace Stand {
    CommandArGps::CommandArGps(CommandList* parent)
        : CommandToggle(parent, LIT("AR GPS"), CMDNAMES("argps"),
            LIT("Navigates you to your waypoints.")) {}

    CommandArGps::~CommandArGps() {
        if (m_ticking) CommandTickDispatch::RemoveCommand(this);
    }

    void CommandArGps::onEnable(Click& click) {
        CommandTickDispatch::AddCommand(this);
        m_ticking = true;
    }

    void CommandArGps::onDisable(Click& click) {
        CommandTickDispatch::RemoveCommand(this);
        m_ticking = false;
    }

    void CommandArGps::onTick()
    {
        // Rework needed: CGpsSlot::m_NodeCoordinates offset and element stride are
        // unconfirmed for Enhanced — accessing the array causes ACCESS_VIOLATION.
        // See README "Future - Rework needed" section.
        (void)Pointers.gps_slots;
    }
}

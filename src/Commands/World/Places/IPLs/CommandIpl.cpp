#include "Commands/World/Places/IPLs/CommandIpl.hpp"
#include "Commands/Stand/CommandToggleNoCorrelation.hpp"
#include "Commands/World/Places/TpUtil.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Menu/Click.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandIpl::CommandIpl(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names,
                           Vector3 coords,
                           std::vector<const char*>&& request,
                           std::vector<const char*>&& remove,
                           std::vector<const char*>&& remove_enable_only,
                           bool needs_mp_world_state,
                           CommandToggleNoCorrelation* tp_toggle)
        : CommandToggle(parent, std::move(menu_name), std::move(command_names),
                        needs_mp_world_state ? LIT("Enabling this IPL will ensure the Online world state which might make the game unresponsive shortly.") : NOLABEL)
        , coords(coords)
        , m_request(std::move(request))
        , m_remove(std::move(remove))
        , m_remove_enable_only(std::move(remove_enable_only))
        , m_tp_toggle(tp_toggle)
    {
    }

    void CommandIpl::onEnable(Click& click)
    {
        click.ensureScriptThread([this] {
            for (const auto& ipl : m_request)
                STREAMING::REQUEST_IPL(ipl);
            for (const auto& ipl : m_remove_enable_only)
                STREAMING::REMOVE_IPL(ipl);
            if (m_tp_toggle->m_on)
                TpUtil::DoTeleport(coords.x, coords.y, coords.z);
        });
    }

    void CommandIpl::onDisable(Click& click)
    {
        FiberPool::queueJob([this] {
            for (const auto& ipl : m_request)
                STREAMING::REMOVE_IPL(ipl);
            for (const auto& ipl : m_remove)
                STREAMING::REQUEST_IPL(ipl);
        });
    }
}

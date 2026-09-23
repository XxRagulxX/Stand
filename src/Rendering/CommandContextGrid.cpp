#include "Rendering/CommandContextGrid.hpp"

#include "Commands/Widgets/Command.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Core/ThreadContext.hpp"
#include "Menu/Click.hpp"
#include "Rendering/Clipboard.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/HotkeysGrid.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <memory>
#include <vector>

namespace Stand::Rendering
{
    namespace
    {
        constexpr float kItemH = Theme::kContentItemHeight;
        constexpr int16_t kW   = Theme::kContentWidth;

        HotkeysGrid g_ContextHotkeys{};
    }

    CommandContextGrid::CommandContextGrid(Stand::Command* cmd)
        : Grid(Theme::GetContentOrigin(), 0), m_Command(cmd)
    {
    }

    Grid& CommandContextGrid::GetOrCreate(Stand::Command* cmd)
    {
        static std::unique_ptr<CommandContextGrid> s_Instance;
        s_Instance = std::make_unique<CommandContextGrid>(cmd);
        return *s_Instance;
    }

    std::string CommandContextGrid::BuildPath(Stand::Command* cmd)
    {
        std::vector<std::string> parts;
        Stand::Command* node = cmd;
        while (node)
        {
            if (auto* p = node->getPhysical())
                parts.push_back(p->getMenuName().getLocalisedUtf8());
            node = node->parent;
        }
        std::reverse(parts.begin(), parts.end());
        std::string result;
        for (const auto& part : parts)
        {
            if (!result.empty()) result += " / ";
            result += part;
        }
        return result;
    }

    void CommandContextGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
    {
        auto* physical = m_Command->getPhysical();
        if (!physical)
            return;

        auto* cmd     = m_Command;
        auto* phys    = physical;

        items_draft.push_back(std::make_unique<GridItemText>(
            kW, kItemH,
            phys->getMenuName().getLocalisedUtf8(),
            Theme::kText));

        items_draft.push_back(std::make_unique<GridItemButton>(
            kW, kItemH, "Save State",
            [cmd, phys] {
                SavedStates()[cmd] = phys->getState();
            }));

        items_draft.push_back(std::make_unique<GridItemButton>(
            kW, kItemH, "Load State",
            [phys, cmd] {
                auto it = SavedStates().find(cmd);
                if (it == SavedStates().end()) return;
                const std::string state = it->second;
                phys->ensureScriptThread([phys, state] {
                    Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                    phys->setState(click, state);
                });
            }));

        items_draft.push_back(std::make_unique<GridItemButton>(
            kW, kItemH, "Apply Default State",
            [phys] {
                phys->ensureScriptThread([phys] {
                    phys->applyDefaultState();
                });
            }));

        items_draft.push_back(std::make_unique<GridItemFolder>(
            kW, kItemH, "Hotkeys", &g_ContextHotkeys));

        const std::string path = BuildPath(m_Command);
        items_draft.push_back(std::make_unique<GridItemButton>(
            kW, kItemH, "Copy Address",
            [path] {
                Clipboard::SetText(path);
            },
            [path] { return path; }));
    }
}

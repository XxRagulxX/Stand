#include "Rendering/CommandContextGrid.hpp"

#include "Commands/Widgets/Command.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/Widgets/ToggleCorrelation.hpp"
#include "Core/ThreadContext.hpp"
#include "Menu/Click.hpp"
#include "Rendering/Clipboard.hpp"
#include "Rendering/CtxHotkeysGrid.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/Gui.hpp"
#include "Rendering/StandPort/GridItemText.hpp"
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

        CtxHotkeysGrid g_CtxHotkeys{};

        static const char* kCorrelationNames[] = {
            "Off", "Menu Open", "On Foot", "Aiming", "Freeroam", "Chatting", "Session Host"
        };
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

    std::string CommandContextGrid::BuildDefaultPath(Stand::Command* cmd)
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
            if (!result.empty()) result += " > ";
            result += part;
        }
        return result;
    }

    std::string CommandContextGrid::BuildApiPath(Stand::Command* cmd)
    {
        std::vector<std::string> parts;
        Stand::Command* node = cmd;
        while (node)
        {
            if (auto* p = node->getPhysical())
            {
                if (!p->command_names.empty())
                    parts.push_back(p->command_names[0]);
                else
                    parts.push_back(p->menu_name.getLocalisedUtf8());
            }
            node = node->parent;
        }
        std::reverse(parts.begin(), parts.end());
        std::string result;
        for (const auto& part : parts)
        {
            if (!result.empty()) result += ".";
            result += part;
        }
        return result;
    }

    void CommandContextGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
    {
        auto* physical = m_Command->getPhysical();
        if (!physical)
            return;

        auto* cmd  = m_Command;
        auto* phys = physical;

        items_draft.push_back(std::make_unique<GridItemText>(
            kW, kItemH,
            phys->getMenuName().getLocalisedUtf8(),
            Theme::kText));

        auto* stateCmd = phys->getStateCommand();

        // Toggle correlation — flat at root level, only when stateCommand == target
        if (phys->isToggle() && stateCmd == phys)
        {
            auto* toggle = static_cast<CommandToggle*>(phys);

            items_draft.push_back(std::make_unique<GridItemButton>(
                kW, kItemH, "Auto State",
                [toggle] {
                    int next = ((int)toggle->correlation.type + 1) % (int)ToggleCorrelation::_NUM_TOGGLE_CORRELATIONS;
                    Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                    toggle->setCorrelation(click, (ToggleCorrelation::Type)next, toggle->correlation.invert);
                },
                [toggle]() -> std::string {
                    return kCorrelationNames[(int)toggle->correlation.type];
                }));

            items_draft.push_back(std::make_unique<GridItemButton>(
                kW, kItemH, "Invert Auto State",
                [toggle] {
                    Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                    toggle->setCorrelation(click, toggle->correlation.type, !toggle->correlation.invert);
                },
                [toggle]() -> std::string { return toggle->correlation.invert ? "On" : "Off"; }));
        }

        // State operations
        if (stateCmd)
        {
            if (g_gui.active_profile.isInitialised()
                && !(stateCmd->flags & CMDFLAG_TEMPORARY)
                && stateCmd->supportsSavedState())
            {
                if (!g_gui.isUsingAutosaveState())
                {
                    items_draft.push_back(std::make_unique<GridItemButton>(
                        kW, kItemH, "Save State",
                        [cmd, stateCmd] { SavedStates()[cmd] = stateCmd->getState(); }));
                }
                items_draft.push_back(std::make_unique<GridItemButton>(
                    kW, kItemH, "Load State",
                    [stateCmd, cmd] {
                        auto it = SavedStates().find(cmd);
                        if (it == SavedStates().end()) return;
                        const std::string state = it->second;
                        stateCmd->ensureScriptThread([stateCmd, state] {
                            Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                            stateCmd->setState(click, state);
                        });
                    }));
            }
            items_draft.push_back(std::make_unique<GridItemButton>(
                kW, kItemH, "Apply Default State",
                [stateCmd] {
                    stateCmd->ensureScriptThread([stateCmd] { stateCmd->applyDefaultState(); });
                }));
        }

        // Apply Default State to Children (lists only)
        if (phys->isListNonAction())
        {
            auto* list = static_cast<CommandList*>(phys);
            items_draft.push_back(std::make_unique<GridItemButton>(
                kW, kItemH, "Apply Default State to Children",
                [list] {
                    list->ensureScriptThread([list] { list->recursivelyApplyDefaultState(); });
                }));
        }

        // Slider Min / Max
        if (phys->isSlider())
        {
            auto* slider = static_cast<CommandSlider*>(phys);
            items_draft.push_back(std::make_unique<GridItemButton>(
                kW, kItemH, "Min",
                [slider] {
                    slider->ensureScriptThread([slider] {
                        Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                        slider->setValue(click, slider->min_value);
                    });
                }));
            items_draft.push_back(std::make_unique<GridItemButton>(
                kW, kItemH, "Max",
                [slider] {
                    slider->ensureScriptThread([slider] {
                        Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                        slider->setValue(click, slider->max_value);
                    });
                }));
        }

        // Hotkeys
        if (!(phys->flags & CMDFLAG_TEMPORARY) && phys->canBeResolved())
        {
            g_CtxHotkeys.SetTarget(phys);
            items_draft.push_back(std::make_unique<GridItemFolder>(
                kW, kItemH, "Hotkeys", &g_CtxHotkeys));
        }

        // Star
        if (!(phys->flags & CMDFLAG_TEMPORARY) && phys->canBeResolved())
        {
            const std::string pathKey = BuildPath(m_Command);
            items_draft.push_back(std::make_unique<GridItemButton>(
                kW, kItemH, "Star",
                [pathKey] {
                    auto& stars = g_gui.starred_commands;
                    if (stars.count(pathKey))
                        stars.erase(pathKey);
                    else
                        stars.emplace(pathKey, "Saved");
                },
                [pathKey]() -> std::string {
                    return g_gui.starred_commands.count(pathKey) ? "On" : "Off";
                }));
        }

        // Copy Address (4 modes) — always shown
        {
            const std::string userPath    = BuildPath(m_Command);
            const std::string defaultPath = BuildDefaultPath(m_Command);
            const std::string apiPath     = BuildApiPath(m_Command);
            const std::string linkUrl     = "https://stand.sh/focus#" + apiPath;
            items_draft.push_back(std::make_unique<GridItemButton>(
                kW, kItemH, "Copy Address (User)",
                [userPath]    { Clipboard::SetText(userPath); }));
            items_draft.push_back(std::make_unique<GridItemButton>(
                kW, kItemH, "Copy Address (Default)",
                [defaultPath] { Clipboard::SetText(defaultPath); }));
            items_draft.push_back(std::make_unique<GridItemButton>(
                kW, kItemH, "Copy API Path",
                [apiPath]     { Clipboard::SetText(apiPath); }));
            items_draft.push_back(std::make_unique<GridItemButton>(
                kW, kItemH, "Copy Stand Link",
                [linkUrl]     { Clipboard::SetText(linkUrl); }));
        }
    }
}

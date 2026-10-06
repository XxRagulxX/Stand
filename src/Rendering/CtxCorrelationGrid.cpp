#include "Rendering/CtxCorrelationGrid.hpp"

#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/Widgets/ToggleCorrelation.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/Theme.hpp"
#include "Menu/Click.hpp"

namespace Stand::Rendering
{
    namespace
    {
        constexpr float kItemH = Theme::kContentItemHeight;
        constexpr int16_t kW   = Theme::kContentWidth;

        static const char* kTypeNames[] = {
            "Off", "Menu Open", "On Foot", "Aiming", "Freeroam", "Chatting", "Session Host"
        };
    }

    CtxCorrelationGrid::CtxCorrelationGrid()
        : Grid(Theme::GetContentOrigin(), 0)
    {
    }

    void CtxCorrelationGrid::SetTarget(Stand::CommandToggle* target)
    {
        m_Target = target;
        invalidate();
    }

    void CtxCorrelationGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
    {
        if (!m_Target) return;
        auto* t = m_Target;

        for (int i = 0; i < (int)ToggleCorrelation::_NUM_TOGGLE_CORRELATIONS; ++i)
        {
            auto type = (ToggleCorrelation::Type)i;
            items_draft.push_back(std::make_unique<GridItemButton>(
                kW, kItemH, kTypeNames[i],
                [t, type] {
                    Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                    t->setCorrelation(click, type, t->correlation.invert);
                },
                [t, type]() -> std::string {
                    return t->correlation.type == type ? "•" : "";
                }));
        }

        items_draft.push_back(std::make_unique<GridItemButton>(
            kW, kItemH, "Invert",
            [t] {
                Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                t->setCorrelation(click, t->correlation.type, !t->correlation.invert);
            },
            [t]() -> std::string { return t->correlation.invert ? "On" : "Off"; }));
    }
}

#include "Rendering/CtxHotkeysGrid.hpp"

#include "Commands/Widgets/CommandPhysical.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/Theme.hpp"
#include "Menu/Hotkey.hpp"

namespace Stand::Rendering
{
    namespace
    {
        constexpr float kItemH = Theme::kContentItemHeight;
        constexpr int16_t kW   = Theme::kContentWidth;

        class CtxHotkeyEntryGrid : public Grid
        {
        public:
            explicit CtxHotkeyEntryGrid() : Grid(Theme::GetContentOrigin(), 0) {}

            void Setup(Stand::CommandPhysical* target, Stand::Hotkey hk)
            {
                m_Target = target;
                m_Hotkey = hk;
                invalidate();
            }

        protected:
            void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override
            {
                if (!m_Target) return;
                auto* t = m_Target;
                Hotkey hk = m_Hotkey;

                if (t->canHotkeyBeRemoved(hk))
                {
                    items_draft.push_back(std::make_unique<GridItemButton>(
                        kW, kItemH, "Remove",
                        [t, hk] { t->removeHotkey(hk); }));
                }

                if (t->isToggle())
                {
                    items_draft.push_back(std::make_unique<GridItemButton>(
                        kW, kItemH, "Hold Mode",
                        [t, hk]() mutable {
                            for (auto& h : t->hotkeys)
                            {
                                if (h == hk)
                                {
                                    h.setHoldMode(!h.isHoldMode());
                                    break;
                                }
                            }
                        },
                        [t, hk]() -> std::string {
                            for (const auto& h : t->hotkeys)
                                if (h == hk)
                                    return h.isHoldMode() ? "On" : "Off";
                            return "Off";
                        }));
                }
            }

        private:
            Stand::CommandPhysical* m_Target = nullptr;
            Stand::Hotkey m_Hotkey{};
        };

        std::vector<CtxHotkeyEntryGrid> g_EntryGrids;
    }

    CtxHotkeysGrid::CtxHotkeysGrid()
        : Grid(Theme::GetContentOrigin(), 0)
    {
    }

    void CtxHotkeysGrid::SetTarget(Stand::CommandPhysical* target)
    {
        m_Target = target;
        invalidate();
    }

    void CtxHotkeysGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
    {
        if (!m_Target) return;

        items_draft.push_back(std::make_unique<GridItemButton>(
            kW, kItemH, "Add Hotkey",
            [] {}));

        g_EntryGrids.resize(m_Target->hotkeys.size());
        for (size_t i = 0; i < m_Target->hotkeys.size(); ++i)
        {
            g_EntryGrids[i].Setup(m_Target, m_Target->hotkeys[i]);
            items_draft.push_back(std::make_unique<GridItemFolder>(
                kW, kItemH, m_Target->hotkeys[i].toString(), &g_EntryGrids[i]));
        }
    }
}

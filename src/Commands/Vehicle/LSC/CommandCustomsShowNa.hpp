#pragma once
#include "Commands/Vehicle/LSC/CommandLscShared.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
    namespace
    {
        class CommandLscShowNonApplicable : public CommandToggle
        {
        public:
            explicit CommandLscShowNonApplicable(CommandList* parent)
                : CommandToggle(parent, LIT("Show Non-Applicable"), CMDNAMES("lscshownonapplicable")) {}

            void onChange(Click& click) override
            {
                s_lsc_show_na.store(m_on, std::memory_order_relaxed);
            }
        };
    }
}

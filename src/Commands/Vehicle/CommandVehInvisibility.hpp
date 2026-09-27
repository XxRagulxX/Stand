#pragma once
#include "Commands/Widgets/CommandSlider.hpp"

#include <string>

namespace Stand
{
    class CommandVehInvisibility : public CommandSlider
    {
        bool m_ticking = false;
    public:
        explicit CommandVehInvisibility(CommandList* parent);
        std::string getValueText() const override;
        void onChange(Click& click, int prev_value) override;
        void onTick() override;
        ~CommandVehInvisibility() override;
    };
}

#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandColourRGB : public CommandList
    {
    public:
        int m_r;
        int m_g;
        int m_b;

        explicit CommandColourRGB(CommandList* parent,
            Label&& menu_name,
            std::vector<CommandName>&& command_names = {},
            Label&& help_text = NOLABEL,
            int default_r = 255,
            int default_g = 255,
            int default_b = 255);

        void getRGB(float& out_r, float& out_g, float& out_b) const
        {
            out_r = m_r / 255.0f;
            out_g = m_g / 255.0f;
            out_b = m_b / 255.0f;
        }
    };
}

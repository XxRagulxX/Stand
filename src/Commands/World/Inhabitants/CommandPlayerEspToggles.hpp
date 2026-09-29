#pragma once
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/AllEntitiesEveryTick.hpp"
#include "Menu/Click.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandPlayerEspBone : public CommandToggle
    {
    public:
        explicit CommandPlayerEspBone(CommandList* parent)
            : CommandToggle(parent, LIT("Bone ESP"), CMDNAMES("playerboneesp", "playbonesp"))
        {}

        void onChange(Click& click) final
        {
            AllEntitiesEveryTick::player_esp_bone = m_on;
        }
    };

    class CommandPlayerEspName : public CommandToggle
    {
    public:
        explicit CommandPlayerEspName(CommandList* parent)
            : CommandToggle(parent, LIT("Name ESP"), CMDNAMES("nameesp"))
        {}

        void onChange(Click& click) final
        {
            AllEntitiesEveryTick::player_esp_name = m_on;
        }
    };

    class CommandPlayerEspBox : public CommandToggle
    {
    public:
        explicit CommandPlayerEspBox(CommandList* parent)
            : CommandToggle(parent, LIT("Box ESP"), CMDNAMES("boxesp"))
        {}

        void onChange(Click& click) final
        {
            AllEntitiesEveryTick::player_esp_box = m_on;
        }
    };

    class CommandPlayerEspLine : public CommandToggle
    {
    public:
        explicit CommandPlayerEspLine(CommandList* parent)
            : CommandToggle(parent, LIT("Line ESP"), CMDNAMES("lineesp"))
        {}

        void onChange(Click& click) final
        {
            AllEntitiesEveryTick::player_esp_line = m_on;
        }
    };
}

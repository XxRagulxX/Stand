#include "Commands/Vehicle/ARSpeed/CommandListArSpeed.hpp"
#include "Commands/Vehicle/ARSpeed/CommandArSpeed.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    namespace
    {
        class CommandArSpeedUseDx : public CommandToggle
        {
        public:
            explicit CommandArSpeedUseDx(CommandList* parent)
                : CommandToggle(parent, LIT("DirectX Rendering"), CMDNAMES("arspeeddx"),
                    LIT("Renders the speedometer using the DirectX 12 renderer instead of GTA's HUD text."))
            {}

            void onChange(Click&) override
            {
                static_cast<CommandListArSpeed*>(parent)->m_use_dx = m_on;
            }
        };
    }

    CommandListArSpeed::CommandListArSpeed(CommandList* parent)
        : CommandList(parent, LIT("AR Speedometer"), CMDNAMES_0())
    {
        auto colour = makeChild<CommandListArSpeedColour>();
        createChild<CommandArSpeed>(colour.get());
        createChild<CommandArSpeedUseDx>();
        children.emplace_back(std::move(colour));
    }
}

#include "Commands/Vehicle/Boost/CommandListRocketBoost.hpp"

#include "Commands/Vehicle/Boost/CommandRocketOverride.hpp"
#include "Commands/Vehicle/Boost/CommandVerticalRocketBoost.hpp"
#include "Commands/Vehicle/Boost/CommandBoostCharge.hpp"
#include "Commands/Vehicle/Boost/CommandAutoRocketBoost.hpp"

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
    CommandListRocketBoost::CommandListRocketBoost(CommandList* parent)
        : CommandList(parent, LIT("Rocket Boost"), CMDNAMES_0())
    {
        createChild<CommandRocketOverride>();
        createChild<CommandVerticalRocketBoost>();
        createChild<CommandBoostCharge>();
        createChild<CommandAutoBoostToggle>();
    }
}

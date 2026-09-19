#include "Commands/Vehicle/CommandTabVehicle.hpp"

namespace Stand::Features
{
    Stand::CommandTabVehicle& GetCommandTabVehicle()
    {
        static Stand::CommandTabVehicle instance{};
        return instance;
    }
}

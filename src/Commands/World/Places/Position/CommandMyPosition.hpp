#pragma once
#include "Commands/World/Places/Position/CommandVector3.hpp"
#include "Commands/World/Places/Position/CommandVector3Slider.hpp"
#include "Commands/World/Places/Position/CommandVector3Copy.hpp"
#include "Commands/World/Places/Position/CommandSavePosMe.hpp"
#include "Commands/World/Places/Position/CommandSavePosWp.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandMyPosition : public CommandVector3
    {
    public:
        explicit CommandMyPosition(CommandList* parent)
            : CommandVector3(parent, LIT("Position"), CMDNAMES("pos", "coords"))
        {
        }

        void populateChildren()
        {
            createChild<CommandSavePosMe>();
            createChild<CommandSavePosWp>();
            createChild<CommandVector3Slider<&Vector3::x>>(LIT("X"), CMDNAMES("posx"));
            createChild<CommandVector3Slider<&Vector3::y>>(LIT("Y"), CMDNAMES("posy"));
            createChild<CommandVector3Slider<&Vector3::z>>(LIT("Z (Altitude)"), CMDNAMES("posz"));
            createChild<CommandVector3Copy>(CMDNAMES("copypos"));
        }

        [[nodiscard]] Vector3 getVec() override
        {
            auto ped = PLAYER::GET_PLAYER_PED(-1);
            if (ped == 0)
                return {};
            return ENTITY::GET_ENTITY_COORDS(ped, TRUE);
        }

        void setVec(Vector3 vec) override
        {
            auto ped = PLAYER::GET_PLAYER_PED(-1);
            auto veh = PED::GET_VEHICLE_PED_IS_IN(ped, FALSE);
            auto ent = veh ? veh : ped;
            ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ent, vec.x, vec.y, vec.z, TRUE, TRUE, TRUE);
        }
    };
}

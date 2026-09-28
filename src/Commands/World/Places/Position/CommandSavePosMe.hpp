#pragma once
#include "Commands/World/Places/Position/CommandSavePos.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

#include <chrono>
#include <iomanip>
#include <sstream>

namespace Stand
{
    class CommandSavePosMe : public CommandSavePos
    {
    public:
        explicit CommandSavePosMe(CommandList* parent)
            : CommandSavePos(parent, LIT("Save My Position"), CMDNAMES("savepos", "savecoords", "saveplace"))
        {
        }

        void onClick(Click& click) override
        {
            click.ensureScriptThread([this](Click& click) {
                auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();
                saveToFile(click, std::to_wstring(ms));
            });
        }

    protected:
        [[nodiscard]] std::string getPos() const override
        {
            auto ped = PLAYER::GET_PLAYER_PED(-1);
            auto coords = ENTITY::GET_ENTITY_COORDS(ped, TRUE);
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(6) << coords.x << ", " << coords.y << ", " << coords.z;
            return oss.str();
        }
    };
}

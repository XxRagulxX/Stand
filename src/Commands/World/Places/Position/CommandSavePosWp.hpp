#pragma once
#include "Commands/World/Places/Position/CommandSavePos.hpp"

namespace Stand
{
    class CommandSavePosWp : public CommandSavePos
    {
    public:
        explicit CommandSavePosWp(CommandList* parent);

        void onClick(Click& click) override;
        void onCommand(Click& click, std::wstring& args) override;

    protected:
        [[nodiscard]] std::string getPos() const override;

    private:
        [[nodiscard]] bool waypointExists() const;
    };
}

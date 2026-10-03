#pragma once
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandArWaypoint : public CommandToggle
    {
    public:
        explicit CommandArWaypoint(CommandList* parent);

        ~CommandArWaypoint() override;

        void onEnable(Click& click) final;
        void onDisable(Click& click) final;
        void onTick() override;

    private:
        bool m_ticking = false;
    };
}

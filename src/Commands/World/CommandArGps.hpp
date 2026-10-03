#pragma once
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandArGps : public CommandToggle
    {
    public:
        explicit CommandArGps(CommandList* parent);

        ~CommandArGps() override;

        void onEnable(Click& click) final;
        void onDisable(Click& click) final;
        void onTick() override;

    private:
        bool m_ticking = false;
    };
}

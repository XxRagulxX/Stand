#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSliderProximity.hpp"
#include "Commands/Stand/CommandToggleNoCorrelation.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandClearArea : public CommandList
    {
    public:
        CommandSliderProximity* distance{};
        CommandToggleNoCorrelation* vehicles{};
        CommandToggleNoCorrelation* peds{};
        CommandToggleNoCorrelation* objects{};
        CommandToggleNoCorrelation* cages{};
        CommandToggleNoCorrelation* pickups{};
        CommandToggleNoCorrelation* ignore_mission_entities{};
        CommandToggleNoCorrelation* no_delay{};

        explicit CommandClearArea(CommandList* parent);
        ~CommandClearArea() override;

        void onBecomesActiveGrid(Rendering::Grid* grid) override;
        void onTick() override;

    private:
        Rendering::Grid* m_myGrid{};
    };
}

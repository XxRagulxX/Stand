#pragma once
#include <ctime>
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/World/Watch_Dogs/DedsecHacks.hpp"
#include "Core/types.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandDedsec : public CommandToggle
    {
    public:
        explicit CommandDedsec(CommandList* parent);
        void onEnable(Click& click) override;
        void onDisable(Click& click) override;
        void onTick() override;

    private:
        Entity m_selected_target = 0;
        Vector3 m_selected_start_rot{};
        int m_tutorial_state = 0;
        time_t m_phone_input_start = 0;
        const DedsecHack* m_last_hack = nullptr;
        time_t m_deselect_delay_start = 0;
        bool m_target_frozen = false;

        Entity findTarget(Ped player_ped) const;
        void drawBoundingBox(Entity ent, int r, int g, int b, int a) const;
        static void hsvToRgb(int h, int s, int v, int& r, int& g, int& b);
        void showTutorial(int state);
    };
}

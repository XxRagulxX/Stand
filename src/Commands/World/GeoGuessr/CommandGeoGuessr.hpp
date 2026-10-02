#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Core/types.hpp"
#include <ctime>

namespace Stand
{
    class CommandToggleNoCorrelation;
    class CommandPhysical;

    class CommandGeoGuessr : public CommandList
    {
    public:
        bool scouting = false;
        time_t guessed_at = 0;
        Vector3 guess{};

        Cam m_cam = 0;
        Vector3 m_target_pos{};
        Vector3 m_target_rot{};

        CommandToggleNoCorrelation* scout_toggle;
        CommandPhysical* submit_action;

        explicit CommandGeoGuessr(CommandList* parent);

        void startScouting();
        void startResultCam();
        void stopScouting();
        void updateChildVisibility(bool show);
        void cleanup();
        void cleanupCam();
        void cleanupGuess();
    };
}

#include "Commands/World/GeoGuessr/CommandGeoGuessr.hpp"

#include "Commands/World/GeoGuessr/CommandGeoGuessrMain.hpp"
#include "Commands/World/GeoGuessr/CommandGeoGuessrScout.hpp"
#include "Commands/World/GeoGuessr/CommandGeoGuessrSubmit.hpp"
#include "Commands/Stand/CommandToggleNoCorrelation.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandGeoGuessr::CommandGeoGuessr(CommandList* parent)
        : CommandList(parent, LIT("GeoGuessr"), {}, LIT("How well do you know San Andreas?"))
    {
        this->createChild<CommandGeoGuessrMain>();
        scout_toggle = this->createChild<CommandGeoGuessrScout>();
        submit_action = this->createChild<CommandGeoGuessrSubmit>();
    }

    void CommandGeoGuessr::startScouting()
    {
        if (m_cam == 0)
        {
            m_cam = CAMERA::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", TRUE);
            CAMERA::SET_CAM_COORD(m_cam, m_target_pos.x, m_target_pos.y, m_target_pos.z);
            CAMERA::SET_CAM_ROT(m_cam, m_target_rot.x, m_target_rot.y, m_target_rot.z, 2);
            CAMERA::SET_CAM_FOV(m_cam, 50.0f);
            CAMERA::SET_CAM_ACTIVE(m_cam, TRUE);
            CAMERA::RENDER_SCRIPT_CAMS(TRUE, FALSE, 0, TRUE, FALSE, 0);
        }

        scouting = true;
        scout_toggle->m_on = true;
    }

    void CommandGeoGuessr::startResultCam()
    {
        m_cam = CAMERA::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", TRUE);
        CAMERA::SET_CAM_ACTIVE(m_cam, TRUE);
        CAMERA::RENDER_SCRIPT_CAMS(TRUE, FALSE, 0, TRUE, FALSE, 0);
    }

    void CommandGeoGuessr::stopScouting()
    {
        if (!scouting)
            return;

        scouting = false;
        scout_toggle->m_on = false;
        cleanupCam();
    }

    void CommandGeoGuessr::updateChildVisibility(bool show)
    {
        if (show)
        {
            scout_toggle->flags &= ~CMDFLAG_CONCEALED;
            submit_action->flags &= ~CMDFLAG_CONCEALED;
        }
        else
        {
            scout_toggle->flags |= CMDFLAG_CONCEALED;
            submit_action->flags |= CMDFLAG_CONCEALED;
        }
    }

    void CommandGeoGuessr::cleanup()
    {
        updateChildVisibility(false);
        stopScouting();

        if (guessed_at != 0)
        {
            guessed_at = 0;
            cleanupGuess();
        }
    }

    void CommandGeoGuessr::cleanupCam()
    {
        if (m_cam != 0)
        {
            CAMERA::RENDER_SCRIPT_CAMS(FALSE, FALSE, 0, FALSE, FALSE, 0);
            CAMERA::DESTROY_CAM(m_cam, FALSE);
            m_cam = 0;
        }
    }

    void CommandGeoGuessr::cleanupGuess()
    {
        cleanupCam();
    }
}

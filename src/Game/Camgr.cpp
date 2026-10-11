#include "Game/Camgr.hpp"

#include <soup/Quaternion.hpp>

#include "Game/AbstractEntity.hpp"
#include "Scripting/Natives.hpp"

namespace Stand
{
	void Camgr::ensure_custom_cam()
	{
		if (custom_cam == 0)
		{
			const Vector3 pos = CAMERA::GET_GAMEPLAY_CAM_COORD();
			const Vector3 rot = CAMERA::GET_GAMEPLAY_CAM_ROT(2);
			custom_cam = CAMERA::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_CAMERA", pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, CAMERA::GET_GAMEPLAY_CAM_FOV(), false, 2);
			CAMERA::SET_CAM_ACTIVE(custom_cam, true);
			CAMERA::SET_CAM_AFFECTS_AIMING(custom_cam, true);
			CAMERA::RENDER_SCRIPT_CAMS(true, false, 0, false, false, false);
		}
	}

	void Camgr::cleanup()
	{
		if (custom_cam != 0)
		{
			CAMERA::RENDER_SCRIPT_CAMS(false, false, 0, false, false, false);
			CAMERA::SET_CAM_ACTIVE(custom_cam, false);
			CAMERA::DESTROY_CAM(custom_cam, false);
			custom_cam = 0;
		}
	}

	bool Camgr::needsCustomCam()
	{
		return !pos_from_gameplay_cam || !rot_from_gameplay_cam || distance_from_character_multiplier != 1.0f;
	}

	bool Camgr::cleanup_if_applicable()
	{
		if (!needsCustomCam())
		{
			cleanup();
			return false;
		}
		return custom_cam != 0;
	}

	void Camgr::set_pos(const v3& pos)
	{
		const v3 cam_pos = pos - g_player_ent.getPos();
		CAMERA::ATTACH_CAM_TO_ENTITY(custom_cam, g_player_ent, cam_pos.x, cam_pos.y, cam_pos.z, false);
	}

	void Camgr::set_rot(const v3& rot)
	{
		CAMERA::SET_CAM_ROT(custom_cam, rot.x, rot.y, rot.z, 2);
	}

	void Camgr::set_rot(const soup::Quaternion& rot)
	{
	}

	void Camgr::set_fov(const float fov)
	{
		CAMERA::SET_CAM_FOV(custom_cam, fov);
	}

	void Camgr::copy_pos_from_gameplay_cam()
	{
		const v3 gameplay_cam_pos = CAMERA::GET_GAMEPLAY_CAM_COORD();
		set_pos(gameplay_cam_pos);
	}

	void Camgr::copy_rot_from_gameplay_cam()
	{
		set_rot(CAMERA::GET_GAMEPLAY_CAM_ROT(2));
	}

	void Camgr::copy_fov_from_gameplay_cam()
	{
		set_fov(CAMERA::GET_GAMEPLAY_CAM_FOV());
	}

	void Camgr::on_tick()
	{
		if (needsCustomCam())
		{
			ensure_custom_cam();
			if (pos_from_gameplay_cam) copy_pos_from_gameplay_cam();
			if (rot_from_gameplay_cam) copy_rot_from_gameplay_cam();
			if (fov_from_gameplay_cam) copy_fov_from_gameplay_cam();
		}
		else
		{
			cleanup();
		}
	}

	void Camgr::lock_pos(const v3& pos)
	{
		pos_from_gameplay_cam = false;
		ensure_custom_cam();
		set_pos(pos);
	}

	void Camgr::unlock_pos()
	{
		pos_from_gameplay_cam = true;
		if (cleanup_if_applicable())
			copy_pos_from_gameplay_cam();
	}

	void Camgr::set_distance_from_character_multiplier(const float multiplier)
	{
		if ((distance_from_character_multiplier = multiplier) == 1.0f)
			cleanup_if_applicable();
		else
			ensure_custom_cam();
	}

	void Camgr::lock_rot(const v3& rot)
	{
		rot_from_gameplay_cam = false;
		ensure_custom_cam();
		set_rot(rot);
	}

	void Camgr::lock_rot(const soup::Quaternion& rot)
	{
		rot_from_gameplay_cam = false;
		ensure_custom_cam();
		set_rot(rot);
	}

	void Camgr::unlock_rot()
	{
		rot_from_gameplay_cam = true;
		if (cleanup_if_applicable())
			copy_rot_from_gameplay_cam();
	}

	void Camgr::lock_fov(const float fov)
	{
		fov_from_gameplay_cam = false;
		set_fov(fov);
	}

	void Camgr::unlock_fov()
	{
		fov_from_gameplay_cam = true;
	}

	void Camgr::lock_focus(const v3& pos)
	{
		focus_pos = pos;
	}

	void Camgr::unlock_focus()
	{
		focus_pos = v3();
		clear_focus_if_applicable();
	}

	bool Camgr::shouldClearFocus()
	{
		return !potato_mode && focus_pos.x == 0.0f && focus_pos.y == 0.0f && focus_pos.z == 0.0f;
	}

	void Camgr::clear_focus_if_applicable()
	{
		if (shouldClearFocus())
			STREAMING::CLEAR_FOCUS();
	}

	void Camgr::stopAnyActiveCamCommand()
	{
		dedup = false;
	}

	void Camgr::startCamCommand()
	{
		stopAnyActiveCamCommand();
		dedup = true;
	}

	bool Camgr::canCamCommandRun()
	{
		return dedup;
	}

	void Camgr::endCamCommand()
	{
		dedup = false;
	}

	void Camgr::disableCinematicCam()
	{
		cinematic_cam_disabled++;
	}

	void Camgr::enableCinematicCam()
	{
		cinematic_cam_disabled--;
	}
}

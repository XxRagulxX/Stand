#include "Game/PositionStateWithCam.hpp"

#include "Game/Camgr.hpp"
#include "Scripting/Natives.hpp"

namespace Stand
{
	PositionStateWithCam::PositionStateWithCam(PositionState&& base, v3&& cam_pos, v3&& cam_rot)
		: PositionState(std::move(base)), cam_pos(std::move(cam_pos)), cam_rot(std::move(cam_rot))
	{
	}

	PositionStateWithCam PositionStateWithCam::save()
	{
		return PositionStateWithCam(
			PositionState::save(),
			CAMERA::GET_FINAL_RENDERED_CAM_COORD(),
			CAMERA::GET_FINAL_RENDERED_CAM_ROT(2)
		);
	}

	void PositionStateWithCam::onRestore() const
	{
		Camgr::lock_pos(cam_pos);
		Camgr::lock_rot(cam_rot);
		HUD::LOCK_MINIMAP_ANGLE((int)cam_rot.heading());
	}
}

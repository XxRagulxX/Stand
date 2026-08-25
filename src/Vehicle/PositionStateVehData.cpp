#include "Vehicle/PositionStateVehData.hpp"

#include "Core/AbstractEntity.hpp"
#include "Vehicle/CVehicleModelInfo.hpp"
#include "Game/gta_vehicle.hpp"
#include "Game/natives.hpp"

namespace Stand
{
	void PositionStateVehData::save(AbstractEntity& veh)
	{
		vel = ENTITY::GET_ENTITY_VELOCITY(veh);
		auto v = g_player_veh.getCVehicle();
		if (reinterpret_cast<CVehicleModelInfo*>(v->archetype)->flags.get(CVehicleModelInfo::FLAG_HAS_GLIDER))
		{
			wings_state = (uint8_t)v->m_gliderState;
			wings_transform = v->m_wingDeployedRatio;
		}
	}

	void PositionStateVehData::restore(AbstractEntity& veh) const
	{
		ENTITY::SET_ENTITY_VELOCITY(veh, vel.x, vel.y, vel.z);
		auto v = veh.getCVehicle();
		if (v->archetype
			&& reinterpret_cast<CVehicleModelInfo*>(v->archetype)->flags.get(CVehicleModelInfo::FLAG_HAS_GLIDER)
			)
		{
			v->m_gliderState = (CVehicle::EGliderState)wings_state;
			v->m_wingDeployedRatio = wings_transform;
		}
	}
}

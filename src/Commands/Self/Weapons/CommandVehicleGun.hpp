#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/get_current_time_millis.hpp"
#include "Util/Label.hpp"
#include "Util/Math.hpp"
#include "Vehicle/VehicleData.hpp"
#include "World/Self.hpp"

#include <array>
#include <cstdlib>
#include <utility>
#include <vector>

namespace Stand
{
	class CommandVehicleGunModel : public CommandSlider
	{
	public:
		explicit CommandVehicleGunModel(CommandList* parent)
			: CommandSlider(parent, LIT("Vehicle"), CMDNAMES("vehiclegunmodel"),
			      NOLABEL, -1, 22, -1)
		{
		}

		[[nodiscard]] std::string getValueText() const override
		{
			if (value == -1)
				return "Random";
			if (value >= 0 && value < (int)g_VehicleClassNames.size())
				return g_VehicleClassNames[value];
			return std::to_string(value);
		}
	};

	class CommandVehicleGunToggle : public CommandToggle
	{
	public:
		CommandVehicleGunModel* model    = nullptr;
		CommandSlider*          speed    = nullptr;
		CommandSlider*          distance = nullptr;

	private:
		bool   m_wasShooting  = false;
		joaat_t m_nextHash     = 0;
		bool   m_bucketsBuilt = false;

		std::vector<std::pair<int, time_t>>      m_vehs;
		std::array<std::vector<joaat_t>, 23>     m_classBuckets;

		void buildBuckets()
		{
			for (auto& b : m_classBuckets)
				b.clear();
			for (size_t i = 0; i < g_VehicleCount; ++i)
			{
				const joaat_t hash = Stand::Joaat(g_VehicleData[i].hash_name);
				const int cls = VEHICLE::GET_VEHICLE_CLASS_FROM_NAME(hash);
				if (cls >= 0 && cls < 23)
					m_classBuckets[cls].push_back(hash);
			}
			m_bucketsBuilt = true;
		}

		[[nodiscard]] joaat_t pickHash() const
		{
			const int cls = model ? model->value : -1;
			if (cls == -1 || cls < 0 || cls >= 23 || m_classBuckets[cls].empty())
				return Stand::Joaat(g_VehicleData[std::rand() % g_VehicleCount].hash_name);
			return m_classBuckets[cls][std::rand() % m_classBuckets[cls].size()];
		}

		void spawnVehicle(joaat_t hash, int pedHandle)
		{
			if (!hash || !STREAMING::HAS_MODEL_LOADED(hash))
				return;

			const Vector3 camRot = CAMERA::GET_FINAL_RENDERED_CAM_ROT(2);
			const Vector3 camPos = CAMERA::GET_FINAL_RENDERED_CAM_COORD();
			const auto    camDir = Math::RotationToDirection({camRot.x, camRot.y, camRot.z});
			const float   dist   = distance ? (float)distance->value : 10.0f;

			const float px = camPos.x + camDir.x * dist;
			const float py = camPos.y + camDir.y * dist;
			const float pz = camPos.z + camDir.z * dist;

			const int veh = VEHICLE::CREATE_VEHICLE(hash, px, py, pz, camRot.z, TRUE, FALSE, FALSE);
			if (!veh)
				return;

			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(veh, px, py, pz, false, false, false);
			ENTITY::SET_ENTITY_ROTATION(veh, camRot.x, camRot.y, camRot.z, 0, true);
			ENTITY::SET_ENTITY_NO_COLLISION_ENTITY(veh, pedHandle, TRUE);
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, speed ? (float)speed->value : 200.0f);

			m_vehs.emplace_back(veh, get_current_time_millis());
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
		}

	public:
		explicit CommandVehicleGunToggle(CommandList* parent)
			: CommandToggle(parent, LIT("Vehicle Gun"), CMDNAMES("vehiclegun"))
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			m_wasShooting = false;
			m_nextHash    = 0;
			for (auto& [handle, _] : m_vehs)
			{
				if (ENTITY::DOES_ENTITY_EXIST(handle))
				{
					int h = handle;
					ENTITY::SET_ENTITY_AS_MISSION_ENTITY(h, FALSE, TRUE);
					ENTITY::DELETE_ENTITY(&h);
				}
			}
			m_vehs.clear();
		}

		~CommandVehicleGunToggle() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}

		void onTick() override
		{
			if (!m_bucketsBuilt)
				buildBuckets();

			if (!m_nextHash)
				m_nextHash = pickHash();
			if (m_nextHash && !STREAMING::HAS_MODEL_LOADED(m_nextHash))
				STREAMING::REQUEST_MODEL(m_nextHash);

			auto ped = Self::GetPed();
			if (!ped)
			{
				m_wasShooting = false;
				return;
			}
			const int pedHandle = ped.GetHandle();

			const bool shooting = PED::IS_PED_SHOOTING(pedHandle) == TRUE;
			const bool rising   = shooting && !m_wasShooting;
			m_wasShooting = shooting;

			if (rising)
			{
				spawnVehicle(m_nextHash, pedHandle);
				m_nextHash = 0;
			}

			const time_t now = get_current_time_millis();
			for (auto it = m_vehs.begin(); it != m_vehs.end(); )
			{
				int h = it->first;
				const bool exists = ENTITY::DOES_ENTITY_EXIST(h) == TRUE;
				if (!exists || m_vehs.size() > 40 || (now - it->second) > 120000)
				{
					if (exists)
					{
						ENTITY::SET_ENTITY_AS_MISSION_ENTITY(h, FALSE, TRUE);
						ENTITY::DELETE_ENTITY(&h);
					}
					it = m_vehs.erase(it);
				}
				else
				{
					if ((now - it->second) < 2000)
						ENTITY::SET_ENTITY_NO_COLLISION_ENTITY(h, pedHandle, TRUE);
					++it;
				}
			}
		}
	};

	class CommandVehicleGun : public CommandList
	{
	public:
		explicit CommandVehicleGun(CommandList* parent)
			: CommandList(parent, LIT("Vehicle Gun"), CMDNAMES("vehiclegunlist"))
		{
			auto* toggle     = createChild<CommandVehicleGunToggle>();
			toggle->model    = createChild<CommandVehicleGunModel>();
			toggle->speed    = createChild<CommandSlider>(
				LIT("Speed"), CMDNAMES("vehiclegunspeed"),
				NOLABEL, 20, 200, 200);
			toggle->distance = createChild<CommandSlider>(
				LIT("Distance"), CMDNAMES("vehiclegundistance"),
				NOLABEL, 10, 30, 10);
		}
	};
}

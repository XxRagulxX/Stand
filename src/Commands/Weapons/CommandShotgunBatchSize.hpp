#pragma once
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Joaat.hpp"
#include "Util/Label.hpp"
#include "Util/Math.hpp"
#include "World/Self.hpp"

#include <cmath>
#include <cstdlib>

namespace Stand
{
	class CommandShotgunBatchSize : public CommandSlider
	{
		bool m_wasShooting = false;

		static constexpr Hash kShotgunHashes[] = {
			"WEAPON_PUMPSHOTGUN"_J, "WEAPON_SAWNOFFSHOTGUN"_J,
			"WEAPON_ASSAULTSHOTGUN"_J, "WEAPON_BULLPUPSHOTGUN"_J,
			"WEAPON_PUMPSHOTGUN_MK2"_J, "WEAPON_HEAVYSHOTGUN"_J,
			"WEAPON_DBSHOTGUN"_J, "WEAPON_AUTOSHOTGUN"_J,
			"WEAPON_COMBATSHOTGUN"_J,
		};

		static bool isShotgun(Hash h)
		{
			for (Hash s : kShotgunHashes)
				if (s == h) return true;
			return false;
		}

		static float randF(float range)
		{
			return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f) * 2.0f * range;
		}

	public:
		explicit CommandShotgunBatchSize(CommandList* const parent)
			: CommandSlider(parent, LIT("Bullets In Shotgun Batch"),
			                CMDNAMES("shotgunbatchsizemultiplier"),
			                NOLABEL, 0, 16, 0, 1)
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onTick() override
		{
			if (value == 0)
			{
				m_wasShooting = false;
				return;
			}

			auto ped = Stand::Self::GetPed();
			if (!ped || ped.IsDead())
			{
				m_wasShooting = false;
				return;
			}

			const int pedHandle = ped.GetHandle();

			Hash currentWeapon{};
			WEAPON::GET_CURRENT_PED_WEAPON(pedHandle, &currentWeapon, false);
			if (!isShotgun(currentWeapon))
			{
				m_wasShooting = false;
				return;
			}

			const bool isShooting = PED::IS_PED_SHOOTING(pedHandle) == TRUE;
			const bool risingEdge = isShooting && !m_wasShooting;
			m_wasShooting = isShooting;

			if (!risingEdge)
				return;

			auto camDir = Math::RotationToDirection(CAMERA::GET_GAMEPLAY_CAM_ROT(0));
			auto camPos = CAMERA::GET_GAMEPLAY_CAM_COORD() + camDir;
			Vector3 end = camPos + camDir * 2000.0f;

			const auto rayHandle = SHAPETEST::START_EXPENSIVE_SYNCHRONOUS_SHAPE_TEST_LOS_PROBE(
				camPos.x, camPos.y, camPos.z,
				end.x, end.y, end.z,
				-1, 0, 7);

			BOOL    didHit{FALSE};
			Vector3 hitPos{};
			Vector3 norm{};
			int     hitEnt{};
			SHAPETEST::GET_SHAPE_TEST_RESULT(rayHandle, &didHit, &hitPos, &norm, &hitEnt);
			if (didHit)
				end = hitPos;

			const Vector3 worldUp{0.f, 0.f, 1.f};
			Vector3 right{
				camDir.y * worldUp.z - camDir.z * worldUp.y,
				camDir.z * worldUp.x - camDir.x * worldUp.z,
				camDir.x * worldUp.y - camDir.y * worldUp.x,
			};
			const float rl = std::sqrtf(right.x * right.x + right.y * right.y + right.z * right.z);
			if (rl > 0.001f) { right.x /= rl; right.y /= rl; right.z /= rl; }
			const Vector3 up{
				right.y * camDir.z - right.z * camDir.y,
				right.z * camDir.x - right.x * camDir.z,
				right.x * camDir.y - right.y * camDir.x,
			};

			constexpr float kSpread = 0.4f;
			const int dmg = WEAPON::GET_WEAPON_DAMAGE(currentWeapon, 0);

			for (int i = 0; i < value; ++i)
			{
				const float ox = randF(kSpread);
				const float oy = randF(kSpread);
				const Vector3 spreadEnd{
					end.x + right.x * ox + up.x * oy,
					end.y + right.y * ox + up.y * oy,
					end.z + right.z * ox + up.z * oy,
				};
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(
					camPos.x, camPos.y, camPos.z,
					spreadEnd.x, spreadEnd.y, spreadEnd.z,
					dmg, true, currentWeapon, pedHandle,
					true, false, -1.0f);
			}
		}

		~CommandShotgunBatchSize() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}

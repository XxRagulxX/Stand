#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Core/Pointers.hpp"
#include "Game/Pools.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

#include <atomic>
#include <chrono>
#include <cmath>
#include <set>

namespace Stand
{
	class CommandAimbotToggle : public CommandToggle
	{
		static constexpr int kBones[] = {
			31086,
			39317,
			40269,
			61007,
			57005,
			26610,
			11816,
			36864,
			63931,
			14201,
			52301,
		};

	public:
		CommandToggle* players{};
		CommandToggle* peds{};
		CommandToggle* vehicles{};
		CommandSlider* range{};
		CommandToggle* excludeFriends{};
		CommandToggle* excludeCrewMembers{};
		CommandToggle* excludeStandUsers{};
		CommandToggle* excludeOrgMembers{};
		CommandToggle* box{};
		CommandToggle* losCheck{};
		CommandSlider* smoothing{};
		CommandSlider* lockout{};
		CommandSlider* bone{};

		std::atomic<int>  m_currentTarget{0};
		std::atomic<bool> m_targetIsPed{false};

	private:
		std::set<int> m_killed;
		int64_t       m_lockoutUntil{};
		float         m_smoothH{};
		float         m_smoothP{};

		bool HasLos(const Vector3& from, const Vector3& to, int ignoreHandle)
		{
			int probe = SHAPETEST::START_SHAPE_TEST_LOS_PROBE(
				from.x, from.y, from.z,
				to.x, to.y, to.z,
				-1, ignoreHandle, 2);
			BOOL    hit    = FALSE;
			Vector3 end    = {};
			Vector3 norm   = {};
			int     hitEnt = 0;
			SHAPETEST::GET_SHAPE_TEST_RESULT(probe, &hit, &end, &norm, &hitEnt);
			return !hit;
		}

		bool IsPlayerExcluded(int pedHandle)
		{
			if (!PED::IS_PED_A_PLAYER(pedHandle))
				return false;

			const int playerId = NETWORK::NETWORK_GET_PLAYER_INDEX_FROM_PED(pedHandle);
			if (playerId < 0 || playerId > 31)
				return false;

			if (excludeFriends && excludeFriends->m_on)
			{
				if (NETWORK::NETWORK_IS_FRIEND_IN_SAME_TITLE(PLAYER::GET_PLAYER_NAME(playerId)))
					return true;
			}

			if (excludeOrgMembers && excludeOrgMembers->m_on)
			{
				const int localId   = Stand::Self::GetPlayer().GetId();
				const int theirTeam = PLAYER::GET_PLAYER_TEAM(playerId);
				const int localTeam = PLAYER::GET_PLAYER_TEAM(localId);
				if (theirTeam != -1 && theirTeam == localTeam)
					return true;
			}

			return false;
		}

		void clearTarget()
		{
			m_currentTarget.store(0, std::memory_order_relaxed);
		}

	public:
		explicit CommandAimbotToggle(CommandList* parent)
			: CommandToggle(parent, LIT("Aimbot"), CMDNAMES("aimbot"),
			                LIT("Locks onto nearby targets and aims at them automatically."))
		{
		}

		void onEnable(Click& click) override
		{
			Pointers.ShouldNotTargetEntityPatch->Apply();
			Pointers.GetAssistedAimTypePatch->Apply();
			CommandTickDispatch::AddCommand(this);
			m_killed.clear();
			m_lockoutUntil = 0;
			m_smoothH = 0.0f;
			m_smoothP = 0.0f;
		}

		void onDisable(Click& click) override
		{
			Pointers.ShouldNotTargetEntityPatch->Restore();
			Pointers.GetAssistedAimTypePatch->Restore();
			CommandTickDispatch::RemoveCommand(this);
			m_killed.clear();
			clearTarget();
		}

		void onTick() override
		{
			auto selfPed = Stand::Self::GetPed();
			if (!selfPed)
				return;

			const int selfHandle = selfPed.GetHandle();
			const int localId    = Stand::Self::GetPlayer().GetId();

			using Clock = std::chrono::steady_clock;
			using Ms    = std::chrono::milliseconds;
			const int64_t nowMs = std::chrono::duration_cast<Ms>(
				Clock::now().time_since_epoch()).count();

			for (Ped ped : Pools::GetPeds())
			{
				const int h = ped.GetHandle();
				if (h == selfHandle || m_killed.contains(h))
					continue;
				if (ped.IsDead()
				    && ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ENTITY(h, selfHandle, false))
				{
					m_killed.insert(h);
					m_lockoutUntil = nowMs + (lockout ? lockout->value : 0);
					PAD::DISABLE_CONTROL_ACTION(0, 25, true);
				}
			}

			if (nowMs < m_lockoutUntil)
			{
				clearTarget();
				return;
			}

			if (!PLAYER::IS_PLAYER_FREE_AIMING(localId))
			{
				m_smoothH = 0.0f;
				m_smoothP = 0.0f;
				clearTarget();
				return;
			}

			const bool wantPlayers  = !players  || players->m_on;
			const bool wantPeds     = peds     && peds->m_on;
			const bool wantVehicles = vehicles && vehicles->m_on;
			const float maxDist     = range ? static_cast<float>(range->value) : 30000.0f;
			const bool  doLos       = losCheck && losCheck->m_on;

			const Vector3 camPos = CAMERA::GET_GAMEPLAY_CAM_COORD();

			int     bestHandle = 0;
			float   bestDistSq = maxDist * maxDist + 1.0f;
			Vector3 bestPos    = {};
			bool    bestIsPed  = false;

			const int boneIdx = bone ? std::clamp(bone->value, 0, 10) : 0;
			const int boneId  = kBones[boneIdx];

			for (Ped ped : Pools::GetPeds())
			{
				const int h = ped.GetHandle();
				if (h == selfHandle || ENTITY::IS_ENTITY_DEAD(h, false))
					continue;

				const bool isPlayer = ped.IsPlayer();
				if (isPlayer && !wantPlayers)   continue;
				if (!isPlayer && !wantPeds)     continue;
				if (isPlayer && IsPlayerExcluded(h)) continue;

				const Vector3 bonePos = PED::GET_PED_BONE_COORDS(h, boneId, 0.f, 0.f, 0.f);
				const float dx = bonePos.x - camPos.x;
				const float dy = bonePos.y - camPos.y;
				const float dz = bonePos.z - camPos.z;
				const float dSq = dx*dx + dy*dy + dz*dz;

				if (dSq >= bestDistSq) continue;
				if (doLos && !HasLos(camPos, bonePos, selfHandle)) continue;

				bestDistSq = dSq;
				bestHandle = h;
				bestPos    = bonePos;
				bestIsPed  = true;
			}

			if (wantVehicles)
			{
				for (Vehicle veh : Pools::GetVehicles())
				{
					const int h = veh.GetHandle();
					if (VEHICLE::IS_VEHICLE_SEAT_FREE(h, -1, true))
						continue;

					const Vector3 pos = ENTITY::GET_ENTITY_COORDS(h, true);
					const float dx = pos.x - camPos.x;
					const float dy = pos.y - camPos.y;
					const float dz = pos.z - camPos.z;
					const float dSq = dx*dx + dy*dy + dz*dz;

					if (dSq >= bestDistSq) continue;
					if (doLos && !HasLos(camPos, pos, selfHandle)) continue;

					bestDistSq = dSq;
					bestHandle = h;
					bestPos    = pos;
					bestIsPed  = false;
				}
			}

			if (bestHandle == 0)
			{
				clearTarget();
				return;
			}

			const float dx   = bestPos.x - camPos.x;
			const float dy   = bestPos.y - camPos.y;
			const float dz   = bestPos.z - camPos.z;
			const float dist = std::sqrt(dx*dx + dy*dy + dz*dz);

			if (dist < 0.01f)
			{
				clearTarget();
				return;
			}

			const float worldHeading = std::atan2f(-dx, dy) * (180.0f / 3.14159265f);
			const float worldPitch   = std::asinf(std::clamp(dz / dist, -1.0f, 1.0f))
			                           * (-180.0f / 3.14159265f);

			const float playerHeading = ENTITY::GET_ENTITY_HEADING(selfHandle);
			float relH = worldHeading - playerHeading;
			while (relH >  180.0f) relH -= 360.0f;
			while (relH < -180.0f) relH += 360.0f;

			const float raw = smoothing ? static_cast<float>(smoothing->value) : 0.0f;
			const float t   = std::clamp(1.0f - raw / 1000.0f, 0.02f, 1.0f);

			m_smoothH += (relH       - m_smoothH) * t;
			m_smoothP += (worldPitch - m_smoothP) * t;

			CAMERA::SET_GAMEPLAY_CAM_RELATIVE_HEADING(m_smoothH);
			CAMERA::SET_GAMEPLAY_CAM_RELATIVE_PITCH(m_smoothP, 1.0f);

			m_targetIsPed.store(bestIsPed, std::memory_order_relaxed);
			m_currentTarget.store(bestHandle, std::memory_order_release);
		}

		~CommandAimbotToggle() override
		{
			if (m_on)
			{
				Pointers.ShouldNotTargetEntityPatch->Restore();
				Pointers.GetAssistedAimTypePatch->Restore();
			}
			CommandTickDispatch::RemoveCommand(this);
		}
	};

	class CommandAimbot : public CommandList
	{
	public:
		CommandAimbotToggle* const toggle;
		CommandList*         const targetSelection;
		CommandToggle*       const players;
		CommandToggle*       const peds;
		CommandToggle*       const vehicles;
		CommandSlider*       const range;
		CommandToggle*       const excludeFriends;
		CommandToggle*       const excludeCrewMembers;
		CommandToggle*       const excludeStandUsers;
		CommandToggle*       const excludeOrgMembers;
		CommandToggle*       const box;
		CommandSlider*       const lockout;
		CommandSlider*       const smoothing;
		CommandToggle*       const losCheck;
		CommandToggle*       const magicBullets;
		CommandSlider*       const bone;

		explicit CommandAimbot(CommandList* parent)
			: CommandList(parent, LIT("Aimbot"), CMDNAMES("aimbotlist")),
			  toggle(createChild<CommandAimbotToggle>()),
			  targetSelection(createChild<CommandList>(LIT("Target Selection"), CMDNAMES("aimbottargets"))),
			  players(targetSelection->createChild<CommandToggle>(
			      LIT("Players"), CMDNAMES("aimbotplayers"), NOLABEL, true)),
			  peds(targetSelection->createChild<CommandToggle>(
			      LIT("Pedestrians"), CMDNAMES("aimbotpeds"), NOLABEL, false)),
			  vehicles(targetSelection->createChild<CommandToggle>(
			      LIT("Vehicles"), CMDNAMES("aimbotvehicles"), NOLABEL, false)),
			  range(targetSelection->createChild<CommandSlider>(
			      LIT("Distance"), CMDNAMES("aimbotrange"),
			      LIT("Maximum lock-on distance."),
			      0, 30000, 30000, 100)),
			  excludeFriends(targetSelection->createChild<CommandToggle>(
			      LIT("Exclude Friends"), CMDNAMES("aimbotexcludefriends"), NOLABEL)),
			  excludeCrewMembers(targetSelection->createChild<CommandToggle>(
			      LIT("Exclude Crew Members"), CMDNAMES("aimbotexcludecrewmembers"), NOLABEL)),
			  excludeStandUsers(targetSelection->createChild<CommandToggle>(
			      LIT("Exclude Stand Users"), CMDNAMES("aimbotexcludestandusers"), NOLABEL)),
			  excludeOrgMembers(targetSelection->createChild<CommandToggle>(
			      LIT("Exclude Organisation Members"), CMDNAMES("aimbotexcludeorgmembers"), NOLABEL)),
			  box(createChild<CommandToggle>(
			      LIT("Show Box"), CMDNAMES("aimbotbox"), NOLABEL)),
			  lockout(createChild<CommandSlider>(
			      LIT("Re-engage Time (ms)"), CMDNAMES("aimbotlockout"),
			      LIT("How long the aimbot will stay disabled for after you've moved the camera."),
			      0, 10000, 0, 100)),
			  smoothing(createChild<CommandSlider>(
			      LIT("Smoothing"), CMDNAMES("aimbotsmoothing"), NOLABEL,
			      0, 1000, 0, 10)),
			  losCheck(createChild<CommandToggle>(
			      LIT("Line-of-Sight Check"), CMDNAMES("aimbotloscheck"), NOLABEL)),
			  magicBullets(createChild<CommandToggle>(
			      LIT("Magic Bullets"), CMDNAMES("aimbotmagicbullets"), NOLABEL)),
			  bone(createChild<CommandSlider>(
			      LIT("Target Bone"), CMDNAMES("aimbotbone"),
			      LIT("0=Head 1=Neck 2=R.Elbow 3=L.Elbow 4=R.Hand 5=L.Hand 6=Pelvis 7=R.Calf 8=L.Calf 9=R.Foot 10=L.Foot"),
			      0, 10, 0, 1))
		{
			toggle->players            = players;
			toggle->peds               = peds;
			toggle->vehicles           = vehicles;
			toggle->range              = range;
			toggle->excludeFriends     = excludeFriends;
			toggle->excludeCrewMembers = excludeCrewMembers;
			toggle->excludeStandUsers  = excludeStandUsers;
			toggle->excludeOrgMembers  = excludeOrgMembers;
			toggle->box                = box;
			toggle->losCheck           = losCheck;
			toggle->smoothing          = smoothing;
			toggle->lockout            = lockout;
			toggle->bone               = bone;
		}
	};
}

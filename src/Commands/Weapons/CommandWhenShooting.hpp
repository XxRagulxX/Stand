#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/Entity.hpp"
#include "Menu/Click.hpp"
#include "Rendering/Notifications.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "Vehicle/Vehicle.hpp"
#include "World/Self.hpp"

#include <cmath>
#include <format>

namespace Stand
{
	class WhenShootingBase : public CommandToggle
	{
		bool m_wasShooting = false;

	protected:
		virtual void onShot(Vector3 pos, int entHandle) = 0;

		virtual void onDisableExtra() {}

	public:
		using CommandToggle::CommandToggle;

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			m_wasShooting = false;
			onDisableExtra();
		}

		~WhenShootingBase() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}

		void onTick() override
		{
			auto ped = Self::GetPed();
			if (!ped)
			{
				m_wasShooting = false;
				return;
			}
			const int pedHandle = ped.GetHandle();
			const bool isShooting = PED::IS_PED_SHOOTING(pedHandle) == TRUE;
			const bool risingEdge = isShooting && !m_wasShooting;
			m_wasShooting = isShooting;
			if (!risingEdge)
				return;

			Vector3 pos{};
			WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(pedHandle, &pos);
			int entHandle = 0;
			PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(Self::GetPlayer().GetId(), &entHandle);
			onShot(pos, entHandle);
		}
	};

	class CommandWhenShootAnonExplosion : public WhenShootingBase
	{
	public:
		explicit CommandWhenShootAnonExplosion(CommandList* parent)
			: WhenShootingBase(parent, LIT("Anonymous Explosion"), CMDNAMES("whenshootexplodeanon"))
		{
		}

		void onShot(Vector3 pos, int) override
		{
			FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 23, 1.0f, TRUE, FALSE, 0.0f, FALSE);
		}
	};

	class CommandWhenShootOwnedExplosion : public WhenShootingBase
	{
	public:
		explicit CommandWhenShootOwnedExplosion(CommandList* parent)
			: WhenShootingBase(parent, LIT("Owned Explosion"), CMDNAMES("whenshootexplodeowned"))
		{
		}

		void onShot(Vector3 pos, int) override
		{
			const int ped = Self::GetPed().GetHandle();
			FIRE::ADD_OWNED_EXPLOSION(ped, pos.x, pos.y, pos.z, 23, 1.0f, TRUE, FALSE, 0.0f);
		}
	};

	class CommandWhenShootBurn : public WhenShootingBase
	{
	public:
		explicit CommandWhenShootBurn(CommandList* parent)
			: WhenShootingBase(parent, LIT("Burn"), CMDNAMES("whenshootburn"))
		{
		}

		void onShot(Vector3 pos, int) override
		{
			FIRE::START_SCRIPT_FIRE(pos.x, pos.y, pos.z, 25, FALSE);
		}
	};

	class CommandWhenShootDie : public WhenShootingBase
	{
	public:
		explicit CommandWhenShootDie(CommandList* parent)
			: WhenShootingBase(parent, LIT("Die"), CMDNAMES("whenshootdie"))
		{
		}

		void onShot(Vector3, int entHandle) override
		{
			if (!entHandle)
				return;
			Entity ent{ entHandle };
			if (!ent.IsDead())
				ent.Kill();
		}
	};

	class CommandWhenShootDisarm : public WhenShootingBase
	{
	public:
		explicit CommandWhenShootDisarm(CommandList* parent)
			: WhenShootingBase(parent, LIT("Disarm"), CMDNAMES("whenshootdisarm"),
			      LIT("Will leave them with nothing but their fists."))
		{
		}

		void onShot(Vector3, int entHandle) override
		{
			if (!entHandle || !ENTITY::IS_ENTITY_A_PED(entHandle))
				return;
			WEAPON::REMOVE_ALL_PED_WEAPONS(entHandle, TRUE);
		}
	};

	class CommandWhenShootFreeze : public WhenShootingBase
	{
	public:
		explicit CommandWhenShootFreeze(CommandList* parent)
			: WhenShootingBase(parent, LIT("Freeze"), CMDNAMES("whenshootfreeze"))
		{
		}

		void onShot(Vector3, int entHandle) override
		{
			if (!entHandle)
				return;
			ENTITY::FREEZE_ENTITY_POSITION(entHandle, TRUE);
		}
	};

	class CommandWhenShootCower : public WhenShootingBase
	{
	public:
		explicit CommandWhenShootCower(CommandList* parent)
			: WhenShootingBase(parent, LIT("Cower"), CMDNAMES("whenshootcower"))
		{
		}

		void onShot(Vector3, int entHandle) override
		{
			if (!entHandle || !ENTITY::IS_ENTITY_A_PED(entHandle))
				return;
			TASK::CLEAR_PED_TASKS_IMMEDIATELY(entHandle);
			TASK::TASK_COWER(entHandle, -1);
		}
	};

	class CommandWhenShootFlee : public WhenShootingBase
	{
	public:
		explicit CommandWhenShootFlee(CommandList* parent)
			: WhenShootingBase(parent, LIT("Flee"), CMDNAMES("whenshootflee"))
		{
		}

		void onShot(Vector3, int entHandle) override
		{
			if (!entHandle || !ENTITY::IS_ENTITY_A_PED(entHandle))
				return;
			const int pedHandle = Self::GetPed().GetHandle();
			TASK::TASK_SMART_FLEE_PED(entHandle, pedHandle, 200.0f, -1, FALSE, FALSE);
		}
	};

	class CommandWhenShootPush : public WhenShootingBase
	{
	public:
		explicit CommandWhenShootPush(CommandList* parent)
			: WhenShootingBase(parent, LIT("Push Away"), CMDNAMES("whenshootpush"))
		{
		}

		void onShot(Vector3, int entHandle) override
		{
			if (!entHandle)
				return;
			Entity ent{ entHandle };
			const auto playerPos = Self::GetPed().GetPosition();
			const auto entPos    = ent.GetPosition();
			const float dx = entPos.x - playerPos.x;
			const float dy = entPos.y - playerPos.y;
			const float dz = entPos.z - playerPos.z;
			const float len = std::sqrt(dx * dx + dy * dy + dz * dz);
			if (len < 0.001f)
				return;
			const float scale = 50.0f / len;
			ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(
				entHandle, 1, dx * scale, dy * scale, dz * scale, TRUE, FALSE, TRUE, TRUE);
		}
	};

	class CommandWhenShootDrive : public WhenShootingBase
	{
	public:
		explicit CommandWhenShootDrive(CommandList* parent)
			: WhenShootingBase(parent, LIT("Drive"), CMDNAMES("drivegun"))
		{
		}

		void onShot(Vector3, int entHandle) override
		{
			if (!entHandle)
				return;
			int vehHandle = 0;
			if (ENTITY::IS_ENTITY_A_PED(entHandle))
			{
				vehHandle = PED::IS_PED_IN_ANY_VEHICLE(entHandle, FALSE)
				              ? PED::GET_VEHICLE_PED_IS_IN(entHandle, FALSE)
				              : 0;
			}
			else if (ENTITY::IS_ENTITY_A_VEHICLE(entHandle))
			{
				vehHandle = entHandle;
			}
			if (!vehHandle)
				return;
			const int driver = VEHICLE::GET_PED_IN_VEHICLE_SEAT(vehHandle, -1, FALSE);
			if (driver)
				TASK::TASK_VEHICLE_DRIVE_WANDER(driver, vehHandle, 40.0f, 786603);
		}
	};

	class CommandWhenShootRevive : public WhenShootingBase
	{
	public:
		explicit CommandWhenShootRevive(CommandList* parent)
			: WhenShootingBase(parent, LIT("Revive"), CMDNAMES("revivegun"))
		{
		}

		void onShot(Vector3, int entHandle) override
		{
			if (!entHandle || !ENTITY::IS_ENTITY_A_PED(entHandle))
				return;
			PED::REVIVE_INJURED_PED(entHandle);
		}
	};

	class CommandWhenShootRagdoll : public WhenShootingBase
	{
	public:
		explicit CommandWhenShootRagdoll(CommandList* parent)
			: WhenShootingBase(parent, LIT("Ragdoll"), CMDNAMES("whenshootragdoll"))
		{
		}

		void onShot(Vector3, int entHandle) override
		{
			if (!entHandle || !ENTITY::IS_ENTITY_A_PED(entHandle))
				return;
			PED::SET_PED_TO_RAGDOLL(entHandle, 3000, 3000, 0, FALSE, FALSE, FALSE);
		}
	};

	class CommandWhenShootRepair : public WhenShootingBase
	{
	public:
		explicit CommandWhenShootRepair(CommandList* parent)
			: WhenShootingBase(parent, LIT("Repair"), CMDNAMES("whenshootrepair"))
		{
		}

		void onShot(Vector3, int entHandle) override
		{
			if (!entHandle)
				return;
			int vehHandle = 0;
			if (ENTITY::IS_ENTITY_A_PED(entHandle))
			{
				vehHandle = PED::IS_PED_IN_ANY_VEHICLE(entHandle, FALSE)
				              ? PED::GET_VEHICLE_PED_IS_IN(entHandle, FALSE)
				              : 0;
			}
			else if (ENTITY::IS_ENTITY_A_VEHICLE(entHandle))
			{
				vehHandle = entHandle;
			}
			if (!vehHandle)
				return;
			Stand::Vehicle veh{ vehHandle };
			veh.Fix();
		}
	};

	class CommandWhenShootWeaken : public WhenShootingBase
	{
	public:
		explicit CommandWhenShootWeaken(CommandList* parent)
			: WhenShootingBase(parent, LIT("Weaken"), CMDNAMES("whenshootweaken"),
			      LIT("Reduces their health so they'll die easily."))
		{
		}

		void onShot(Vector3, int entHandle) override
		{
			if (!entHandle || !ENTITY::IS_ENTITY_A_PED(entHandle))
				return;
			Entity ent{ entHandle };
			if (!ent.IsDead() && ent.GetHealth() > 101)
				ent.SetHealth(101);
		}
	};

	class CommandWhenShootDelete : public WhenShootingBase
	{
	public:
		explicit CommandWhenShootDelete(CommandList* parent)
			: WhenShootingBase(parent, LIT("Delete"), CMDNAMES("deletegun"))
		{
		}

		void onShot(Vector3, int entHandle) override
		{
			if (!entHandle)
				return;
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(entHandle, FALSE, TRUE);
			ENTITY::DELETE_ENTITY(&entHandle);
		}
	};

	class CommandWhenShootARMarker : public WhenShootingBase
	{
		int  m_trackedEnt = 0;
		bool m_hasTarget  = false;

	public:
		explicit CommandWhenShootARMarker(CommandList* parent)
			: WhenShootingBase(parent, LIT("AR Marker"), CMDNAMES("whenshootarmarker"))
		{
		}

	protected:
		void onDisableExtra() override
		{
			m_trackedEnt = 0;
			m_hasTarget  = false;
		}

		void onShot(Vector3, int entHandle) override
		{
			m_trackedEnt = entHandle;
			m_hasTarget  = (entHandle != 0);
		}

	public:
		void onTick() override
		{
			WhenShootingBase::onTick();
			if (!m_hasTarget || !m_trackedEnt || !ENTITY::DOES_ENTITY_EXIST(m_trackedEnt))
			{
				m_hasTarget  = false;
				m_trackedEnt = 0;
				return;
			}
			const Vector3 entPos = ENTITY::GET_ENTITY_COORDS(m_trackedEnt, TRUE);
			Vector3 p{ entPos.x, entPos.y, entPos.z + 2.0f };
			GRAPHICS::DRAW_MARKER(0, p.x, p.y, p.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.6f, 0.6f, 0.6f, 255, 0, 0, 200, FALSE, TRUE, 2, FALSE, nullptr, nullptr, FALSE);
		}
	};

	class CommandWhenShootShowModelName : public WhenShootingBase
	{
	public:
		explicit CommandWhenShootShowModelName(CommandList* parent)
			: WhenShootingBase(parent, LIT("Show Model Name"), CMDNAMES("whenshootshowmodelname"))
		{
		}

		void onShot(Vector3, int entHandle) override
		{
			if (!entHandle)
				return;
			const Hash modelHash = ENTITY::GET_ENTITY_MODEL(entHandle);
			Notifications::Show("When Shooting", std::format("0x{:08X}", modelHash));
		}
	};

	class CommandWhenShooting : public CommandList
	{
	public:
		explicit CommandWhenShooting(CommandList* parent)
			: CommandList(parent, LIT("When Shooting"), CMDNAMES("whenshooting"))
		{
			createChild<CommandWhenShootAnonExplosion>();
			createChild<CommandWhenShootOwnedExplosion>();
			createChild<CommandWhenShootBurn>();
			createChild<CommandWhenShootDie>();
			createChild<CommandWhenShootDisarm>();
			createChild<CommandWhenShootFreeze>();
			createChild<CommandWhenShootCower>();
			createChild<CommandWhenShootFlee>();
			createChild<CommandWhenShootPush>();
			createChild<CommandWhenShootDrive>();
			createChild<CommandWhenShootRevive>();
			createChild<CommandWhenShootRagdoll>();
			createChild<CommandWhenShootRepair>();
			createChild<CommandWhenShootWeaken>();
			createChild<CommandWhenShootDelete>();
			createChild<CommandWhenShootARMarker>();
			createChild<CommandWhenShootShowModelName>();
		}
	};
}

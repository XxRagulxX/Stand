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
	class WhenAimingBase : public CommandToggle
	{
	protected:
		int  m_currentTarget = 0;
		bool m_isNewTarget   = false;

		virtual void onAim(int entHandle) = 0;

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
			m_currentTarget = 0;
			m_isNewTarget   = false;
			onDisableExtra();
		}

		~WhenAimingBase() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}

		void onTick() override
		{
			auto ped = Self::GetPed();
			if (!ped)
			{
				m_currentTarget = 0;
				return;
			}
			int entHandle = 0;
			PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(Self::GetPlayer().GetId(), &entHandle);
			if (!entHandle)
			{
				m_currentTarget = 0;
				return;
			}
			m_isNewTarget   = (entHandle != m_currentTarget);
			m_currentTarget = entHandle;
			onAim(entHandle);
		}
	};

	class CommandWhenAimAnonExplosion : public WhenAimingBase
	{
	public:
		explicit CommandWhenAimAnonExplosion(CommandList* parent)
			: WhenAimingBase(parent, LIT("Anonymous Explosion"), CMDNAMES("whenaimexplodeanon"))
		{
		}

		void onAim(int entHandle) override
		{
			if (!m_isNewTarget)
				return;
			const Vector3 pos = ENTITY::GET_ENTITY_COORDS(entHandle, TRUE);
			FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 23, 1.0f, TRUE, FALSE, 0.0f, FALSE);
		}
	};

	class CommandWhenAimOwnedExplosion : public WhenAimingBase
	{
	public:
		explicit CommandWhenAimOwnedExplosion(CommandList* parent)
			: WhenAimingBase(parent, LIT("Owned Explosion"), CMDNAMES("whenaimexplodeowned"))
		{
		}

		void onAim(int entHandle) override
		{
			if (!m_isNewTarget)
				return;
			const int ped  = Self::GetPed().GetHandle();
			const Vector3 pos = ENTITY::GET_ENTITY_COORDS(entHandle, TRUE);
			FIRE::ADD_OWNED_EXPLOSION(ped, pos.x, pos.y, pos.z, 23, 1.0f, TRUE, FALSE, 0.0f);
		}
	};

	class CommandWhenAimBurn : public WhenAimingBase
	{
	public:
		explicit CommandWhenAimBurn(CommandList* parent)
			: WhenAimingBase(parent, LIT("Burn"), CMDNAMES("whenaimburn"))
		{
		}

		void onAim(int entHandle) override
		{
			if (!m_isNewTarget)
				return;
			const Vector3 pos = ENTITY::GET_ENTITY_COORDS(entHandle, TRUE);
			FIRE::START_SCRIPT_FIRE(pos.x, pos.y, pos.z, 25, FALSE);
		}
	};

	class CommandWhenAimDie : public WhenAimingBase
	{
	public:
		explicit CommandWhenAimDie(CommandList* parent)
			: WhenAimingBase(parent, LIT("Die"), CMDNAMES("whenaimdie"))
		{
		}

		void onAim(int entHandle) override
		{
			Entity ent{ entHandle };
			if (!ent.IsDead())
				ent.Kill();
		}
	};

	class CommandWhenAimDisarm : public WhenAimingBase
	{
	public:
		explicit CommandWhenAimDisarm(CommandList* parent)
			: WhenAimingBase(parent, LIT("Disarm"), CMDNAMES("whenaimdisarm"),
			      LIT("Will leave them with nothing but their fists."))
		{
		}

		void onAim(int entHandle) override
		{
			if (!m_isNewTarget || !ENTITY::IS_ENTITY_A_PED(entHandle))
				return;
			WEAPON::REMOVE_ALL_PED_WEAPONS(entHandle, TRUE);
		}
	};

	class CommandWhenAimFreeze : public WhenAimingBase
	{
	public:
		explicit CommandWhenAimFreeze(CommandList* parent)
			: WhenAimingBase(parent, LIT("Freeze"), CMDNAMES("whenaimfreeze"))
		{
		}

		void onAim(int entHandle) override
		{
			ENTITY::FREEZE_ENTITY_POSITION(entHandle, TRUE);
		}
	};

	class CommandWhenAimCower : public WhenAimingBase
	{
	public:
		explicit CommandWhenAimCower(CommandList* parent)
			: WhenAimingBase(parent, LIT("Cower"), CMDNAMES("whenaimcower"))
		{
		}

		void onAim(int entHandle) override
		{
			if (!m_isNewTarget || !ENTITY::IS_ENTITY_A_PED(entHandle))
				return;
			TASK::CLEAR_PED_TASKS_IMMEDIATELY(entHandle);
			TASK::TASK_COWER(entHandle, -1);
		}
	};

	class CommandWhenAimFlee : public WhenAimingBase
	{
	public:
		explicit CommandWhenAimFlee(CommandList* parent)
			: WhenAimingBase(parent, LIT("Flee"), CMDNAMES("whenaimflee"))
		{
		}

		void onAim(int entHandle) override
		{
			if (!m_isNewTarget || !ENTITY::IS_ENTITY_A_PED(entHandle))
				return;
			const int pedHandle = Self::GetPed().GetHandle();
			TASK::TASK_SMART_FLEE_PED(entHandle, pedHandle, 200.0f, -1, FALSE, FALSE);
		}
	};

	class CommandWhenAimPush : public WhenAimingBase
	{
	public:
		explicit CommandWhenAimPush(CommandList* parent)
			: WhenAimingBase(parent, LIT("Push Away"), CMDNAMES("whenaimpush"))
		{
		}

		void onAim(int entHandle) override
		{
			Entity ent{ entHandle };
			const auto playerPos = Self::GetPed().GetPosition();
			const auto entPos    = ent.GetPosition();
			const float dx  = entPos.x - playerPos.x;
			const float dy  = entPos.y - playerPos.y;
			const float dz  = entPos.z - playerPos.z;
			const float len = std::sqrt(dx * dx + dy * dy + dz * dz);
			if (len < 0.001f)
				return;
			const float scale = 3.0f / len;
			ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(
				entHandle, 1, dx * scale, dy * scale, dz * scale, TRUE, FALSE, TRUE, TRUE);
		}
	};

	class CommandWhenAimDrive : public WhenAimingBase
	{
	public:
		explicit CommandWhenAimDrive(CommandList* parent)
			: WhenAimingBase(parent, LIT("Drive"), CMDNAMES("whenaimdrive"))
		{
		}

		void onAim(int entHandle) override
		{
			if (!m_isNewTarget)
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

	class CommandWhenAimRevive : public WhenAimingBase
	{
	public:
		explicit CommandWhenAimRevive(CommandList* parent)
			: WhenAimingBase(parent, LIT("Revive"), CMDNAMES("whenaimrevive"))
		{
		}

		void onAim(int entHandle) override
		{
			if (!ENTITY::IS_ENTITY_A_PED(entHandle))
				return;
			PED::REVIVE_INJURED_PED(entHandle);
		}
	};

	class CommandWhenAimRagdoll : public WhenAimingBase
	{
	public:
		explicit CommandWhenAimRagdoll(CommandList* parent)
			: WhenAimingBase(parent, LIT("Ragdoll"), CMDNAMES("whenaimragdoll"))
		{
		}

		void onAim(int entHandle) override
		{
			if (!m_isNewTarget || !ENTITY::IS_ENTITY_A_PED(entHandle))
				return;
			PED::SET_PED_TO_RAGDOLL(entHandle, 3000, 3000, 0, FALSE, FALSE, FALSE);
		}
	};

	class CommandWhenAimRepair : public WhenAimingBase
	{
	public:
		explicit CommandWhenAimRepair(CommandList* parent)
			: WhenAimingBase(parent, LIT("Repair"), CMDNAMES("whenaimrepair"))
		{
		}

		void onAim(int entHandle) override
		{
			if (!m_isNewTarget)
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

	class CommandWhenAimWeaken : public WhenAimingBase
	{
	public:
		explicit CommandWhenAimWeaken(CommandList* parent)
			: WhenAimingBase(parent, LIT("Weaken"), CMDNAMES("whenaimweaken"),
			      LIT("Reduces their health so they'll die easily."))
		{
		}

		void onAim(int entHandle) override
		{
			if (!ENTITY::IS_ENTITY_A_PED(entHandle))
				return;
			Entity ent{ entHandle };
			if (!ent.IsDead() && ent.GetHealth() > 101)
				ent.SetHealth(101);
		}
	};

	class CommandWhenAimDelete : public WhenAimingBase
	{
	public:
		explicit CommandWhenAimDelete(CommandList* parent)
			: WhenAimingBase(parent, LIT("Delete"), CMDNAMES("whenaimdelete"))
		{
		}

		void onAim(int entHandle) override
		{
			if (!m_isNewTarget)
				return;
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(entHandle, FALSE, TRUE);
			ENTITY::DELETE_ENTITY(&entHandle);
		}
	};

	class CommandWhenAimARMarker : public WhenAimingBase
	{
	public:
		explicit CommandWhenAimARMarker(CommandList* parent)
			: WhenAimingBase(parent, LIT("AR Marker"), CMDNAMES("whenaimarmarker"))
		{
		}

		void onAim(int entHandle) override
		{
			if (!ENTITY::DOES_ENTITY_EXIST(entHandle))
				return;
			const Vector3 entPos = ENTITY::GET_ENTITY_COORDS(entHandle, TRUE);
			const Vector3 p{ entPos.x, entPos.y, entPos.z + 2.0f };
			GRAPHICS::DRAW_MARKER(0, p.x, p.y, p.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.6f, 0.6f, 0.6f, 255, 0, 0, 200, FALSE, TRUE, 2, FALSE, nullptr, nullptr, FALSE);
		}
	};

	class CommandWhenAimShowModelName : public WhenAimingBase
	{
	public:
		explicit CommandWhenAimShowModelName(CommandList* parent)
			: WhenAimingBase(parent, LIT("Show Model Name"), CMDNAMES("whenaimshowmodelname"))
		{
		}

		void onAim(int entHandle) override
		{
			if (!m_isNewTarget)
				return;
			const Hash modelHash = ENTITY::GET_ENTITY_MODEL(entHandle);
			Notifications::Show("When Aiming", std::format("0x{:08X}", modelHash));
		}
	};

	class CommandWhenAiming : public CommandList
	{
	public:
		explicit CommandWhenAiming(CommandList* parent)
			: CommandList(parent, LIT("When Aiming"), CMDNAMES("whenaiming"))
		{
			createChild<CommandWhenAimAnonExplosion>();
			createChild<CommandWhenAimOwnedExplosion>();
			createChild<CommandWhenAimBurn>();
			createChild<CommandWhenAimDie>();
			createChild<CommandWhenAimDisarm>();
			createChild<CommandWhenAimFreeze>();
			createChild<CommandWhenAimCower>();
			createChild<CommandWhenAimFlee>();
			createChild<CommandWhenAimPush>();
			createChild<CommandWhenAimDrive>();
			createChild<CommandWhenAimRevive>();
			createChild<CommandWhenAimRagdoll>();
			createChild<CommandWhenAimRepair>();
			createChild<CommandWhenAimWeaken>();
			createChild<CommandWhenAimDelete>();
			createChild<CommandWhenAimARMarker>();
			createChild<CommandWhenAimShowModelName>();
		}
	};
}

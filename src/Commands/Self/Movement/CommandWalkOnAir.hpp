#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandWalkOnAir : public CommandToggle
	{
	public:
		explicit CommandWalkOnAir(CommandList* const parent)
			: CommandToggle(parent, LIT("Walk On Air"), CMDNAMES("walkonair"), LIT("Walk on thin air."))
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			if (!Stand::Self::GetPed())
				return;
			ENTITY::SET_ENTITY_HAS_GRAVITY(Stand::Self::GetPed().GetHandle(), true);
			PED::SET_PED_GRAVITY(Stand::Self::GetPed().GetHandle(), true);
		}

		void onTick() override
		{
			if (!Stand::Self::GetPed())
				return;

			const auto handle = Stand::Self::GetPed().GetHandle();

			if (PED::IS_PED_RAGDOLL(handle))
			{
				ENTITY::SET_ENTITY_HAS_GRAVITY(handle, true);
				PED::SET_PED_GRAVITY(handle, true);
			}
			else
			{
				ENTITY::SET_ENTITY_HAS_GRAVITY(handle, false);
				PED::SET_PED_GRAVITY(handle, false);
			}

			if (!ENTITY::IS_ENTITY_IN_AIR(handle))
			{
				const auto vel = ENTITY::GET_ENTITY_VELOCITY(handle);
				if (vel.z != 0.0f)
					ENTITY::SET_ENTITY_VELOCITY(handle, vel.x, vel.y, 0.0f);
			}
		}

		~CommandWalkOnAir() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}

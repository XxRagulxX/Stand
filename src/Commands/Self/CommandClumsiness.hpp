#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

#include <chrono>

namespace Stand
{
	class CommandClumsiness : public CommandToggle
	{
		std::chrono::steady_clock::time_point m_ReenableAt{};

	public:
		explicit CommandClumsiness(CommandList* const parent)
			: CommandToggle(parent, LIT("Clumsiness"), { CMDNAME("clumsiness") }, LIT("Makes you ragdoll on collisions."))
		{
		}

		void onEnable(Click& click) override
		{
			m_ReenableAt = {};
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			if (auto ped = Stand::Self::GetPed())
				PED::SET_PED_RAGDOLL_ON_COLLISION(ped.GetHandle(), false);
		}

		void onTick() override
		{
			auto ped = Stand::Self::GetPed();
			if (!ped)
				return;

			const auto handle = ped.GetHandle();
			if (PED::IS_PED_RAGDOLL(handle))
			{
				m_ReenableAt = std::chrono::steady_clock::now() + std::chrono::seconds(2);
			}
			else if (std::chrono::steady_clock::now() >= m_ReenableAt)
			{
				PED::SET_PED_RAGDOLL_ON_COLLISION(handle, true);
			}
		}

		~CommandClumsiness() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}

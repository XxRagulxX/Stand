#pragma once
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/Self/CommandWetness.hpp"

namespace Stand
{
	class CommandLockWetness final : public CommandToggle
	{
		CommandWetness* const m_wetness;

	public:
		explicit CommandLockWetness(CommandList* parent, CommandWetness* wetness)
			: CommandToggle(parent, LIT("Lock Wetness"), CMDNAMES("lockwetness"))
			, m_wetness(wetness)
		{
		}

		void onEnable(Click& click) final;
		void onDisable(Click& click) final;
		void onTick() override;
		void applyDefaultState() final;
	};
}

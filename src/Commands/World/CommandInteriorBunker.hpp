#pragma once

#include "Commands/World/CommandInteriorCustomisable.hpp"

#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Stand/CommandToggleNoCorrelation.hpp"

namespace Stand
{
	class CommandInteriorBunker : public CommandInteriorCustomisable
	{
	public:
		inline static CommandInteriorBunker* instance;

		CommandListSelect* style;
		CommandToggleNoCorrelation* security;
		CommandToggleNoCorrelation* equipment_upgrade;
		CommandToggleNoCorrelation* gun_locker;
		CommandToggleNoCorrelation* personal_quarters;
		CommandToggleNoCorrelation* gun_range;

		explicit CommandInteriorBunker(CommandList* const parent);

	protected:
		v3 getPosition() const final;

		void toggleEntitySets(long long style, bool security, bool equipment_upgrade, bool gun_locker, bool personal_quarters, bool gun_range) const;
		void toggleEntitySets() const final;
		void randomiseEntitySets() const final;
	};
}

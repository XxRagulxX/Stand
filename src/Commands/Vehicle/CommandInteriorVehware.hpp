#pragma once

#include "Commands/World/CommandInteriorCustomisable.hpp"

#include "Commands/Widgets/CommandListSelect.hpp"

namespace Stand
{
	class CommandInteriorVehware : public CommandInteriorCustomisable
	{
	public:
		inline static CommandInteriorVehware* instance;

		CommandListSelect* style;

		explicit CommandInteriorVehware(CommandList* parent);

	protected:
		v3 getPosition() const final;

		void toggleEntitySets(long long style) const;
		void toggleEntitySets() const final;
		void randomiseEntitySets() const final;
	};
}

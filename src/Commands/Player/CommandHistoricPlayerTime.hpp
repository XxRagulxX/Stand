#pragma once

#include "Commands/Widgets/CommandReadonlyValueCopy.hpp"

#include "Core/Offset.hpp"

namespace Stand
{
#pragma pack(push, 1)
	class CommandHistoricPlayerTime : public CommandReadonlyValueCopy
	{
	private:
		const Offset<HistoricPlayer, time_t> value_offset;

	public:
		explicit CommandHistoricPlayerTime(CommandList* const parent, Label&& menu_name, Offset<HistoricPlayer, time_t> value_offset);

		void onTickInGameViewport() final;
		void onTickInWebViewport() final;
	};
#pragma pack(pop)
}

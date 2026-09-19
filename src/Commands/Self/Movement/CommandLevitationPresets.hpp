#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
	class CommandTabLevitation;

	class CommandLevitationPresets : public CommandList
	{
	public:
		class PresetLevitation : public CommandPhysical
		{
			CommandTabLevitation* const m_Tab;
		public:
			explicit PresetLevitation(CommandList* parent, CommandTabLevitation* tab);
			void onClick(Click& click) override;
		};

		class PresetNoClip : public CommandPhysical
		{
			CommandTabLevitation* const m_Tab;
		public:
			explicit PresetNoClip(CommandList* parent, CommandTabLevitation* tab);
			void onClick(Click& click) override;
		};

		explicit CommandLevitationPresets(CommandList* parent, CommandTabLevitation* tab);
	};
}

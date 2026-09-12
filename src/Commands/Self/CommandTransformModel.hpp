#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/ScriptMgr.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	class CommandTransformModel : public CommandPhysical
	{
		const Hash m_hash;

	public:
		CommandTransformModel(CommandList* const parent, Label&& label, Hash hash)
			: CommandPhysical(COMMAND_ACTION, parent, std::move(label), {}, NOLABEL)
			, m_hash(hash)
		{
		}

		void onClick(Click& click) override
		{
			for (int i = 0; !STREAMING::HAS_MODEL_LOADED(m_hash); ++i)
			{
				STREAMING::REQUEST_MODEL(m_hash);
				Script::current()->yield();
				if (i > 30)
					return;
			}
			PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), m_hash);
			PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(m_hash);
		}
	};
}

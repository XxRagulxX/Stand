#include "Commands/CommandLegacy.hpp"
#include "Scripting/Natives.hpp"

namespace Stand::Features
{
	class SkipConversation : CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			AUDIO::SKIP_TO_NEXT_SCRIPTED_CONVERSATION_LINE();
		}
	};

	static SkipConversation _SkipConversation{"skipconversation", "Skip Conversation", "Skips current conversation in the phone."};
}
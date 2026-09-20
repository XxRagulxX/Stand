#include "Commands/CommandLegacy.hpp"
#include "Rendering/Notifications.hpp"
#include "World/Self.hpp"
#include "Scripting/Scripts.hpp"
#include "Scripting/ScriptLocal.hpp"
#include "Scripting/Globals.hpp"
#include "Core/Pointers.hpp"
#include "Util/Timer.hpp"

namespace Stand::Features
{
	class CallMechanic : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			if (!*Pointers.IsSessionStarted || Scripts::IsScriptActive("AM_CONTACT_REQUESTS"_J))
			{
				Notifications::Show("Mechanic", "Not safe to call the mechanic at the moment.", NotificationType::Error);
				return;
			}

			if (auto freemode = Scripts::FindScriptThread("freemode"_J))
			{
				auto data = ScriptLocal(freemode, 8854);
				*data.at(3).as<int*>() = 235;
				*data.at(3).at(16).as<int*>() = Self::GetPlayer().GetId();
				*data.at(3).at(1).as<int*>() = 89;
				*data.at(2).as<int*>() = "AM_CONTACT_REQUESTS"_J;

				auto args = data.at(3).as<void*>();
				if (auto id = Scripts::StartScript("AM_CONTACT_REQUESTS"_J, eStackSizes::SCRIPT_XML, args, 21))
				{
					if (auto thread = Scripts::FindScriptThreadByID(id))
					{
						*ScriptLocal(thread, 535).as<int*>() = 1;
						Globals::CEO_GLOBAL.at(4373).at(260).at(7, 2).as<TIMER*>()->Destroy();
					}
				}
				else
				{
					Notifications::Show("Mechanic", "Failed to call the mechanic.", NotificationType::Error);
				}
			}
		}
	};

	static CallMechanic _CallMechanic{"callmechanic", "Call Mechanic", "Allows you to request your personal vehicles."};
}
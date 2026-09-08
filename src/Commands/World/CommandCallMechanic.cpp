#include "Commands/CommandLegacy.hpp"
#include "Rendering/Notifications.hpp"
#include "World/Self.hpp"
#include "Scripting/Scripts.hpp"
#include "Scripting/ScriptLocal.hpp"
#include "Scripting/ScriptGlobal.hpp"
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
				*data.At(3).As<int*>() = 235;
				*data.At(3).At(16).As<int*>() = Self::GetPlayer().GetId();
				*data.At(3).At(1).As<int*>() = 89;
				*data.At(2).As<int*>() = "AM_CONTACT_REQUESTS"_J;

				auto args = data.At(3).As<void*>();
				if (auto id = Scripts::StartScript("AM_CONTACT_REQUESTS"_J, eStackSizes::SCRIPT_XML, args, 21))
				{
					if (auto thread = Scripts::FindScriptThreadByID(id))
					{
						*ScriptLocal(thread, 535).As<int*>() = 1;
						ScriptGlobal(2686124).At(4373).At(260).At(7, 2).As<TIMER*>()->Destroy();
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
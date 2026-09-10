#include "Commands/CommandInput.hpp"
#include "Commands/Widgets/CommandLambdaAction.hpp"
#include "Config/ProfileManager.hpp"

#include <cstdlib>
#include <filesystem>
#include <windows.h>
#include <shellapi.h>

namespace Stand::Features
{
	class CommandNewProfile : public CommandInput
	{
	public:
		CommandNewProfile()
			: CommandInput("newprofile", "Create Profile",
				"Save the state of every command to a new profile.")
		{
		}

		bool ShouldSaveState() const override
		{
			return false;
		}

		void OnChange() override
		{
			if (!m_StringValue.empty())
				ProfileManager::SaveProfile(m_StringValue);
		}
	};

	static CommandNewProfile g_NewProfile;

	static StandWidgets::CommandLambdaAction _OpenProfilesFolder{"openprofilesfolder",
	    "Open Folder",
	    "Opens the profiles folder.",
	    [] {
		    auto path = ProfileManager::GetProfilesPath();
		    std::filesystem::create_directories(path);
		    ShellExecuteA(nullptr, "explore", path.string().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
	    }};
}

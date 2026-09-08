#include "Commands/CommandListSelect.hpp"
#include "Core/FileMgr.hpp"
#include "Rendering/HeaderBanner.hpp"

namespace Stand::Features
{
	// Ported from real Stand's own "Custom Header" (Commands/Widgets/
	// CommandHeader.hpp/.cpp on origin/stand-reference) - a local image
	// folder the user drops files into, shown as a banner above the
	// menu's own addressbar (see Rendering/HeaderBanner.hpp for the
	// draw side) - only real Stand's own value >= 200 ("Custom Header")
	// branch is ported: none of its ~26 curated downloadable headers or
	// their animated-download variants, both of which need stand.sh
	// network access this project deliberately doesn't use.
	//
	// The folder itself lives at this project's own %appdata%/
	// StandEnhanced/Headers (FileMgr::GetProjectFolder("./Headers"), the
	// same "sibling of wherever this project already keeps its own user
	// files" precedent CommandSavedVehicles/LuaFileMgr already set) in
	// place of real Stand's own getCustomHeaderDirectory() (%appdata%/
	// Headers/Custom Header under the game's own install-adjacent
	// path) - created eagerly from GridRenderer::Init() (see that
	// function's own comment) so it exists and is easy to find as soon
	// as the menu is ready, not only after this command is first
	// switched to Custom.
	//
	// {0, 1} instead of real Stand's own {0, 200} - this project's own
	// small-CommandListSelect convention (see CommandTabsPos), not a
	// value with meaning of its own worth preserving now that every
	// other option in real Stand's own list is out of scope.
	class CommandHeader : public CommandListSelect
	{
	public:
		CommandHeader() :
		    CommandListSelect("header",
		        "Header",
		        "A banner image shown above the menu. Drop image files into the local Headers folder, then select Custom.",
		        {{0, "Hide"}, {1, "Custom"}},
		        0)
		{
		}

	protected:
		void OnChange() override
		{
			Sync();
		}

		void LoadState(nlohmann::json& value) override
		{
			CommandListSelect::LoadState(value);
			Sync();
		}

	private:
		void Sync()
		{
			if (GetState() == 1)
			{
				auto folder = FileMgr::GetProjectFolder("./Headers");
				Rendering::HeaderBanner::LoadFromFolder(folder.Path());
			}
			else
			{
				Rendering::HeaderBanner::Clear();
			}
		}
	};

	static CommandHeader _Header{};
}

#include "Commands/CommandColourCustom.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Features
{
	class CommandPrimaryColour : public CommandColourCustom
	{
	public:
		CommandPrimaryColour() :
		    CommandColourCustom("primary",
		        "Primary Colour",
		        "The one accent colour used everywhere in this menu - a sidebar/tab's active entry, a toggle's ON state, a button's fill.",
		        ImVec4(1.f, 0.f, 1.f, 1.f))
		{
		}

	protected:
		void OnChange() override
		{
			Sync();
		}

		void LoadState(nlohmann::json& value) override
		{
			CommandColourCustom::LoadState(value);
			Sync();
		}

	private:
		void Sync()
		{
			auto c = GetState();
			Rendering::Theme::kAccent = {c.x, c.y, c.z, c.w};
		}
	};

	static CommandPrimaryColour _PrimaryColour{};
}

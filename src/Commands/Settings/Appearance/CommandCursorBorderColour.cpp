#include "Commands/CommandColourCustom.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Features
{
	class CommandCursorBorderColour : public CommandColourCustom
	{
	public:
		CommandCursorBorderColour() :
		    CommandColourCustom("cursorborder", "Colour", "The colour of the cursor border.", ImVec4(1.f, 1.f, 1.f, 1.f))
		{
		}

	protected:
		void OnChange() override { Sync(); }
		void LoadState(nlohmann::json& value) override { CommandColourCustom::LoadState(value); Sync(); }

	private:
		void Sync()
		{
			auto c = GetState();
			Rendering::Theme::kCursorBorderColour = {c.x, c.y, c.z, c.w};
		}
	};

	static CommandCursorBorderColour _CursorBorderColour{};
}

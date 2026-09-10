#include "Commands/CommandColourCustom.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Features
{
	class CommandHudColour : public CommandColourCustom
	{
	public:
		CommandHudColour() :
		    CommandColourCustom("hud", "HUD Colour", "Colour used for HUD overlay elements.", ImVec4(1.f, 0.f, 1.f, 1.f))
		{
		}

	protected:
		void OnChange() override { Sync(); }
		void LoadState(nlohmann::json& value) override { CommandColourCustom::LoadState(value); Sync(); }

	private:
		void Sync()
		{
			auto c = GetState();
			Rendering::Theme::kHud = {c.x, c.y, c.z, c.w};
		}
	};

	class CommandArColour : public CommandColourCustom
	{
	public:
		CommandArColour() :
		    CommandColourCustom("ar", "AR Colour", "Colour used for augmented reality overlay elements.", ImVec4(1.f, 0.f, 1.f, 1.f))
		{
		}

	protected:
		void OnChange() override { Sync(); }
		void LoadState(nlohmann::json& value) override { CommandColourCustom::LoadState(value); Sync(); }

	private:
		void Sync()
		{
			auto c = GetState();
			Rendering::Theme::kAr = {c.x, c.y, c.z, c.w};
		}
	};

	class CommandMinigameColour : public CommandColourCustom
	{
	public:
		CommandMinigameColour() :
		    CommandColourCustom("minigame", "Minigame Colour", "Colour used for minigame overlay elements.", ImVec4(1.f, 0.f, 1.f, 1.f))
		{
		}

	protected:
		void OnChange() override { Sync(); }
		void LoadState(nlohmann::json& value) override { CommandColourCustom::LoadState(value); Sync(); }

	private:
		void Sync()
		{
			auto c = GetState();
			Rendering::Theme::kMinigame = {c.x, c.y, c.z, c.w};
		}
	};

	static CommandHudColour _HudColour{};
	static CommandArColour _ArColour{};
	static CommandMinigameColour _MinigameColour{};
}

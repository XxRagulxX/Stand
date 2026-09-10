#include "Commands/CommandColourCustom.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Features
{
	class CommandFocusTextColour : public CommandColourCustom
	{
	public:
		CommandFocusTextColour() :
		    CommandColourCustom("focustext", "Focused Text Colour", "Colour of text in the focused/selected menu row.", ImVec4(1.f, 1.f, 1.f, 1.f))
		{
		}

	protected:
		void OnChange() override { Sync(); }
		void LoadState(nlohmann::json& value) override { CommandColourCustom::LoadState(value); Sync(); }

	private:
		void Sync()
		{
			auto c = GetState();
			Rendering::Theme::kFocusText = {c.x, c.y, c.z, c.w};
		}
	};

	class CommandFocusRightTextColour : public CommandColourCustom
	{
	public:
		CommandFocusRightTextColour() :
		    CommandColourCustom("focusrighttext", "Focused Right-Bound Text Colour", "Colour of right-aligned text (values, arrows) in the focused/selected menu row.", ImVec4(1.f, 1.f, 1.f, 1.f))
		{
		}

	protected:
		void OnChange() override { Sync(); }
		void LoadState(nlohmann::json& value) override { CommandColourCustom::LoadState(value); Sync(); }

	private:
		void Sync()
		{
			auto c = GetState();
			Rendering::Theme::kFocusRightText = {c.x, c.y, c.z, c.w};
		}
	};

	class CommandFocusTextureColour : public CommandColourCustom
	{
	public:
		CommandFocusTextureColour() :
		    CommandColourCustom("focustexture", "Focused Texture Colour", "Colour of texture/sprite elements (toggle indicator border) in the focused/selected menu row.", ImVec4(1.f, 1.f, 1.f, 1.f))
		{
		}

	protected:
		void OnChange() override { Sync(); }
		void LoadState(nlohmann::json& value) override { CommandColourCustom::LoadState(value); Sync(); }

	private:
		void Sync()
		{
			auto c = GetState();
			Rendering::Theme::kFocusTexture = {c.x, c.y, c.z, c.w};
		}
	};

	static CommandFocusTextColour _FocusTextColour{};
	static CommandFocusRightTextColour _FocusRightTextColour{};
	static CommandFocusTextureColour _FocusTextureColour{};
}

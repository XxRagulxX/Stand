#include "Commands/CommandColourCustom.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Features
{
	class CommandUnfocusedTextColour : public CommandColourCustom
	{
	public:
		CommandUnfocusedTextColour() :
		    CommandColourCustom("unfocusedtext", "Unfocused Text Colour", "Colour of text in unfocused/non-selected menu rows.", ImVec4(1.f, 1.f, 1.f, 1.f))
		{
		}

	protected:
		void OnChange() override { Sync(); }
		void LoadState(nlohmann::json& value) override { CommandColourCustom::LoadState(value); Sync(); }

	private:
		void Sync()
		{
			auto c = GetState();
			Rendering::Theme::kUnfocusedText = {c.x, c.y, c.z, c.w};
		}
	};

	class CommandUnfocusedRightTextColour : public CommandColourCustom
	{
	public:
		CommandUnfocusedRightTextColour() :
		    CommandColourCustom("unfocusedrighttext", "Unfocused Right-Bound Text Colour", "Colour of right-aligned text (values, arrows) in unfocused/non-selected menu rows.", ImVec4(1.f, 1.f, 1.f, 1.f))
		{
		}

	protected:
		void OnChange() override { Sync(); }
		void LoadState(nlohmann::json& value) override { CommandColourCustom::LoadState(value); Sync(); }

	private:
		void Sync()
		{
			auto c = GetState();
			Rendering::Theme::kUnfocusedRightText = {c.x, c.y, c.z, c.w};
		}
	};

	class CommandUnfocusedTextureColour : public CommandColourCustom
	{
	public:
		CommandUnfocusedTextureColour() :
		    CommandColourCustom("unfocusedtexture", "Unfocused Texture Colour", "Colour of texture/sprite elements (toggle indicator border) in unfocused/non-selected menu rows.", ImVec4(1.f, 1.f, 1.f, 1.f))
		{
		}

	protected:
		void OnChange() override { Sync(); }
		void LoadState(nlohmann::json& value) override { CommandColourCustom::LoadState(value); Sync(); }

	private:
		void Sync()
		{
			auto c = GetState();
			Rendering::Theme::kUnfocusedTexture = {c.x, c.y, c.z, c.w};
		}
	};

	static CommandUnfocusedTextColour _UnfocusedTextColour{};
	static CommandUnfocusedRightTextColour _UnfocusedRightTextColour{};
	static CommandUnfocusedTextureColour _UnfocusedTextureColour{};
}

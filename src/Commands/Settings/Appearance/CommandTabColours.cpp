#include "Commands/Settings/CommandTabColours.hpp"

#include "Commands/CommandColourCustom.hpp"
#include "Commands/Commands.hpp"
#include "Util/Joaat.hpp"

namespace Stand
{
	void CommandCopyFocusTextToRightText::onClick(Click& click)
	{
		auto src = Rendering::Theme::kFocusText;
		Rendering::Theme::kFocusRightText = src;
		if (auto* cmd = Commands::GetCommand<CommandColourCustom>("focusrighttext"_J))
			cmd->SetState(ImVec4{src.x, src.y, src.z, src.w});
	}

	void CommandCopyFocusTextToTexture::onClick(Click& click)
	{
		auto src = Rendering::Theme::kFocusText;
		Rendering::Theme::kFocusTexture = src;
		if (auto* cmd = Commands::GetCommand<CommandColourCustom>("focustexture"_J))
			cmd->SetState(ImVec4{src.x, src.y, src.z, src.w});
	}

	void CommandCopyUnfocusedTextToRightText::onClick(Click& click)
	{
		auto src = Rendering::Theme::kUnfocusedText;
		Rendering::Theme::kUnfocusedRightText = src;
		if (auto* cmd = Commands::GetCommand<CommandColourCustom>("unfocusedrighttext"_J))
			cmd->SetState(ImVec4{src.x, src.y, src.z, src.w});
	}

	void CommandCopyUnfocusedTextToTexture::onClick(Click& click)
	{
		auto src = Rendering::Theme::kUnfocusedText;
		Rendering::Theme::kUnfocusedTexture = src;
		if (auto* cmd = Commands::GetCommand<CommandColourCustom>("unfocusedtexture"_J))
			cmd->SetState(ImVec4{src.x, src.y, src.z, src.w});
	}

	void CommandCopyPrimaryToHud::onClick(Click& click)
	{
		auto src = Rendering::Theme::kAccent;
		Rendering::Theme::kHud = src;
		if (auto* cmd = Commands::GetCommand<CommandColourCustom>("hud"_J))
			cmd->SetState(ImVec4{src.x, src.y, src.z, src.w});
	}

	void CommandCopyPrimaryToAr::onClick(Click& click)
	{
		auto src = Rendering::Theme::kAccent;
		Rendering::Theme::kAr = src;
		if (auto* cmd = Commands::GetCommand<CommandColourCustom>("ar"_J))
			cmd->SetState(ImVec4{src.x, src.y, src.z, src.w});
	}

	void CommandCopyPrimaryToMinigame::onClick(Click& click)
	{
		auto src = Rendering::Theme::kAccent;
		Rendering::Theme::kMinigame = src;
		if (auto* cmd = Commands::GetCommand<CommandColourCustom>("minigame"_J))
			cmd->SetState(ImVec4{src.x, src.y, src.z, src.w});
	}
}

namespace Stand::Features
{
	Stand::CommandTabColours& GetCommandTabColours()
	{
		static Stand::CommandTabColours instance{};
		return instance;
	}
}

#include "Commands/Settings/Appearance/CommandTabColours.hpp"

namespace Stand
{
	void CommandCopyFocusTextToRightText::onClick(Click& click)
	{
		auto src = Rendering::Theme::kFocusText;
		DirectX::SimpleMath::Color srcColor{src.x, src.y, src.z, src.w};
		auto& tab = Features::GetCommandTabColours();
		tab.focusRightText->set(click, srcColor);
		Rendering::Theme::kFocusRightText = src;
	}

	void CommandCopyFocusTextToTexture::onClick(Click& click)
	{
		auto src = Rendering::Theme::kFocusText;
		DirectX::SimpleMath::Color srcColor{src.x, src.y, src.z, src.w};
		auto& tab = Features::GetCommandTabColours();
		tab.focusTexture->set(click, srcColor);
		Rendering::Theme::kFocusTexture = src;
	}

	void CommandCopyUnfocusedTextToRightText::onClick(Click& click)
	{
		auto src = Rendering::Theme::kUnfocusedText;
		DirectX::SimpleMath::Color srcColor{src.x, src.y, src.z, src.w};
		auto& tab = Features::GetCommandTabColours();
		tab.unfocusedRightText->set(click, srcColor);
		Rendering::Theme::kUnfocusedRightText = src;
	}

	void CommandCopyUnfocusedTextToTexture::onClick(Click& click)
	{
		auto src = Rendering::Theme::kUnfocusedText;
		DirectX::SimpleMath::Color srcColor{src.x, src.y, src.z, src.w};
		auto& tab = Features::GetCommandTabColours();
		tab.unfocusedTexture->set(click, srcColor);
		Rendering::Theme::kUnfocusedTexture = src;
	}

	void CommandCopyPrimaryToHud::onClick(Click& click)
	{
		auto src = Rendering::Theme::kAccent;
		DirectX::SimpleMath::Color srcColor{src.x, src.y, src.z, src.w};
		auto& tab = Features::GetCommandTabColours();
		tab.hud->set(click, srcColor);
		Rendering::Theme::kHud = src;
	}

	void CommandCopyPrimaryToAr::onClick(Click& click)
	{
		auto src = Rendering::Theme::kAccent;
		DirectX::SimpleMath::Color srcColor{src.x, src.y, src.z, src.w};
		auto& tab = Features::GetCommandTabColours();
		tab.ar->set(click, srcColor);
		Rendering::Theme::kAr = src;
	}

	void CommandCopyPrimaryToMinigame::onClick(Click& click)
	{
		auto src = Rendering::Theme::kAccent;
		DirectX::SimpleMath::Color srcColor{src.x, src.y, src.z, src.w};
		auto& tab = Features::GetCommandTabColours();
		tab.minigame->set(click, srcColor);
		Rendering::Theme::kMinigame = src;
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

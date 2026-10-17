#include "Commands/Stand/CommandMenuRainbow.hpp"

#include "Rendering/StandPort/ColourUtil.hpp"
#include "Rendering/Theme.hpp"

namespace Stand
{
	CommandMenuRainbow::CommandMenuRainbow(CommandList* const parent)
		: CommandColourRainbow(parent, LOC("LGBT"), { CMDNAME("rainbow") }, Rendering::Theme::kAccent)
	{
	}

	static void negateXMFLOAT4IfInsufficientContrast(DirectX::XMFLOAT4& c, const DirectX::XMFLOAT4& against)
	{
		if (!ColourUtil::isContrastSufficient(c, against))
			c = {1.f - c.x, 1.f - c.y, 1.f - c.z, c.w};
	}

	void CommandMenuRainbow::onHueChanged()
	{
		negateXMFLOAT4IfInsufficientContrast(Rendering::Theme::kFocusText, m_Target);
		negateXMFLOAT4IfInsufficientContrast(Rendering::Theme::kFocusRightText, m_Target);
		negateXMFLOAT4IfInsufficientContrast(Rendering::Theme::kFocusTexture, m_Target);
	}
}

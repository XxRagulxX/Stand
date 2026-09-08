#include "Rendering/GridItemCommandToggle.hpp"

#include "Commands/Commands.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kIndicatorSize = 16.f;
		constexpr float kBorderWidth = 2.f;
	}

	GridItemCommandToggle::GridItemCommandToggle(int16_t width, int16_t height, joaat_t id, std::optional<std::string> labelOverride) :
	    GridItem(GRIDITEM_INDIFFERENT, width, height),
	    m_Command(Commands::GetCommand<CommandToggleLegacy>(id)),
	    m_LabelOverride(std::move(labelOverride))
	{
	}

	std::string GridItemCommandToggle::GetDescription() const
	{
		return m_Command ? m_Command->GetDescription() : std::string{};
	}

	const std::string& GridItemCommandToggle::Label() const
	{
		static const std::string unknown = "Unknown!";
		if (!m_Command)
			return unknown;

		return m_LabelOverride.has_value() ? *m_LabelOverride : m_Command->GetLabel();
	}

	void GridItemCommandToggle::draw()
	{
		if (isKeyboardFocused())
			GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);

		const float indicatorX = x + width - kIndicatorSize;
		const float indicatorY = y + std::max(0.f, (height - kIndicatorSize) * 0.5f);

		const auto borderColour = m_Command ? Theme::kText : Theme::kError;
		const auto fillColour = !m_Command ? Theme::kError : (m_Command->GetState() ? Theme::kAccent : Theme::kPanelBackground);
		GridRenderer::DrawRect(indicatorX, indicatorY, kIndicatorSize, kIndicatorSize, borderColour);
		GridRenderer::DrawRect(indicatorX + kBorderWidth,
		    indicatorY + kBorderWidth,
		    kIndicatorSize - kBorderWidth * 2.f,
		    kIndicatorSize - kBorderWidth * 2.f,
		    fillColour);
	}

	void GridItemCommandToggle::drawText()
	{
		const auto& label = Label();
		const float textY = y + std::max(0.f, (height - GridRenderer::MeasureText(label.c_str()).y) * 0.5f);
		GridRenderer::DrawText(x + 5.f, textY, label.c_str(), Theme::kText);
	}

	void GridItemCommandToggle::onClick(int16_t, int16_t)
	{
		activate();
	}

	void GridItemCommandToggle::activate()
	{
		if (!m_Command)
			return;

		// Matches BoolCommandItem::Draw() exactly: read the live state,
		// flip it, SetState() - not a locally-tracked bool.
		m_Command->SetState(!m_Command->GetState());
	}
}

#include "Rendering/GridItemCommandReadonlyValue.hpp"

#include "Commands/Commands.hpp"
#include "Commands/Widgets/CommandReadonlyValue.hpp"
#include "Rendering/Clipboard.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <utility>

namespace Stand::Rendering
{
	namespace
	{
		constexpr auto kCopiedFlashDuration = std::chrono::milliseconds(1000);
	}

	GridItemCommandReadonlyValue::GridItemCommandReadonlyValue(int16_t width, int16_t height, joaat_t id, std::optional<std::string> labelOverride) :
	    GridItem(GRIDITEM_INDIFFERENT, width, height),
	    m_Id(id),
	    m_LabelOverride(std::move(labelOverride))
	{
	}

	void GridItemCommandReadonlyValue::draw()
	{
		if (isKeyboardFocused())
			GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);
	}

	std::string GridItemCommandReadonlyValue::GetDescription() const
	{
		auto* command = Commands::GetCommand<StandWidgets::CommandReadonlyValue>(m_Id);
		return command ? command->GetDescription() : std::string{};
	}

	void GridItemCommandReadonlyValue::drawText()
	{
		auto* command = Commands::GetCommand<StandWidgets::CommandReadonlyValue>(m_Id);

		const std::string label = m_LabelOverride.has_value() ? *m_LabelOverride : (command ? command->GetLabel() : "Unknown!");
		const auto labelSize = GridRenderer::MeasureText(label.c_str());
		GridRenderer::DrawText(x + 5.f, y + std::max(0.f, (height - labelSize.y) * 0.5f), label.c_str(), Theme::kText);

		const bool showingCopied = std::chrono::steady_clock::now() < m_CopiedUntil;
		const std::string value = showingCopied ? "Copied!" : (command ? command->GetValue() : "?");
		const auto valueSize = GridRenderer::MeasureText(value.c_str());
		GridRenderer::DrawText(x + width - valueSize.x - 5.f,
		    y + std::max(0.f, (height - valueSize.y) * 0.5f),
		    value.c_str(),
		    command ? Theme::kPlaceholderText : Theme::kError);
	}

	void GridItemCommandReadonlyValue::onClick(int16_t, int16_t)
	{
		activate();
	}

	void GridItemCommandReadonlyValue::activate()
	{
		CopyToClipboard();
	}

	void GridItemCommandReadonlyValue::CopyToClipboard()
	{
		auto* command = Commands::GetCommand<StandWidgets::CommandReadonlyValue>(m_Id);
		if (!command)
			return;

		Clipboard::SetText(command->GetValue());
		m_CopiedUntil = std::chrono::steady_clock::now() + kCopiedFlashDuration;
	}
}

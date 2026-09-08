#include "Rendering/GridItemCommandButton.hpp"

#include "Commands/Commands.hpp"
#include "Scripting/FiberPool.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>

namespace Stand::Rendering
{
	GridItemCommandButton::GridItemCommandButton(int16_t width, int16_t height, joaat_t id, std::optional<std::string> labelOverride) :
	    GridItem(GRIDITEM_INDIFFERENT, width, height),
	    m_Command(Commands::GetCommand<CommandLegacy>(id)),
	    m_LabelOverride(std::move(labelOverride))
	{
	}

	std::string GridItemCommandButton::GetDescription() const
	{
		return m_Command ? m_Command->GetDescription() : std::string{};
	}

	const std::string& GridItemCommandButton::Label() const
	{
		static const std::string unknown = "Unknown!";
		if (!m_Command)
			return unknown;

		return m_LabelOverride.has_value() ? *m_LabelOverride : m_Command->GetLabel();
	}

	void GridItemCommandButton::draw()
	{
		// Unresolved-command error fill stays (a real problem worth
		// flagging, unrelated to the question below) - but otherwise,
		// focused-only highlight, no fill when merely unfocused - see
		// GridItemFolder.cpp's identical comment for why (confirmed
		// against real Stand's own row-rendering source).
		if (!m_Command)
			GridRenderer::DrawRect(x, y, width, height, Theme::kError);
		else if (isKeyboardFocused())
			GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);
	}

	void GridItemCommandButton::drawText()
	{
		const auto& label = Label();
		const auto size = GridRenderer::MeasureText(label.c_str());
		const float textY = y + std::max(0.f, (height - size.y) * 0.5f);
		GridRenderer::DrawText(x + 5.f, textY, label.c_str(), Theme::kText);
	}

	void GridItemCommandButton::onClick(int16_t, int16_t)
	{
		activate();
	}

	void GridItemCommandButton::activate()
	{
		if (!m_Command)
			return;

		// Matches CommandItem::Draw()'s own FiberPool::queueJob([command] {
		// command->Call(); }) exactly - captured by value (a raw pointer),
		// not 'this', in case this item is ever destroyed before the job runs.
		auto* command = m_Command;
		FiberPool::queueJob([command] {
			command->Call();
		});
	}
}

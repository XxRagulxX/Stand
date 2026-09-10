#include "Rendering/GridItemStandCommand.hpp"

#include "Commands/Stand/CommandToggleNoCorrelation.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Menu/Click.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/GridStandCommandList.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/FiberPool.hpp"

#include <algorithm>
#include <string>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kIndicatorSize = 16.f;
		constexpr float kIndicatorBorderWidth = 2.f;
		constexpr float kArrowGap = 5.f;
		constexpr float kButtonSize = 22.f;
		constexpr float kValueWidth = 40.f;
		constexpr float kGap = 6.f;

		std::string Label(Stand::Command* command)
		{
			if (auto* physical = command->getPhysical())
			{
				auto label = physical->getMenuName().getLocalisedUtf8();

				// Same " [Ctrl+G]"-style suffix real Stand's own
				// GridItemList::update() appends per-hotkey (confirmed
				// against origin/stand-reference) - see
				// CommandHotkeyDispatch.hpp's own class comment for what
				// actually happens when one of these is pressed.
				for (const auto& hotkey : physical->hotkeys)
					label.append(" [").append(hotkey.toString()).append("]");

				return label;
			}

			return "Unknown!";
		}
	}

	GridItemStandCommand::GridItemStandCommand(int16_t width, int16_t height, Stand::Command* command) :
	    GridItem(GRIDITEM_INDIFFERENT, width, height),
	    m_Command(command)
	{
	}

	std::string GridItemStandCommand::GetDescription() const
	{
		if (!m_Command)
			return {};

		auto* physical = m_Command->getPhysical();
		if (!physical)
			return {};

		// help_text, then "Command: <name> [on/off]" (for a toggle) on
		// its own line below - matches real Stand's own populateCorner()
		// (CommandPhysical.cpp on origin/stand-reference), which shows
		// help_text and getCommandSyntax() as separate stacked lines, not
		// run together into one paragraph. '\n' here is a hard line
		// break, not just whitespace - see DescriptionPanel::WrappedLines()'s
		// own comment for why it's split out before word-wrapping.
		std::string result = physical->help_text.getLocalisedUtf8();
		if (auto syntax = physical->getCommandSyntax(); !syntax.empty())
		{
			if (!result.empty())
				result += '\n';
			result += syntax;
		}

		return result;
	}

	void GridItemStandCommand::draw()
	{
		if (Theme::kBorderWidth > 0)
		{
			const float bw = static_cast<float>(Theme::kBorderWidth);
			GridRenderer::DrawRect(x, y, width, bw, Theme::kBorderColour);
			GridRenderer::DrawRect(x, y + height - bw, width, bw, Theme::kBorderColour);
			GridRenderer::DrawRect(x, y, bw, height, Theme::kBorderColour);
			GridRenderer::DrawRect(x + width - bw, y, bw, height, Theme::kBorderColour);
		}

		if (isKeyboardFocused())
		{
			if (Theme::kCursorBorderWidth > 0)
			{
				const float bw = static_cast<float>(Theme::kCursorBorderWidth);
				GridRenderer::DrawRect(x - bw, y - bw, width + bw * 2.f, bw, Theme::kCursorBorderColour);
				GridRenderer::DrawRect(x - bw, y + height, width + bw * 2.f, bw, Theme::kCursorBorderColour);
				GridRenderer::DrawRect(x - bw, y, bw, height, Theme::kCursorBorderColour);
				GridRenderer::DrawRect(x + width, y, bw, height, Theme::kCursorBorderColour);
			}
			GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);
		}

		{
			const auto mode = Theme::kLeftTextures;
			const bool isToggle = m_Command && m_Command->isToggle();
			const bool drawAll = (mode == Theme::LeftTexturesMode::AllCommands || mode == Theme::LeftTexturesMode::AllCommandsCompact);
			const bool drawToggle = (mode == Theme::LeftTexturesMode::TogglesOnly || mode == Theme::LeftTexturesMode::TogglesOnlyCompact);
			const bool compact = (mode == Theme::LeftTexturesMode::AllCommandsCompact || mode == Theme::LeftTexturesMode::TogglesOnlyCompact);

			if (drawAll || (drawToggle && isToggle))
			{
				const auto& texColour = isKeyboardFocused() ? Theme::kFocusTexture : Theme::kUnfocusedTexture;
				if (compact)
				{
					GridRenderer::DrawRect(x, y, 4.f, height, texColour);
				}
				else
				{
					const float texSize = kIndicatorSize;
					const float texY = y + std::max(0.f, (height - texSize) * 0.5f);
					GridRenderer::DrawRect(x + 2.f, texY, texSize, texSize, texColour);
				}
			}
		}

		if (!m_Command || !m_Command->isToggle())
			return;

		// Same checkbox visual as this project's own GridItemCommandToggle.cpp -
		// deliberately kept identical so a Stand-backed row and a
		// StandEnhanced-backed one look indistinguishable while both exist side
		// by side during the migration.
		auto* toggle = m_Command->as<Stand::CommandToggleNoCorrelation>();
		const float indicatorX = x + width - kIndicatorSize;
		const float indicatorY = y + std::max(0.f, (height - kIndicatorSize) * 0.5f);
		GridRenderer::DrawRect(indicatorX, indicatorY, kIndicatorSize, kIndicatorSize, isKeyboardFocused() ? Theme::kFocusTexture : Theme::kUnfocusedTexture);
		GridRenderer::DrawRect(indicatorX + kIndicatorBorderWidth,
		    indicatorY + kIndicatorBorderWidth,
		    kIndicatorSize - kIndicatorBorderWidth * 2.f,
		    kIndicatorSize - kIndicatorBorderWidth * 2.f,
		    toggle->m_on ? Theme::kAccent : Theme::kPanelBackground);
	}

	void GridItemStandCommand::drawText()
	{
		using Rendering::GridRenderer;

		if (!m_Command)
		{
			GridRenderer::DrawText(x + 5.f, y, "Unknown!", Theme::kError);
			return;
		}

		const bool focused = isKeyboardFocused();
		const auto& textColour = focused ? Theme::kFocusText : Theme::kUnfocusedText;
		const auto& rightColour = focused ? Theme::kFocusRightText : Theme::kUnfocusedRightText;

		const float textScale = Theme::kCommandTextScale;
		const auto label = Label(m_Command);
		const auto labelSize = GridRenderer::MeasureText(label.c_str(), textScale);
		GridRenderer::DrawText(x + Theme::kCommandTextXOffset, y + Theme::kCommandTextYOffset + std::max(0.f, (height - labelSize.y) * 0.5f), label.c_str(), textColour, textScale);

		if (m_Command->isList())
		{
			const auto arrowSize = GridRenderer::MeasureText(">");
			GridRenderer::DrawText(x + width - arrowSize.x - kArrowGap, y + std::max(0.f, (height - arrowSize.y) * 0.5f), ">", rightColour);
			return;
		}

		if (m_Command->isSlider())
		{
			auto* slider = m_Command->as<Stand::CommandSlider>();
			const auto valueStr = slider->getValueText();
			const auto layout = ComputeSliderLayout(valueStr);

			const auto valueSize = GridRenderer::MeasureText(valueStr.c_str());
			GridRenderer::DrawText(layout.valueX + std::max(0.f, (layout.valueWidth - valueSize.x) * 0.5f),
			    y + std::max(0.f, (height - valueSize.y) * 0.5f),
			    valueStr.c_str(),
			    rightColour);

			const auto minusSize = GridRenderer::MeasureText("<");
			GridRenderer::DrawText(layout.minusX + std::max(0.f, (layout.buttonSize - minusSize.x) * 0.5f),
			    y + std::max(0.f, (height - minusSize.y) * 0.5f),
			    "<",
			    rightColour);

			const auto plusSize = GridRenderer::MeasureText(">");
			GridRenderer::DrawText(layout.plusX + std::max(0.f, (layout.buttonSize - plusSize.x) * 0.5f),
			    y + std::max(0.f, (height - plusSize.y) * 0.5f),
			    ">",
			    rightColour);
		}
	}

	void GridItemStandCommand::onClick(int16_t cursorX, int16_t)
	{
		if (!m_Command)
			return;

		if (m_Command->isSlider())
		{
			auto* slider = m_Command->as<Stand::CommandSlider>();
			const auto layout = ComputeSliderLayout(slider->getValueText());
			if (cursorX >= layout.plusX && cursorX < layout.plusX + layout.buttonSize)
				SliderStep(1);
			else if (cursorX >= layout.minusX && cursorX < layout.minusX + layout.buttonSize)
				SliderStep(-1);
			return;
		}

		activate();
	}

	void GridItemStandCommand::activate()
	{
		if (!m_Command)
			return;

		if (m_Command->isToggle())
			ToggleClicked();
		else if (m_Command->isList())
			OpenSubList();
		else if (auto* physical = m_Command->getPhysical())
			ButtonClicked(physical);
	}

	bool GridItemStandCommand::onArrow(int delta)
	{
		if (!m_Command || !m_Command->isSlider())
			return false;

		SliderStep(delta > 0 ? 1 : -1);
		return true;
	}

	void GridItemStandCommand::ToggleClicked()
	{
		auto* toggle = m_Command->as<Stand::CommandToggleNoCorrelation>();

		// A menu click, dispatched on a script thread since a toggle's
		// own onEnable()/onDisable() touches game state/natives, which
		// must never run directly on the render/input thread - same
		// "queue it, don't call it inline" convention
		// GridItemCommandButton.cpp's own onClick() already uses for
		// this project's own Command::Call().
		//
		// Deliberately NOT calling click.ensureResponse()/respond() here
		// (unlike MenuCommandConsole.cpp's own CLICK_COMMAND activation,
		// or CommandHotkeyDispatch.cpp's own CLICK_HOTKEY one) - a
		// previous pass here added it, then removed it again per explicit
		// request: this row's own checkbox already shows the new state
		// on screen the instant it flips, so a toast on top of that is
		// redundant noise specifically for a menu click. Typing into the
		// console (or a hotkey, which can fire with the menu closed
		// entirely) has no such visible feedback of its own, which is
		// exactly why those two still get one.
		FiberPool::queueJob([toggle] {
			Stand::Click click(Stand::CLICK_MENU, Stand::TC_SCRIPT_YIELDABLE);
			toggle->onClick(click);
		});
	}

	void GridItemStandCommand::SliderStep(int direction)
	{
		auto* slider = m_Command->as<Stand::CommandSlider>();

		FiberPool::queueJob([slider, direction] {
			Stand::Click click(Stand::CLICK_MENU, Stand::TC_SCRIPT_YIELDABLE);
			if (direction > 0)
				slider->onRight(click, false);
			else
				slider->onLeft(click, false);
		});
	}

	void GridItemStandCommand::ButtonClicked(Stand::CommandPhysical* physical)
	{
		// Same queue-it-not-call-it-inline convention as ToggleClicked()/
		// SliderStep() above - a plain action's own onClick() can touch
		// game natives just as freely as a toggle's onEnable()/onDisable().
		// No ensureResponse()/respond() here either - see ToggleClicked()'s
		// own comment for why a menu click doesn't get a toast.
		FiberPool::queueJob([physical] {
			Stand::Click click(Stand::CLICK_MENU, Stand::TC_SCRIPT_YIELDABLE);
			physical->onClick(click);
		});
	}

	void GridItemStandCommand::OpenSubList()
	{
		auto* list = m_Command->as<Stand::CommandList>();
		MenuNavigation::Push(Label(m_Command), &GridStandCommandList::GetOrCreate(list));
	}

	GridItemStandCommand::SliderLayout GridItemStandCommand::ComputeSliderLayout(const std::string& valueText) const
	{
		const float measuredWidth = GridRenderer::MeasureText(valueText.c_str()).x;
		SliderLayout layout;
		layout.buttonSize = kButtonSize;
		layout.valueWidth = std::max(kValueWidth, measuredWidth + kGap);
		layout.plusX = x + width - kButtonSize;
		layout.valueX = layout.plusX - kGap - layout.valueWidth;
		layout.minusX = layout.valueX - kGap - kButtonSize;
		return layout;
	}
}

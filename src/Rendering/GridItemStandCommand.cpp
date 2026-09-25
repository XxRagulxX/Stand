#include "Rendering/GridItemStandCommand.hpp"

#include "Commands/Stand/CommandToggleNoCorrelation.hpp"
#include "Commands/Vehicle/LSC/CommandVehicleColour.hpp"
#include "Commands/Widgets/CommandFlags.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Menu/Click.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/GridStandCommandList.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/ThemeIcons.hpp"
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

	bool GridItemStandCommand::isSectionHeader() const
	{
		return m_Command && (m_Command->flags & CMDFLAG_SECTION_HEADER) != 0;
	}

	bool GridItemStandCommand::isFocusable() const
	{
		return !isSectionHeader();
	}

	std::string GridItemStandCommand::GetDescription() const
	{
		if (!m_Command)
			return {};

		auto* physical = m_Command->getPhysical();
		if (!physical)
			return {};

		std::string result = physical->help_text.getLocalisedUtf8();

		if (auto syntax = physical->getCommandSyntax(); !syntax.empty())
		{
			if (!result.empty())
				result += '\n';
			result += syntax;
		}

		if (m_Command->isSlider() && Theme::kShowSliderBehaviour)
		{
			auto* slider = m_Command->as<Stand::CommandSlider>();
			if (!slider->command_names.empty())
			{
				result += '\n';
				result += "Click to input a value.";
			}
		}

		return result;
	}

	void GridItemStandCommand::draw()
	{
		if (isSectionHeader())
			return;

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
			uint32_t nav = Theme::kNavBarColour.load(std::memory_order_relaxed);
			if (nav & 0x01000000u)
			{
				float r = float(nav & 0xFF) / 255.f;
				float g = float((nav >> 8) & 0xFF) / 255.f;
				float b = float((nav >> 16) & 0xFF) / 255.f;
				GridRenderer::DrawRect(x, y, width, height, {r, g, b, 1.f});
			}
			else
			{
				GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);
			}
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

				const IconSlot iconSlot = isToggle
				    ? (m_Command->as<Stand::CommandToggleNoCorrelation>()->m_on ? IconSlot::Enabled : IconSlot::Disabled)
				    : IconSlot::List;

				if (compact)
				{
					GridRenderer::DrawRect(x, y, 4.f, height, texColour);
				}
				else if (ThemeIcons::IsLoaded(iconSlot))
				{
					ThemeIcons::QueueDraw(iconSlot, static_cast<float>(x), static_cast<float>(y), static_cast<float>(height), texColour);
				}
				else
				{
					const float texSize = kIndicatorSize;
					const float texY = y + std::max(0.f, (height - texSize) * 0.5f);
					GridRenderer::DrawRect(x + 2.f, texY, texSize, texSize, texColour);
				}
			}
		}

		if (!m_Command)
			return;

		const float iconSize   = static_cast<float>(height);
		const float iconX      = static_cast<float>(x + width) - iconSize;
		const float iconY      = static_cast<float>(y);
		const auto& spriteTint = isKeyboardFocused() ? Theme::kFocusTexture : Theme::kUnfocusedTexture;

		if (m_Command->isToggle())
		{
			auto* toggle = m_Command->as<Stand::CommandToggleNoCorrelation>();
			const IconSlot toggleSlot = toggle->m_on ? IconSlot::ToggleOn : IconSlot::ToggleOff;
			if (ThemeIcons::IsLoaded(toggleSlot))
			{
				ThemeIcons::QueueDraw(toggleSlot, iconX, iconY, iconSize, spriteTint);
			}
			else
			{
				GridRenderer::DrawRect(iconX, iconY, kIndicatorSize, kIndicatorSize, spriteTint);
				GridRenderer::DrawRect(iconX + kIndicatorBorderWidth,
				    iconY + kIndicatorBorderWidth,
				    kIndicatorSize - kIndicatorBorderWidth * 2.f,
				    kIndicatorSize - kIndicatorBorderWidth * 2.f,
				    toggle->m_on ? Theme::kAccent : Theme::kPanelBackground);
			}
		}
		else if (m_Command->isList())
		{
			auto listTint = spriteTint;
			if (auto* cl = dynamic_cast<Stand::CommandColourList*>(m_Command)) {
				uint32_t p = cl->m_listColour.load(std::memory_order_relaxed);
				if (p & 0x01000000u)
					listTint = { float(p & 0xFF) / 255.f, float((p >> 8) & 0xFF) / 255.f, float((p >> 16) & 0xFF) / 255.f, 1.f };
			}
			if (ThemeIcons::IsLoaded(IconSlot::List))
				ThemeIcons::QueueDraw(IconSlot::List, iconX, iconY, iconSize, listTint);
		}
		else if (m_Command->isLink())
		{
			if (ThemeIcons::IsLoaded(IconSlot::Link))
				ThemeIcons::QueueDraw(IconSlot::Link, iconX, iconY, iconSize, spriteTint);
		}
		else if (m_Command->flags & CMDFLAG_SEARCH_INPUT)
		{
			if (ThemeIcons::IsLoaded(IconSlot::Search))
				ThemeIcons::QueueDraw(IconSlot::Search, iconX, iconY, iconSize, spriteTint);
		}
		else if (m_Command->flags & CMDFLAG_TEXT_INPUT)
		{
			if (ThemeIcons::IsLoaded(IconSlot::Edit))
				ThemeIcons::QueueDraw(IconSlot::Edit, iconX, iconY, iconSize, spriteTint);
		}
	}

	void GridItemStandCommand::drawText()
	{
		using Rendering::GridRenderer;

		if (!m_Command)
		{
			GridRenderer::DrawText(x + 5.f, y, "Unknown!", Theme::kError);
			return;
		}

		const float textScale = Theme::kCommandTextScale;
		const auto label = Label(m_Command);
		const auto labelSize = GridRenderer::MeasureText(label.c_str(), textScale);

		if (isSectionHeader())
		{
			const float centreX = x + std::max(0.f, (width - labelSize.x) * 0.5f);
			const float centreY = y + Theme::kCommandTextYOffset + std::max(0.f, (height - labelSize.y) * 0.5f);
			GridRenderer::DrawText(centreX, centreY, label.c_str(), Theme::kUnfocusedRightText, textScale);
			return;
		}

		const bool focused = isKeyboardFocused();
		const auto& textColour = focused ? Theme::kFocusText : Theme::kUnfocusedText;
		const auto& rightColour = focused ? Theme::kFocusRightText : Theme::kUnfocusedRightText;

		GridRenderer::DrawText(x + Theme::kCommandTextXOffset, y + Theme::kCommandTextYOffset + std::max(0.f, (height - labelSize.y) * 0.5f), label.c_str(), textColour, textScale);

		if (m_Command->isList())
		{
			// Skip ">" text when the List icon is loaded — sprite in draw() replaces it.
			if (!ThemeIcons::IsLoaded(IconSlot::List))
			{
				auto arrowColour = rightColour;
				if (auto* cl = dynamic_cast<Stand::CommandColourList*>(m_Command)) {
					uint32_t p = cl->m_listColour.load(std::memory_order_relaxed);
					if (p & 0x01000000u)
						arrowColour = { float(p & 0xFF) / 255.f, float((p >> 8) & 0xFF) / 255.f, float((p >> 16) & 0xFF) / 255.f, 1.f };
				}
				const auto arrowSize = GridRenderer::MeasureText(">");
				GridRenderer::DrawText(x + width - arrowSize.x - kArrowGap, y + std::max(0.f, (height - arrowSize.y) * 0.5f), ">", arrowColour);
			}
			return;
		}

		if (m_Command->isSlider())
		{
			auto* slider = m_Command->as<Stand::CommandSlider>();
			const auto valueStr = slider->getValueText();

			if (focused)
			{
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
			else
			{
				// Unfocused: just show the value right-aligned, no arrows.
				const auto valueSize = GridRenderer::MeasureText(valueStr.c_str());
				GridRenderer::DrawText(
				    static_cast<float>(x + width) - valueSize.x - kArrowGap,
				    y + std::max(0.f, (height - valueSize.y) * 0.5f),
				    valueStr.c_str(),
				    rightColour);
			}
		}
	}

	void GridItemStandCommand::onClick(int16_t cursorX, int16_t)
	{
		if (!m_Command || isSectionHeader())
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
		if (!m_Command || isSectionHeader())
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

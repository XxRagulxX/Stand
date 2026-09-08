#pragma once
#include "Commands/Widgets/Command.hpp"
#include "Rendering/GridItem.hpp"

#include <cstdint>

namespace Stand::Rendering
{
	// Bridges one real Stand::Command (Commands/Widgets/ - the already-
	// adapted-but-previously-unwired port of real Stand's own
	// foundational Command/CommandIssuable/CommandPhysical/CommandList/
	// CommandToggle/CommandSlider tree, confirmed against
	// origin/stand-reference) into a row of this project's own Grid/
	// GridItem renderer, which real Stand has no equivalent of - Stand
	// routes every row through its own GridItemList walking this SAME
	// tree directly, a system this project replaced with Grid/GridItem
	// months ago (see GridRenderer.hpp's own class comment). This is the
	// one piece that was missing to make anything built on that tree
	// actually visible/interactive: without it, Commands/Widgets/ was
	// real, working logic with nothing to ever call it.
	//
	// One instance renders exactly one command, dispatching its own
	// draw()/drawText()/onClick()/activate()/onArrow() by
	// Stand::Command::isToggle()/isSlider()/isList() at construction
	// time - a runtime type switch rather than three separate GridItem
	// subclasses the way GridItemCommandToggle/GridItemCommandSlider
	// already are for this project's OWN command system, since a
	// Stand::CommandList's own children are a heterogeneous
	// std::vector<std::unique_ptr<Command>> populated generically by
	// GridStandCommandList::populate() (every other content Grid here
	// instead hand-writes one push_back() per row naming its own
	// concrete GridItem type - there's no tree to walk generically).
	//
	// Phase 1 scope, matching this project's own "port the foundation,
	// prove one real example, then migrate feature-by-feature" plan -
	// toggle, slider, list (folder navigation), and plain action
	// (COMMAND_ACTION - any physical command that's none of the above,
	// e.g. a one-shot button) only. Nothing else (CommandColourCustom,
	// CommandInput, ...) has been ported to this tree at all yet, so
	// there's nothing further to bridge until it has been.
	class GridItemStandCommand : public GridItem
	{
	public:
		GridItemStandCommand(int16_t width, int16_t height, Stand::Command* command);

		void draw() override;
		void drawText() override;
		void onClick(int16_t cursorX, int16_t cursorY) override;

		bool isFocusable() const override
		{
			return true;
		}

		void activate() override;
		bool onArrow(int delta) override;

		[[nodiscard]] std::string GetDescription() const override;

	private:
		void ToggleClicked();
		void SliderStep(int direction);
		void OpenSubList();
		void ButtonClicked(Stand::CommandPhysical* physical);

		struct SliderLayout
		{
			float valueX;
			float valueWidth;
			float minusX;
			float plusX;
			float buttonSize;
		};
		SliderLayout ComputeSliderLayout(const std::string& valueText) const;

		Stand::Command* m_Command;
	};
}

#pragma once
#include "Rendering/GridItem.hpp"

#include <cstdint>
#include <string>

namespace rage
{
	class scrProgram;
}

namespace Stand::Rendering
{
	// One 16-byte row of Debug > Scripts > Threads' bytecode viewer -
	// mirrors MenuScripts.cpp's RenderBytecode() row rendering, but as a
	// real top-level GridItem rather than an ImGuiListClipper-virtualized
	// inline loop: BytecodeGrid pushes one of these per 16 bytes of
	// program->GetFullCodeSize(), reusing this system's own existing
	// scroll/off-screen-clipping (Grid::draw()/drawText() already skip
	// anything scrolled above origin.y, and the GPU clips the rest) as
	// the virtualization mechanism instead of building a second one.
	//
	// Byte values are read live from `program` on every draw()/drawText()
	// call, never cached - the same "read fresh, no populate()-time
	// snapshot" approach as GridItemPlayerList/GridItemLiveText, just
	// applied to raw memory instead of a live game object.
	//
	// Up/Down (handled above this class, by the owning Grid's normal
	// keyboard navigation moving focus between rows) selects which row;
	// Left/Right moves m_SelectedCell between *this* row's own 16 (or
	// fewer, for the last row) byte cells - the standard "Up/Down between
	// items, Left/Right adjusts the focused item" split this whole menu
	// uses elsewhere (see GridItemIntStepper's own class comment for the
	// same pattern). Enter starts editing the selected cell's hex text
	// (typed via onChar, hex digits only, at most 2 - see onChar()'s own
	// comment), Enter again commits (parsed and written straight into
	// program's live memory, exactly like the original's own direct
	// `*byte = newVal` - "this will conflict with the ScriptPatches
	// class, but still useful to keep for quick testing", unchanged
	// here), Escape discards. While editing, GridRenderer routes every
	// keystroke (including Left/Right) to onChar()/onEditKey() instead of
	// onArrow()/activate() - see GridItem.hpp's own isEditingText() doc
	// comment - so onArrow() is simply never called mid-edit; no extra
	// guard needed.
	class GridItemHexRow : public GridItem
	{
	public:
		GridItemHexRow(int16_t width, int16_t height, rage::scrProgram* program, std::uint32_t rowOffset, std::uint32_t codeSize);

		void draw() override;
		void drawText() override;
		void onClick(int16_t cursorX, int16_t cursorY) override;

		bool isFocusable() const override
		{
			return true;
		}

		bool onArrow(int delta) override;
		void activate() override;

		bool isEditingText() const override
		{
			return m_Editing;
		}

		void onChar(wchar_t c) override;
		void onEditKey(unsigned int vkCode) override;

	private:
		struct Layout
		{
			float offsetLabelWidth;
			float cellWidth;
			float cellGap;
			float firstCellX;
		};
		Layout ComputeLayout() const;

		// 16, or fewer for the last row (codeSize isn't necessarily a
		// multiple of 16).
		int CellCount() const;

		void BeginEditing();
		void Commit();
		void Cancel();

		rage::scrProgram* m_Program;
		std::uint32_t m_RowOffset;
		std::uint32_t m_CodeSize;
		int m_SelectedCell = 0;
		bool m_Editing = false;
		std::string m_EditBuffer; // in-progress typed hex text, at most 2 chars
	};
}

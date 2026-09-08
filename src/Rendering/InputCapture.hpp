#pragma once

namespace Stand::Rendering
{
	// Global "is some on-screen text field currently capturing keyboard
	// input" flag - the DirectXTK-era equivalent of ImGui's own
	// GetIO().WantTextInput. GridItemTextInput flips this on while
	// actively editing and off again on commit/cancel/blur, so
	// GUI::IsUsingKeyboard() (hotkey suppression while typing) has a
	// source of truth without needing to know about GridItem internals
	// directly. GridItemSelectList/GridItemHexRow have their own
	// isEditingText() the same way, but don't set this flag - their
	// text-edit interception in GridRenderer::WndProcImpl is already
	// handled via MenuFocus before this flag would even be checked, so
	// nothing currently needs them to.
	class InputCapture
	{
	public:
		static bool IsTextInputActive()
		{
			return s_Active;
		}

		static void SetTextInputActive(bool active)
		{
			s_Active = active;
		}

	private:
		static inline bool s_Active = false;
	};
}

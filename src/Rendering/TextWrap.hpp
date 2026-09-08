#pragma once
#include <string>
#include <vector>

namespace Stand::Rendering
{
	// Greedy word-wrap against GridRenderer::MeasureText - DrawText/
	// DrawTextScreen have no built-in wrapping (unlike ImGui::
	// TextWrapped, which every pre-port ImGui overlay in this system
	// relied on), so this replaces it wherever one of them needs to show
	// a block of text that might not fit on one line (Notifications'
	// own message body, ChatDisplay's own messages, ...). Same
	// "simplest thing that still works" trade-off as everywhere else in
	// this system without a real text-layout engine: no hyphenation,
	// and a single word wider than maxWidth on its own just overflows
	// rather than being split mid-word. scale is whatever the caller
	// will actually draw the wrapped lines at - MeasureText needs it to
	// measure accurately.
	std::vector<std::string> WrapText(const std::string& text, float maxWidth, float scale);
}

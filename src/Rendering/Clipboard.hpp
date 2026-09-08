#pragma once
#include <string>

namespace Stand::Rendering::Clipboard
{
	// Win32 clipboard read/write, standing in for ImGui::GetClipboardText()/
	// SetClipboardText() (what the classic ImGui widgets use) - this
	// project's own Grid code goes straight to the platform API instead of
	// picking up a dependency on ImGui it's working to remove entirely
	// (see this repo's own phased ImGui-removal plan). Originally a
	// GridItem-local CopyToClipboard() in InfoGrid.cpp; hoisted out here
	// once StatEditorGrid needed to read the clipboard too.
	void SetText(const std::string& text);

	// Empty string if the clipboard couldn't be opened, has nothing on it,
	// or isn't plain text (CF_TEXT).
	std::string GetText();
}

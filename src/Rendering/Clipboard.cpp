#include "Rendering/Clipboard.hpp"

#include <cstring>
#include <windows.h>

namespace Stand::Rendering::Clipboard
{
	void SetText(const std::string& text)
	{
		if (!OpenClipboard(nullptr))
			return;

		EmptyClipboard();

		const auto size = text.size() + 1;
		if (HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, size))
		{
			if (void* dst = GlobalLock(mem))
			{
				std::memcpy(dst, text.c_str(), size);
				GlobalUnlock(mem);
				SetClipboardData(CF_TEXT, mem);
			}
			else
			{
				GlobalFree(mem);
			}
		}

		CloseClipboard();
	}

	std::string GetText()
	{
		if (!OpenClipboard(nullptr))
			return "";

		std::string result;
		if (HANDLE data = GetClipboardData(CF_TEXT))
		{
			if (const char* src = static_cast<const char*>(GlobalLock(data)))
			{
				result = src;
				GlobalUnlock(data);
			}
		}

		CloseClipboard();
		return result;
	}
}

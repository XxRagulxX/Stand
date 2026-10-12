#pragma once
#include "Commands/Widgets/CommandToggle.hpp"
#include "Core/Pointers.hpp"
#include "Menu/Click.hpp"

#include <windows.h>

namespace Stand
{
	// Ported from real Stand's own CommandStreamproof
	// (src/Commands/Online/CommandStreamproof.hpp in origin/dev).
	//
	// Real Stand hooks a second IDXGISwapChain::Present call so only the
	// menu overlay is excluded from capture. Our D3D12 renderer shares one
	// Present path, so we use SetWindowDisplayAffinity with
	// WDA_EXCLUDEFROMCAPTURE (Windows 10 Build 2004+) instead — this
	// excludes the entire game window from capture tools rather than only
	// the overlay, but is the correct non-legacy equivalent available in our
	// architecture. WDA_NONE restores normal capture on disable.
	class CommandStreamproof : public CommandToggle
	{
		// WDA_EXCLUDEFROMCAPTURE = 0x11, documented in Windows SDK 10.0.19041+
		static constexpr DWORD kWdaExcludeFromCapture = 0x00000011;
		static constexpr DWORD kWdaNone              = 0x00000000;

	public:
		explicit CommandStreamproof(CommandList* const parent)
			: CommandToggle(parent, LIT("Stream-Proof Rendering"), CMDNAMES("streamproof"),
				LIT("Prevents Stand being visible to streaming software, if it uses DirectX/game capture."),
				false)
		{
		}

		void onChange(Click& click) override
		{
			if (!Pointers.Hwnd || !*Pointers.Hwnd)
				return;

			const DWORD affinity = m_on ? kWdaExcludeFromCapture : kWdaNone;
			SetWindowDisplayAffinity(*Pointers.Hwnd, affinity);
		}
	};
}

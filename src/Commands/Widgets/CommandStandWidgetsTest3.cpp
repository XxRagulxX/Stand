#include "Commands/Widgets/CommandStandWidgetsTest3.hpp"
#include "Commands/Widgets/CommandToggleBitflag.hpp"

#include <cstdint>

namespace Stand::Features
{
	// Test-only command for CommandToggleBitflag (see Commands/Widgets/
	// CommandToggleBitflag.hpp) - reachable from Settings > Debug > "Stand
	// Widgets Test 3", a page of its own so none of this touches any
	// real feature. Once confirmed working in-game, this whole file
	// (and its test page) gets deleted, same as the two rounds before it.
	namespace
	{
		std::uint32_t g_TestFlags = 0;
		constexpr std::uint32_t kTestBit0 = 1u << 0;
		constexpr std::uint32_t kTestBit3 = 1u << 3;
	}

	// Two toggles sharing the same underlying uint32_t but different
	// bits - toggling one shouldn't affect the other, and both should
	// be independently readable back.
	static StandWidgets::CommandToggleBitflag<std::uint32_t> _TestBitflag0{"standwidgets_testbitflag0",
	    "Test: Bitflag (bit 0)",
	    "Confirms CommandToggleBitflag sets/clears only its own bit",
	    &g_TestFlags,
	    kTestBit0};

	static StandWidgets::CommandToggleBitflag<std::uint32_t> _TestBitflag3{"standwidgets_testbitflag3",
	    "Test: Bitflag (bit 3)",
	    "Confirms CommandToggleBitflag sets/clears only its own bit",
	    &g_TestFlags,
	    kTestBit3};

	std::uint32_t GetTestFlags()
	{
		return g_TestFlags;
	}
}

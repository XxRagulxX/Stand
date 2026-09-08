#pragma once
#include "Commands/CommandToggleLegacy.hpp"

#include <utility>

namespace Stand::StandWidgets
{
	// Ported from real Stand's own CommandToggleBitflag (the simpler of
	// its two bit-level toggles - the other, CommandToggleBitPointer,
	// uses a soup::BitPointer type this project doesn't vendor, so not
	// safe to port without a verified equivalent). Templated the same
	// way Stand's own version is: T is whatever integral type the
	// packed field actually is (int, uint32_t, ...) - flips a single bit
	// (or any mask) of *addr on/off, rather than owning a whole bool the
	// way CommandTogglePointer does. Built the same safe way as CommandTogglePointer/
	// CommandSliderPointer - CommandToggle's own existing OnEnable()/OnDisable()
	// hooks, no changes to CommandToggle.hpp needed.
	template<typename T>
	class CommandToggleBitflag : public CommandToggleLegacy
	{
	public:
		CommandToggleBitflag(std::string name, std::string label, std::string description, T* addr, T mask) :
		    CommandToggleLegacy(std::move(name), std::move(label), std::move(description), addr && (*addr & mask) != T{0}),
		    m_Addr(addr),
		    m_Mask(mask)
		{
		}

	protected:
		void OnEnable() override
		{
			if (m_Addr)
				*m_Addr |= m_Mask;
		}

		void OnDisable() override
		{
			if (m_Addr)
				*m_Addr &= static_cast<T>(~m_Mask);
		}

	private:
		T* m_Addr;
		T m_Mask;
	};
}

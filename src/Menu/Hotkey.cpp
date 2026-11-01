#include "Menu/Hotkey.hpp"

#include <algorithm>
#include <cctype>
#include <windows.h>

namespace Stand
{
	namespace
	{
		std::string GetKeyNameA(unsigned int vk)
		{
			struct Entry { unsigned int vk; const char* name; };
			static constexpr Entry map[] = {
				{ VK_F1,  "F1"  }, { VK_F2,  "F2"  }, { VK_F3,  "F3"  }, { VK_F4,  "F4"  },
				{ VK_F5,  "F5"  }, { VK_F6,  "F6"  }, { VK_F7,  "F7"  }, { VK_F8,  "F8"  },
				{ VK_F9,  "F9"  }, { VK_F10, "F10" }, { VK_F11, "F11" }, { VK_F12, "F12" },
				{ VK_F13, "F13" }, { VK_F14, "F14" }, { VK_F15, "F15" }, { VK_F16, "F16" },
				{ VK_F17, "F17" }, { VK_F18, "F18" }, { VK_F19, "F19" }, { VK_F20, "F20" },
				{ VK_F21, "F21" }, { VK_F22, "F22" }, { VK_F23, "F23" }, { VK_F24, "F24" },
				{ VK_NUMPAD0, "Numpad 0" }, { VK_NUMPAD1, "Numpad 1" }, { VK_NUMPAD2, "Numpad 2" },
				{ VK_NUMPAD3, "Numpad 3" }, { VK_NUMPAD4, "Numpad 4" }, { VK_NUMPAD5, "Numpad 5" },
				{ VK_NUMPAD6, "Numpad 6" }, { VK_NUMPAD7, "Numpad 7" }, { VK_NUMPAD8, "Numpad 8" },
				{ VK_NUMPAD9, "Numpad 9" },
				{ VK_ADD,      "Numpad +" }, { VK_SUBTRACT, "Numpad -" },
				{ VK_MULTIPLY, "Numpad *" }, { VK_DECIMAL,  "Numpad ." },
				{ VK_DIVIDE,   "Numpad /" },
				{ VK_SHIFT,    "Shift"       }, { VK_RSHIFT,   "Right Shift" }, { VK_LSHIFT,   "Left Shift"   },
				{ VK_CONTROL,  "Ctrl"        }, { VK_RCONTROL, "Right Ctrl"  }, { VK_LCONTROL, "Left Ctrl"    },
				{ VK_MENU,     "Alt"         }, { VK_RMENU,    "Right Alt"   }, { VK_LMENU,    "Left Alt"     },
				{ VK_RETURN,   "Enter"       }, { VK_BACK,     "Backspace"   }, { VK_ESCAPE,   "Escape"       },
				{ VK_SPACE,    "Space"       }, { VK_TAB,      "Tab"         }, { VK_CAPITAL,  "Caps Lock"    },
				{ VK_INSERT,   "Insert"      }, { VK_DELETE,   "Delete"      },
				{ VK_HOME,     "Home"        }, { VK_END,      "End"         },
				{ VK_PRIOR,    "Page Up"     }, { VK_NEXT,     "Page Down"   },
				{ VK_UP,       "Arrow Up"    }, { VK_DOWN,     "Arrow Down"  },
				{ VK_LEFT,     "Arrow Left"  }, { VK_RIGHT,    "Arrow Right" },
				{ VK_LBUTTON,  "Mouse Left"  }, { VK_RBUTTON,  "Mouse Right" }, { VK_MBUTTON, "Mouse Middle" },
				{ VK_XBUTTON1, "Mouse 4"     }, { VK_XBUTTON2, "Mouse 5"     },
				{ VK_MEDIA_PLAY_PAUSE, "Media Play/Pause" }, { VK_MEDIA_STOP, "Media Stop" },
				{ VK_MEDIA_NEXT_TRACK, "Next Track" }, { VK_MEDIA_PREV_TRACK, "Previous Track" },
				{ VK_OEM_MINUS, "/" }, { VK_OEM_PLUS, "+" }, { VK_OEM_PERIOD, "." }, { VK_OEM_COMMA, "," },
			};
			for (const auto& e : map)
				if (e.vk == vk)
					return e.name;
			if ((vk >= 'A' && vk <= 'Z') || (vk >= '0' && vk <= '9'))
				return std::string(1, static_cast<char>(vk));
			return std::to_string(vk);
		}

		void ToLower(std::string& str)
		{
			std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
				return static_cast<char>(std::tolower(c));
			});
		}

		bool IsNumberChar(char c)
		{
			return c >= '0' && c <= '9';
		}
	}

	Hotkey::Hotkey(std::string str) :
	    value(0)
	{
		ToLower(str);
		if (str.length() > 5 && str.substr(0, 5) == "hold ")
		{
			str.erase(0, 5);
			value |= FLAG_HOLDMODE;
		}
		if (str.length() > 5 && str.substr(0, 5) == "ctrl+")
		{
			str.erase(0, 5);
			value |= FLAG_CTRL;
		}
		if (str.length() > 6 && str.substr(0, 6) == "shift+")
		{
			str.erase(0, 6);
			value |= FLAG_SHIFT;
		}
		if (str.length() > 4 && str.substr(0, 4) == "alt+")
		{
			str.erase(0, 4);
			value |= FLAG_ALT;
		}
		if (str.length() == 1)
		{
			const char c = str.at(0);
			if (IsNumberChar(c))
			{
				value |= static_cast<uint64_t>(c);
				return;
			}
			if (c >= 'a' && c <= 'z')
			{
				value |= static_cast<uint64_t>(c + ('A' - 'a'));
				return;
			}
		}
		for (unsigned int vk = 1; vk < 255; ++vk)
		{
			auto name = GetKeyNameA(vk);
			ToLower(name);
			if (name == str)
			{
				value |= vk;
				return;
			}
		}
		try
		{
			value |= std::stoul(str);
		}
		catch (...)
		{
			value = 0;
		}
	}

	Hotkey Hotkey::fromPressing(unsigned int vk, bool repeat)
	{
		bool ctrl = false, shift = false, alt = false;
		if (vk != VK_CONTROL && vk != VK_LCONTROL && vk != VK_RCONTROL)
			ctrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
		if (vk != VK_SHIFT && vk != VK_LSHIFT && vk != VK_RSHIFT)
			shift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
		if (vk != VK_MENU && vk != VK_LMENU && vk != VK_RMENU)
			alt = (GetKeyState(VK_MENU) & 0x8000) != 0;
		return Hotkey(vk, ctrl, shift, alt, repeat);
	}

	void Hotkey::operator=(const Hotkey b)
	{
		value = b.value;
	}

	bool Hotkey::operator==(const Hotkey b) const
	{
		return getKeyValue() == b.getKeyValue();
	}

	bool Hotkey::operator!=(const Hotkey b) const
	{
		return !operator==(b);
	}

	Hotkey::operator bool() const
	{
		return value != 0;
	}

	void Hotkey::reset()
	{
		value = 0;
	}

	unsigned int Hotkey::vk() const
	{
		return static_cast<unsigned int>(value & 0xFFFFFFFFull);
	}

	bool Hotkey::hasModkeys() const
	{
		return (value >> 32) & 0b111;
	}

	bool Hotkey::hasModifiers() const
	{
		return (value >> 32) != 0;
	}

	bool Hotkey::ctrl() const
	{
		return (value & FLAG_CTRL) != 0;
	}

	bool Hotkey::shift() const
	{
		return (value & FLAG_SHIFT) != 0;
	}

	bool Hotkey::alt() const
	{
		return (value & FLAG_ALT) != 0;
	}

	uint64_t Hotkey::getKeyValue() const
	{
		return value & 0b11111111111111111111111111111111111ull;
	}

	bool Hotkey::isHoldMode() const
	{
		return (value & FLAG_HOLDMODE) != 0;
	}

	void Hotkey::setHoldMode(bool on) noexcept
	{
		if (on)
			enableHoldMode();
		else
			value &= ~FLAG_HOLDMODE;
	}

	void Hotkey::enableHoldMode() noexcept
	{
		value |= FLAG_HOLDMODE;
	}

	bool Hotkey::isRepeat() const
	{
		return (value & FLAG_REPEAT) != 0;
	}

	std::string Hotkey::toString() const
	{
		std::string str;
		if (isHoldMode())
			str.append("Hold ");
		if (ctrl())
			str.append("Ctrl+");
		if (shift())
			str.append("Shift+");
		if (alt())
			str.append("Alt+");
		return str.append(GetKeyNameA(vk()));
	}

	std::wstring Hotkey::toWString() const
	{
		const auto narrow = toString();
		return std::wstring(narrow.begin(), narrow.end());
	}

	CommandName Hotkey::toStringForCommandName() const
	{
#if COMPACT_COMMAND_NAMES
		return toString();
#else
		return toWString();
#endif
	}

	std::string Hotkey::toBracketedString() const
	{
		std::string str(1, '[');
		str.append(toString());
		str.push_back(']');
		return str;
	}

	std::string Hotkey::toFileString() const
	{
		std::string str;
		if (isHoldMode())
			str.append("Hold ");
		if (ctrl())
			str.append("Ctrl+");
		if (shift())
			str.append("Shift+");
		if (alt())
			str.append("Alt+");
		return str.append(GetKeyNameA(vk()));
	}

	bool Hotkey::isPressedAsync() const
	{
		const auto vkCode = vk();
		if (!(GetAsyncKeyState(vkCode) & 0x8000))
			return false;
		return fromPressing(vkCode).overlapsWith(*this);
	}

	float Hotkey::getNormal() const
	{
		const auto vkCode = vk();
		if (fromPressing(vkCode).overlapsWith(*this) && (GetAsyncKeyState(vkCode) & 0x8000))
			return 1.0f;
		return 0.0f;
	}

	bool Hotkey::overlapsWith(const Hotkey b) const
	{
		if (vk() != b.vk())
			return false;

		HotkeyModkeyBehaviour current_modkey_behaviour = Hotkey::modkey_behaviour;
		if (current_modkey_behaviour == HMB_STRICT_IF_HAS_MODKEY)
			current_modkey_behaviour = b.hasModkeys() ? HMB_STRICT : HMB_LAX;

		if (current_modkey_behaviour == HMB_STRICT)
			return ctrl() == b.ctrl() && shift() == b.shift() && alt() == b.alt();

		if (b.ctrl() && !ctrl())
			return false;
		if (b.shift() && !shift())
			return false;
		if (b.alt() && !alt())
			return false;
		return true;
	}

	const Hotkey* Hotkey::getOverlap(const std::vector<Hotkey>& hotkeys) const
	{
		for (const auto& hotkey : hotkeys)
		{
			if (overlapsWith(hotkey))
				return &hotkey;
		}
		return nullptr;
	}

	bool Hotkey::overlapsWith(const std::vector<Hotkey>& hotkeys) const
	{
		for (const auto& hotkey : hotkeys)
		{
			if (overlapsWith(hotkey))
				return true;
		}
		return false;
	}

	bool Hotkey::overlapsWithIncludeHoldMode(const Hotkey b) const
	{
		return overlapsWith(b) && isHoldMode() == b.isHoldMode();
	}

	bool Hotkey::overlapsWithIncludeHoldMode(const std::vector<Hotkey>& hotkeys) const
	{
		for (const auto& hotkey : hotkeys)
		{
			if (overlapsWithIncludeHoldMode(hotkey))
				return true;
		}
		return false;
	}

	bool Hotkey::isAnyVkEqual(const std::vector<Hotkey>& hotkeys) const
	{
		for (const auto& hotkey : hotkeys)
		{
			if (vk() == hotkey.vk())
				return true;
		}
		return false;
	}

	bool Hotkey::isNumpadKey() const
	{
		switch (vk())
		{
		case VK_NUMPAD0:
		case VK_NUMPAD1:
		case VK_NUMPAD2:
		case VK_NUMPAD3:
		case VK_NUMPAD4:
		case VK_NUMPAD5:
		case VK_NUMPAD6:
		case VK_NUMPAD7:
		case VK_NUMPAD8:
		case VK_NUMPAD9:
		case VK_ADD:
		case VK_SUBTRACT:
		case VK_MULTIPLY:
		case VK_DIVIDE:
			return true;
		}
		return false;
	}
}

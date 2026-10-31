#pragma once

#include "Input/ControllerInputConfig.hpp"
#include "Menu/Hotkey.hpp"
#include "Util/hashtype.hpp"

#include <vector>

namespace Stand
{
	struct InputScheme
	{
		std::vector<Hotkey> key_menu_open_close;
		std::vector<Hotkey> key_menu_root_up;
		std::vector<Hotkey> key_menu_root_down;
		std::vector<Hotkey> key_menu_up;
		std::vector<Hotkey> key_menu_down;
		std::vector<Hotkey> key_menu_left;
		std::vector<Hotkey> key_menu_right;
		std::vector<Hotkey> key_menu_click;
		std::vector<Hotkey> key_menu_back;
		std::vector<Hotkey> key_menu_context;
		std::vector<Hotkey> key_command_box;

		[[nodiscard]] bool isDefault() const noexcept { return true; }
	};

	struct Input
	{
		inline static bool keyboard_and_mouse = false;
		inline static bool user_has_numpad = false;
		inline static InputScheme scheme{};

		[[nodiscard]] static uhash_t getSchemeHash() noexcept { return 0; }
		[[nodiscard]] static bool isController() noexcept { return false; }
		[[nodiscard]] static bool isFreeSprintAvailable() noexcept { return false; }
		[[nodiscard]] static bool isAscDescAvailable() noexcept { return false; }

		[[nodiscard]] static ControlInput getContext() noexcept { return INPUT_CONTEXT; }
		[[nodiscard]] static ControlInput getFreeSprint() noexcept { return INPUT_SPRINT; }
		[[nodiscard]] static ControlInput getAscend() noexcept { return INPUT_SCRIPTED_FLY_ZUP; }
		[[nodiscard]] static ControlInput getDescend() noexcept { return INPUT_SCRIPTED_FLY_ZDOWN; }

		[[nodiscard]] static Hotkey getPreferedHotkey(const std::vector<Hotkey>&) noexcept { return {}; }
		[[nodiscard]] static std::string vk_to_string(int) { return {}; }

		static void addToScaleform(ControlInput) {}
		static void addToScaleform(const std::vector<Hotkey>&, ControlInput) {}
	};
}

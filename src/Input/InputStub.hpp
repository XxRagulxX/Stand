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

		InputScheme() = default;

		explicit InputScheme(
			std::vector<Hotkey> key_menu_open_close,
			std::vector<Hotkey> key_menu_root_up,
			std::vector<Hotkey> key_menu_root_down,
			std::vector<Hotkey> key_menu_up,
			std::vector<Hotkey> key_menu_down,
			std::vector<Hotkey> key_menu_left,
			std::vector<Hotkey> key_menu_right,
			std::vector<Hotkey> key_menu_click,
			std::vector<Hotkey> key_menu_back,
			std::vector<Hotkey> key_menu_context,
			std::vector<Hotkey> key_command_box
		)
			: key_menu_open_close(std::move(key_menu_open_close)),
			  key_menu_root_up(std::move(key_menu_root_up)),
			  key_menu_root_down(std::move(key_menu_root_down)),
			  key_menu_up(std::move(key_menu_up)),
			  key_menu_down(std::move(key_menu_down)),
			  key_menu_left(std::move(key_menu_left)),
			  key_menu_right(std::move(key_menu_right)),
			  key_menu_click(std::move(key_menu_click)),
			  key_menu_back(std::move(key_menu_back)),
			  key_menu_context(std::move(key_menu_context)),
			  key_command_box(std::move(key_command_box))
		{
		}

		[[nodiscard]] bool isDefault() const noexcept { return true; }
	};

	struct InputSchemeNoNumpad : public InputScheme
	{
		explicit InputSchemeNoNumpad()
			: InputScheme(
				{ VK_F4 },
				{ VK_RSHIFT },
				{ VK_RCONTROL },
				{ VK_UP },
				{ VK_DOWN },
				{ VK_LEFT },
				{ VK_RIGHT },
				{ VK_RETURN },
				{ VK_BACK },
				{ 'O' },
				{ 'U' }
			)
		{
		}
	};

	struct InputSchemeNumpad : public InputScheme
	{
		explicit InputSchemeNumpad()
			: InputScheme(
				{ VK_ADD },
				{ VK_NUMPAD7 },
				{ VK_NUMPAD1 },
				{ VK_NUMPAD8 },
				{ VK_NUMPAD2 },
				{ VK_NUMPAD4 },
				{ VK_NUMPAD6 },
				{ VK_NUMPAD5 },
				{ VK_NUMPAD0 },
				{ VK_NUMPAD3 },
				{ 'U' }
			)
		{
		}
	};

	struct InputSchemeFlexible : public InputScheme
	{
		explicit InputSchemeFlexible()
			: InputScheme(
				{ VK_F4, VK_ADD },
				{ VK_RSHIFT, VK_NUMPAD7 },
				{ VK_RCONTROL, VK_NUMPAD1 },
				{ VK_UP, VK_NUMPAD8 },
				{ VK_DOWN, VK_NUMPAD2 },
				{ VK_LEFT, VK_NUMPAD4 },
				{ VK_RIGHT, VK_NUMPAD6 },
				{ VK_RETURN, VK_NUMPAD5 },
				{ VK_BACK, VK_NUMPAD0 },
				{ 'O', VK_NUMPAD3 },
				{ 'U' }
			)
		{
		}
	};

	struct Input
	{
		inline static bool keyboard_and_mouse = false;
		inline static bool user_has_numpad = false;
		inline static InputScheme scheme = InputSchemeFlexible{};

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

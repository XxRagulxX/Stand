#pragma once

#include <string>
#include <vector>

#include <Windows.h>

#include "Game/ControllerInputs.hpp"
#include "Menu/Hotkey.hpp"
#include "Scripting/Natives.hpp"
#include "Util/hashtype.hpp"

namespace Stand
{
    struct InputScheme
    {
        std::vector<Hotkey> key_menu_open_close  { VK_F4, VK_ADD };
        std::vector<Hotkey> key_menu_root_up     { VK_RSHIFT, VK_NUMPAD7 };
        std::vector<Hotkey> key_menu_root_down   { VK_RCONTROL, VK_NUMPAD1 };
        std::vector<Hotkey> key_menu_up          { VK_UP, VK_NUMPAD8 };
        std::vector<Hotkey> key_menu_down        { VK_DOWN, VK_NUMPAD2 };
        std::vector<Hotkey> key_menu_left        { VK_LEFT, VK_NUMPAD4 };
        std::vector<Hotkey> key_menu_right       { VK_RIGHT, VK_NUMPAD6 };
        std::vector<Hotkey> key_menu_click       { VK_RETURN, VK_NUMPAD5 };
        std::vector<Hotkey> key_menu_back        { VK_BACK, VK_NUMPAD0 };
        std::vector<Hotkey> key_menu_context     { 'O', VK_NUMPAD3 };
        std::vector<Hotkey> key_command_box      { 'U' };

        [[nodiscard]] bool isDefault() const;
        [[nodiscard]] std::vector<Hotkey> getAll() const;
        [[nodiscard]] bool conflictsWithGameplay() const;
        [[nodiscard]] bool conflictsWith(Hotkey hotkey) const;
        [[nodiscard]] bool conflictsWith(const std::vector<Hotkey>& hotkeys) const;
    };

    using ControlInput = ControllerInputs;

    struct Input
    {
        inline static bool keyboard_and_mouse = false;
        inline static bool user_has_numpad = true;
        inline static InputScheme scheme{};

        [[nodiscard]] static Hotkey getPreferedHotkey(const std::vector<Hotkey>& hotkeys);

        [[nodiscard]] static std::string vk_to_string_no_brackets(unsigned int vk);
        [[nodiscard]] static std::string vk_to_string(unsigned int vk);
        [[nodiscard]] static std::string vk_to_string(std::string&& prefix, unsigned int vk, std::string&& suffix);
        [[nodiscard]] static std::string vk_to_file_string(unsigned int vk);

        [[nodiscard]] static unsigned int ControlInput_to_VirtualKey(ControlInput control_input);
        [[nodiscard]] static bool doesSchemeConflictWithInput(ControlInput control_input);

        [[nodiscard]] static uhash_t getSchemeHash();

        [[nodiscard]] static bool isController() noexcept;
        [[nodiscard]] static bool isMenuClosedOrControllerUnused() noexcept;
        [[nodiscard]] static bool isContextAvailable() noexcept;
        [[nodiscard]] static bool isFreeSprintAvailable() noexcept;
        [[nodiscard]] static bool isAscDescAvailable() noexcept;

        [[nodiscard]] static ControlInput getContext() noexcept;
        [[nodiscard]] static ControlInput getFreeSprint() noexcept;
        [[nodiscard]] static ControlInput getAscend() noexcept;
        [[nodiscard]] static ControlInput getDescend() noexcept;

        static void addToScaleform(ControlInput control_input);
        static void addToScaleform(const std::vector<Hotkey>& hotkeys, ControlInput controller_input);

        [[nodiscard]] static bool isCommandInputAllowed();
        [[nodiscard]] static bool isPressingAim();
        [[nodiscard]] static bool isPressingAttack();
        [[nodiscard]] static float getControlNormal(int control_input);
        [[nodiscard]] static float getControlNormal(int positive, int negative);
        [[nodiscard]] static bool isControlPressed(int control_input);
        [[nodiscard]] static bool isControlJustPressed(int control_input);
        [[nodiscard]] static bool isContextJustPressed();
        [[nodiscard]] static bool isContextJustPressed(int input_for_controller);
        [[nodiscard]] static bool isAnyKeyPressed(const std::vector<Hotkey>& hotkeys, int control_input);
        [[nodiscard]] static float getHighestNormal(const std::vector<Hotkey>& hotkeys, int control_input);
        [[nodiscard]] static bool canMovementCommandPerformMovement();
    };
}

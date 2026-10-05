#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include "Core/RecursiveScopedSpinlock.hpp"
#include "Core/ThreadContext.hpp"
#include "Game/typedecl.hpp"

namespace Stand
{
    class CommandList;

    enum class MouseMode : uint8_t
    {
        NONE,
        NAVIGATE,
    };

    enum InputType : uint8_t
    {
        INPUTTYPE_INDIFFERENT,
        INPUTTYPE_MOUSE_MOVE,
        INPUTTYPE_MOUSE_SCROLLWHEEL,
        INPUTTYPE_MOUSE_CLICK,
    };

    struct WarningData
    {
        uint64_t hash;
        std::wstring text;
        time_t can_proceed_after;
        std::function<void(ThreadContext)> proceed_callback;
        std::function<void(ThreadContext)> cancel_callback;
    };

    class Gui
    {
    public:
        std::unique_ptr<CommandList> root_list{};
        uint8_t root_cursor = 0;

        RecursiveScopedSpinlock root_mtx{};

        cursor_t command_rows = 11;
        int16_t command_columns = 1;

        cursor_t lerp = 70;
        time_t lerp_start = 0;
        cursor_t lerp_from_cursor = 0;
        cursor_t lerp_from_offset = 0;

        std::unordered_map<int8_t, WarningData> warnings{};

        MouseMode mouse_mode = MouseMode::NONE;
        InputType last_input_type = INPUTTYPE_INDIFFERENT;
        bool hotkeys_disabled = false;

        std::vector<CommandList*> m_active_list{};

        [[nodiscard]] CommandList* getCurrentUiList() const noexcept;

        [[nodiscard]] cursor_t getCommandsOnScreenLimit() const noexcept
        {
            return command_rows * command_columns;
        }

        [[nodiscard]] bool isRootStateFull() const noexcept { return true; }
        [[nodiscard]] bool isUnloadPending() const noexcept { return false; }
        [[nodiscard]] bool isRootUpdatePendingOrInProgress() const noexcept { return false; }
        [[nodiscard]] bool isInBadBoyTimeout() const noexcept { return false; }
        [[nodiscard]] bool isAwaitingSetHotkeyInput() const noexcept { return false; }
        [[nodiscard]] bool isPromptActive() const noexcept { return false; }
    };

    extern Gui g_gui;
}

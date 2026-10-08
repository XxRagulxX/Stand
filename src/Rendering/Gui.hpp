#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <soup/WeakRef.hpp>

#include "Commands/Widgets/CommandIssuable.hpp"
#include "Core/RecursiveScopedSpinlock.hpp"
#include "Core/ThreadContext.hpp"
#include "Game/typedecl.hpp"
#include "Rendering/StandPort/Direction.hpp"

namespace Stand
{
    class CommandList;
    class CommandToggle;

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
        std::vector<CommandToggle*> commands_with_correlation{};
        std::unordered_map<std::string, std::string> starred_commands;

        [[nodiscard]] CommandList* getCurrentUiList() const noexcept;

        void processToggleCorrelation(ThreadContext thread_context, ToggleCorrelation_t correlation, bool value);

        [[nodiscard]] cursor_t getCommandsOnScreenLimit() const noexcept
        {
            return command_rows * command_columns;
        }

        struct ActiveProfile
        {
            [[nodiscard]] bool isInitialised() const noexcept { return true; }
        } active_profile;

        [[nodiscard]] bool isUsingAutosaveState() const noexcept { return false; }

        [[nodiscard]] bool isRootStateFull() const noexcept { return true; }
        [[nodiscard]] bool isUnloadPending() const noexcept { return false; }
        [[nodiscard]] bool isRootUpdatePendingOrInProgress() const noexcept { return false; }
        [[nodiscard]] bool isInBadBoyTimeout() const noexcept { return false; }
        [[nodiscard]] bool isAwaitingSetHotkeyInput() const noexcept { return false; }
		bool inputUp(ThreadContext thread_context, const bool holding = false) { return false; }
		bool inputDown(ThreadContext thread_context, const bool holding = false) { return false; }
        [[nodiscard]] bool isPromptActive() const noexcept { return false; }

        bool opened = false;
        bool players_discovered = false;
        bool show_syntax = true;
        bool user_understands_navigation = false;

        Direction tabs_pos = LEFT;
        int16_t tabs_width = 112;

        void inputStop() {}
        void updateTabsIgnoreContextMenu() {}
        void saveTutorialFlags() {}
        void setProfilesTutorialDone() {}
        [[nodiscard]] bool isInteractionMenuOpen() const noexcept { return false; }

        float character_scale_multiplier = 1.0f;

        [[nodiscard]] static bool parseCommand(std::wstring& command, std::wstring& args);
        [[nodiscard]] std::vector<CommandIssuable*> findCommandsWhereCommandNameStartsWith(const CommandName& command_name_prefix, CommandPerm perms = COMMANDPERM_ALL) const;
        [[nodiscard]] std::vector<soup::WeakRef<CommandIssuable>> findCommandsWhereCommandNameStartsWithAsWeakrefs(const CommandName& command_name_prefix, CommandPerm perms = COMMANDPERM_ALL) const;
    };

    extern Gui g_gui;
}

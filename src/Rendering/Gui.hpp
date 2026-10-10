#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <soup/WeakRef.hpp>
#include "Core/RecursiveScopedSpinlock.hpp"
#include "Core/ThreadContext.hpp"
#include "Game/typedecl.hpp"
#include "Rendering/StandPort/Direction.hpp"

namespace Stand
{
    class Command;
    class CommandIssuable;
    class CommandList;
    class CommandPhysical;
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
        bool user_understands_context_menu = false;

        std::vector<CommandList*> m_active_list{};
        size_t m_preOpenNavDepth = 0;
        std::vector<CommandToggle*> commands_with_correlation{};

        struct StarredCommands
        {
            std::unordered_map<std::string, std::string> data{};
            void save() {}
        } starred_commands;

        struct HotkeysStore
        {
            std::unordered_map<std::string, std::vector<int>> data{};
            void save() {}
        } hotkeys;

        [[nodiscard]] CommandList* getCurrentUiList() const noexcept;
        [[nodiscard]] Command* getCurrentMenuFocus() const noexcept;
        [[nodiscard]] CommandPhysical* getCurrentMenuFocusPhysical() const noexcept;

        void updateFocus(ThreadContext thread_context, Direction momentum);
        void updateActiveFocus(ThreadContext thread_context, Direction momentum, Command* prev_focus);

        bool show_syntax = false;
        bool opened = false;
        bool user_understands_navigation = false;

        void sfxOpenClose(ThreadContext thread_context, bool opened) {}
        void addHotkeyToFocusedCommand() {}
        void changeHotkeyOnFocusedCommand() {}
        void saveTutorialFlags() {}
        void inputStop() {}
        void updateTabsIgnoreContextMenu() {}
        void setProfilesTutorialDone() {}

        [[nodiscard]] bool isInteractionMenuOpen() const noexcept { return false; }
        [[nodiscard]] static bool parseCommand(std::wstring& command_name, std::wstring& args) { return false; }
        [[nodiscard]] std::vector<soup::WeakRef<CommandIssuable>> findCommandsWhereCommandNameStartsWithAsWeakrefs(const std::wstring&) const { return {}; }
        [[nodiscard]] std::vector<soup::WeakRef<CommandIssuable>> findCommandsWhereCommandNameStartsWithAsWeakrefs(const std::string&) const { return {}; }
        [[nodiscard]] std::string getActiveStateNameUtf8() const { return {}; }
        [[nodiscard]] CommandList* getStandTab() const { return nullptr; }
        void loadStateToMemory(auto&) {}

        void processToggleCorrelation(ThreadContext thread_context, ToggleCorrelation_t correlation, bool value);

        [[nodiscard]] cursor_t getCommandsOnScreenLimit() const noexcept
        {
            return command_rows * command_columns;
        }

        struct ActiveProfile
        {
            std::unordered_map<std::string, std::string> data{};
            [[nodiscard]] bool isInitialised() const noexcept { return true; }
            void save() {}
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
    };

    extern Gui g_gui;
}

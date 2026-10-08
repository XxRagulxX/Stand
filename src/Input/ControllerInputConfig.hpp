#pragma once

#include <string>

#include "Game/ControllerInputs.hpp"

namespace Stand
{
    using ControlInput = ControllerInputs;

    struct ControllerInputConfig
    {
        inline static ControlInput menu_open_close_1 = ControlInput::INPUT_FRONTEND_RB;
        inline static ControlInput menu_open_close_2 = ControlInput::INPUT_FRONTEND_RIGHT;
        inline static ControlInput menu_root_up      = ControlInput::INPUT_FRONTEND_LT;
        inline static ControlInput menu_root_down    = ControlInput::INPUT_FRONTEND_RT;
        inline static ControlInput menu_up           = ControlInput::INPUT_FRONTEND_UP;
        inline static ControlInput menu_down         = ControlInput::INPUT_FRONTEND_DOWN;
        inline static ControlInput menu_left         = ControlInput::INPUT_FRONTEND_LEFT;
        inline static ControlInput menu_right        = ControlInput::INPUT_FRONTEND_RIGHT;
        inline static ControlInput menu_click        = ControlInput::INPUT_FRONTEND_ACCEPT;
        inline static ControlInput menu_back         = ControlInput::INPUT_FRONTEND_CANCEL;
        inline static ControlInput menu_context      = ControlInput::INPUT_FRONTEND_X;
        inline static ControlInput command_box       = ControlInput::INPUT_FRONTEND_Y;

        [[nodiscard]] static std::string getOpenCloseStringForUser() { return "[RB]"; }
        [[nodiscard]] static std::string getStringForUser(ControlInput) { return "[?]"; }
        [[nodiscard]] static std::string getStringForGame(ControlInput) { return "[?]"; }
        [[nodiscard]] static std::string getLocalisedString(ControlInput) { return "[?]"; }
    };
}

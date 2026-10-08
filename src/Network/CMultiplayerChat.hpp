#pragma once

#include <string>

namespace Stand
{
    struct CMultiplayerChat
    {
        std::wstring message;
        uint32_t message_length = 0;
    };
}

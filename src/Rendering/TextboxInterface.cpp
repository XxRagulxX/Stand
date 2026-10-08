#include "Rendering/TextboxInterface.hpp"

#include <soup/base.hpp>

#include "Network/CMultiplayerChat.hpp"
#include "Util/pointers.hpp"

namespace Stand
{
    void ChatboxInterface::add(const std::wstring&) const
    {
    }

    void ChatboxInterface::remove(size_t) const
    {
    }

    std::wstring ChatboxInterface::getText() const noexcept
    {
        return (*pointers::chat_box)->message;
    }

    size_t ChatboxInterface::getLength() const noexcept
    {
        return (*pointers::chat_box)->message_length;
    }
}

#include "Rendering/Commandbox.hpp"

namespace Stand
{
    void CommandboxInterface::add(const std::wstring& text) const
    {
        g_commandbox.appendText(text);
    }

    void CommandboxInterface::remove(size_t len) const
    {
        g_commandbox.eraseTextBack(len);
    }

    std::wstring CommandboxInterface::getText() const noexcept
    {
        return g_commandbox.text;
    }

    size_t CommandboxInterface::getLength() const noexcept
    {
        return g_commandbox.text.length();
    }
}

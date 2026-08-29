#include "Menu/TextboxInterface.hpp"

#include "lib/soup/base.hpp"

#include "Network/Chat.hpp"
#include "Network/CMultiplayerChat.hpp"
#include "Game/pointers.hpp"

namespace Stand
{
	void ChatboxInterface::add(const std::wstring& text) const
	{
		Chat::addToDraft(text);
	}

	void ChatboxInterface::remove(size_t len) const
	{
		SOUP_ASSERT(len <= 0xFFFF);
		Chat::removeFromDraft((uint16_t)len);
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

#include "Commands/Widgets/Commandbox.hpp"

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

#include "CommandReadonlyValue.hpp"

#include "Exceptional.hpp"
#include "RelayCon.hpp"
#include "StringUtils.hpp"

namespace Stand
{
	CommandReadonlyValue::CommandReadonlyValue(CommandList* parent, Label&& menu_name, Label&& help_text, commandflags_t flags, std::wstring&& value)
		: CommandPhysical(COMMAND_READONLY_VALUE, parent, std::move(menu_name), {}, std::move(help_text), flags), value(std::move(value))
	{
	}

	void CommandReadonlyValue::setValue(const std::string& value)
	{
		setValue(StringUtils::utf8_to_utf16(value));
	}
	
	void CommandReadonlyValue::setValue(std::wstring&& value)
	{
		if (this->value != value)
		{
			this->value = std::move(value);
			processVisualUpdate(false);
			updateWebState();
		}
	}

	// Kept out of the lambda below, which holds EXCEPTIONAL_LOCK's __try: the
	// e_line/s_line std::string objects can't be built or destroyed in a frame
	// that also has a __try, so this only takes references to already-built ones.
	static void sendReadonlyValueLines(std::string& e_line, std::string& s_line)
	{
		EXCEPTIONAL_LOCK(g_relay.send_mtx)
		g_relay.sendLine(std::move(e_line));
		g_relay.sendLine(std::move(s_line));
		EXCEPTIONAL_UNLOCK(g_relay.send_mtx)
	}

	void CommandReadonlyValue::updateWebState() const
	{
		if (isActiveOnWeb())
		{
			Exceptional::createManagedExceptionalThread(__FUNCTION__, [this]
			{
				auto e_line = std::string("e ").append(menu_name.getWebString());
				auto s_line = std::string("s ").append(StringUtils::utf16_to_utf8(value));
				sendReadonlyValueLines(e_line, s_line);
			});
		}
	}
}

#include "Rendering/GridItemCommandInput.hpp"

#include "Commands/Commands.hpp"
#include "Commands/CommandInput.hpp"
#include "Rendering/Notifications.hpp"

#include <functional>
#include <string>

namespace Stand::Rendering
{
	namespace
	{
		// Looked up twice (here and again inside the commit callback
		// below) rather than cached as a member - GridItemTextInput
		// itself has no notion of a Command at all (kept generic, so it
		// doesn't need to know CommandInput exists), so there's nowhere
		// on the base class to stash a resolved pointer for reuse.
		std::string ResolveLabel(joaat_t id, const std::optional<std::string>& labelOverride)
		{
			if (labelOverride.has_value())
				return *labelOverride;

			if (auto* command = Commands::GetCommand<CommandInput>(id))
				return command->GetLabel();

			return "Unknown!";
		}

		std::string ResolveInitialValue(joaat_t id)
		{
			if (auto* command = Commands::GetCommand<CommandInput>(id))
				return command->GetString();

			return "";
		}
	}

	GridItemCommandInput::GridItemCommandInput(int16_t width, int16_t height, joaat_t id, std::optional<std::string> labelOverride, bool scrolling, std::optional<std::size_t> maxLength) :
	    GridItemTextInput(
	        width, height, ResolveLabel(id, labelOverride), ResolveInitialValue(id), [id](const std::string& value) {
		        if (auto* command = Commands::GetCommand<CommandInput>(id))
			        command->SetStringValue(value);
	        },
	        nullptr, scrolling, maxLength.value_or(255), maxLength.has_value() ? std::function<void()>([maxLength] {
		        Notifications::Show("Input", "Limited to " + std::to_string(*maxLength) + " characters.", NotificationType::Warning);
	        }) :
	                                                                             nullptr),
	    m_Id(id)
	{
	}

	std::string GridItemCommandInput::GetDescription() const
	{
		auto* command = Commands::GetCommand<CommandInput>(m_Id);
		return command ? command->GetDescription() : std::string{};
	}
}

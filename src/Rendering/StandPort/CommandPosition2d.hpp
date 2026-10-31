#pragma once

#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPosition2dMouse.hpp"
#include "Rendering/StandPort/Position2d.hpp"

namespace Stand
{
	class CommandSlider;

	class CommandPosition2d : public CommandList
	{
		friend class CommandPosition2dMouse;
	protected:
		CommandSlider* x;
		CommandSlider* y;
		CommandPosition2dMouse* mouse = nullptr;

	public:
		explicit CommandPosition2d(CommandList* parent, Label&& menu_name, const CommandName& command_name, const Rendering::Position2d& default_pos, Label&& help_text = NOLABEL);

		[[nodiscard]] Rendering::Position2d getPosition() const;
		void onBack(ThreadContext thread_context) override;

	protected:
		virtual void onChange(Rendering::Position2d&& pos) const = 0;
	};
}

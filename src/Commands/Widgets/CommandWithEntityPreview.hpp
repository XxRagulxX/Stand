#pragma once

#include "Commands/Widgets/CommandWithOnTickFocused.hpp"

#include "Core/ThreadContext.hpp"
#include "Core/types.hpp"

namespace Stand
{
	template <typename T>
	class CommandWithEntityPreview : public CommandWithOnTickFocused<T>
	{
	protected:
		using CommandWithOnTickFocused<T>::CommandWithOnTickFocused;

	public:
		~CommandWithEntityPreview() override
		{
			getRidOfPreview();
		}

		void onTickFocused() override
		{
		}

		void onBlur(ThreadContext thread_context, Direction momentum) override
		{
			CommandWithOnTickFocused<T>::onBlur(thread_context, momentum);
			getRidOfPreview();
		}

	protected:
		[[nodiscard]] virtual bool isPreviewEnabled() const noexcept
		{
			return false;
		}

		[[nodiscard]] virtual Hash getModelHash() const = 0;

	protected:
		void getRidOfPreview()
		{
		}
	};
}

#pragma once

namespace Stand
{
#pragma pack(push, 1)
	template <class T>
	class CommandWithOnTickFocused : public T
	{
	protected:
		bool focused = false;

		using T::T;

	public:
		void onFocus() override
		{
			T::onFocus();

			focused = true;
		}

		void onBlur() override
		{
			T::onBlur();

			focused = false;
		}

		void onTickInGameViewport() override
		{
			if (focused)
			{
				onTickFocused();
			}

			return T::onTickInGameViewport();
		}

	protected:
		virtual void onTickFocused() = 0;
	};
#pragma pack(pop)
}

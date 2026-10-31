#pragma once

namespace Stand
{
#pragma pack(push, 1)
	template <typename T>
	class CommandListWithOnTickAsActiveList : public T
	{
	protected:
		bool is_active = false;

		using T::T;

	public:
		void onActiveListUpdate() override
		{
			if (((CommandListWithOnTickAsActiveList<T>*)this)->isCurrentUiList())
			{
				is_active = true;
				((CommandListWithOnTickAsActiveList<T>*)this)->onTickAsActiveList();
			}
			else
			{
				is_active = false;
			}
		}

	protected:
		virtual void onTickAsActiveList() = 0;
	};
#pragma pack(pop)
}

#pragma once

#include "Commands/Widgets/CommandList.hpp"

#include "lib/soup/rand.hpp"

#include "Commands/Widgets/CommandAction.hpp"
#include "Core/Exceptional.hpp"

namespace Stand
{
	class CommandStress : public CommandList
	{
	private:
		CommandAction* visual;
		bool erasing = false;
		bool thread_running = false;

	public:
		explicit CommandStress(CommandList* const parent)
			: CommandList(parent, LIT("Stress"))
		{
			visual = createChild<CommandAction>(NOLABEL);
		}

		void onActiveListUpdate() final
		{
			if (isCurrentMenuList())
			{
				if (!thread_running)
				{
					Exceptional::createManagedExceptionalThread(__FUNCTION__, [this]
					{
						thread_running = true;
						while (thread_running)
						{
							EXCEPTIONAL_LOCK_WRITE(g_gui.root_mtx)
							visual->setMenuName(LIT(soup::rand.str<std::string>(10)));
							if (!erasing)
							{
								if (children.size() == 11)
								{
									erasing = true;
								}
								else
								{
									createChild<CommandAction>(LIT("Deez"));
								}
							}
							else
							{
								if (children.size() == 1)
								{
									erasing = false;
								}
								else
								{
									children.erase(children.begin() + (children.size() - 1));
									processChildrenUpdate();
								}
							}
							EXCEPTIONAL_UNLOCK_WRITE(g_gui.root_mtx)
						}
					});
				}
			}
			else
			{
				thread_running = false;
			}
		}
	};
}

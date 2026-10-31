#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include <thread>

#include <soup/AnalogueKeyboard.hpp>
#include <soup/os.hpp>

#include "Core/Pointers.hpp"

namespace Stand
{
	extern "C" void rage_input_helper_set_override(float(*pArray)[256]);

	class CommandAnalogueInput : public CommandToggle
	{
	public:
		struct ThreadData
		{
			bool running = true;
			soup::AnalogueKeyboard* pKbd = nullptr;
		};

		ThreadData* current_thread_data;

		inline static float pressure_subtrahend = 0.0f;
		inline static float pressure_divisior = 1.0f;

		explicit CommandAnalogueInput(CommandList* const parent)
			: CommandToggle(parent, LOC("ANAL"), { CMDNAME("analogue"), CMDNAME("wooting") }, LOC("ANAL_H"))
		{
		}

		void onEnable(Click& click) final
		{
			current_thread_data = new ThreadData();
			std::thread([thread_data{ current_thread_data }]
			{
				float arr[256];
				while (thread_data->running)
				{
					for (auto& kbd : soup::AnalogueKeyboard::getAll())
					{
						thread_data->pKbd = &kbd;
						rage_input_helper_set_override(&arr);
						while (thread_data->running && !kbd.disconnected)
						{
							float new_arr[256];
							memset(new_arr, 0, sizeof(new_arr));
							if (!kbd.isPoll() || GetForegroundWindow() == *Pointers.Hwnd)
							{
								for (const auto& key : kbd.getActiveKeys())
								{
									const auto vk = key.getVk();
									if (vk >= 0 && vk < 256)
									{
										new_arr[vk] = std::clamp((key.fvalue - pressure_subtrahend) / pressure_divisior, 0.0f, 1.0f);
									}
								}
							}
							if (GetForegroundWindow() == *Pointers.Hwnd)
							{
								memcpy(arr, new_arr, sizeof(new_arr));
							}
							else
							{
								memset(arr, 0, sizeof(arr));
							}
						}
						thread_data->pKbd = nullptr;
					}
					rage_input_helper_set_override(nullptr);
					if (!thread_data->running)
					{
						break;
					}
					soup::os::sleep(100);
				}
				delete thread_data;
			}).detach();
		}

		void onDisable(Click& click) final
		{
			current_thread_data->running = false;
			if (current_thread_data->pKbd)
			{
				current_thread_data->pKbd->hid.cancelReceiveReport();
			}
		}
	};
}

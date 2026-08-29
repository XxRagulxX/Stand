#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "AntiCheat/Hooking.hpp"
#include "Rendering/Renderer.hpp"

namespace Stand
{
	class CommandStreamproof : public CommandToggle
	{
	public:
		explicit CommandStreamproof(CommandList* parent)
			: CommandToggle(parent, LOC("STRMPRF"), CMDNAMES("streamproof", "stream"), LOC("STRMPRF_H"))
		{
		}

		void onEnable(Click& click) final
		{
			if (!click.isAuto() && !g_hooking.streamproof_present_works)
			{
				click.setResponse(LOC("GENFAIL"));
				setOnIndication(false);
			}
			else
			{
				g_renderer.stream_proof = true;
			}
		}

		void onDisable(Click& click) final
		{
			g_renderer.stream_proof = false;
		}

		void applyDefaultState() final
		{
		}
	};
}

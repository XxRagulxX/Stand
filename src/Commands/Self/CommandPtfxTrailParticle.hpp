#pragma once
#include "Commands/Self/CommandPtfxTrail.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Rendering/Particles.hpp"

#include <vector>

namespace Stand
{
	class CommandPtfxTrailParticle final : public CommandSlider
	{
	public:
		CommandSlider*               m_interval = nullptr;
		CommandSliderFloat*          m_scale    = nullptr;
		CommandSliderFloat*          m_offset   = nullptr;
		std::vector<CommandPtfxTrail*> m_bones;

		explicit CommandPtfxTrailParticle(CommandList* parent);

		[[nodiscard]] std::string getValueText() const override;
		void onChange(Click& click, int prev_value) override;
		void onTick() override;
		void applyDefaultState() override;
	};
}

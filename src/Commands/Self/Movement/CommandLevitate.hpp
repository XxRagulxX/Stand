#pragma once
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/vector.hpp"
#include "Menu/Click.hpp"

#include <chrono>

namespace Stand
{
	class CommandLevitate : public CommandToggle
	{
		bool m_ZExtraGoingDown = false;
		float m_ZExtra = 0.0f;
		float m_CurrentSpeed = 0.0f;
		bool m_HasLastPos = false;
		rage::fvector3 m_LastPos{};
		bool m_HasMomentum = false;
		rage::fvector3 m_MomentumPos{};
		std::chrono::steady_clock::time_point m_MomentumTime{};

		CommandSlider* const base_speed;
		CommandSlider* const sprint_speed;
		CommandSlider* const acceleration;
		CommandToggle* const ignore_pitch;
		CommandToggle* const keep_momentum;
		CommandToggle* const apply_pitch_to_entity;
		CommandToggle* const only_rotate_on_movement;
		CommandToggle* const show_button_instructions;
		CommandSlider* const passive_min;
		CommandSlider* const passive_max;
		CommandSlider* const passive_speed;
		CommandSlider* const assist_up;
		CommandSlider* const assist_down;
		CommandSlider* const assist_deadzone;
		CommandSlider* const assist_snap;

	public:
		explicit CommandLevitate(
			CommandList* parent,
			CommandSlider* speed,
			CommandSlider* sprint_speed,
			CommandSlider* acceleration,
			CommandToggle* ignore_pitch,
			CommandToggle* keep_momentum,
			CommandToggle* apply_pitch_to_entity,
			CommandToggle* only_rotate_on_movement,
			CommandToggle* show_button_instructions,
			CommandSlider* passive_min,
			CommandSlider* passive_max,
			CommandSlider* passive_speed,
			CommandSlider* assist_up,
			CommandSlider* assist_down,
			CommandSlider* assist_deadzone,
			CommandSlider* assist_snap
		);

		void onEnable(Click& click) override;
		void onDisable(Click& click) override;
		void onTick() override;
		~CommandLevitate() override;
	};
}

#pragma once
#include "Commands/CommandLegacy.hpp"
#include "Commands/CommandPosition3d.hpp"
#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/Commands.hpp"
#include "Commands/LoopedCommand.hpp"
#include "Menu/GUI.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Joaat.hpp"

#include <string>
#include <utility>

namespace Stand::StandWidgets
{
	// Ported from real Stand's own CommandPositionSelection (Commands/
	// Widgets/CommandPositionSelection.cpp on origin/stand-reference) - a
	// button that closes the menu, lets you fly to a spot in the world,
	// and writes that position back into a CommandPosition3d once you
	// reopen the menu.
	//
	// Real Stand's own version enables its own dedicated freecam +
	// reticle + persistent world beacon while picking, and captures
	// wherever a raycast from the reticle hits world geometry. This
	// project reuses its own EXISTING "freecam" LoopedCommand instead of
	// building a second camera system (found and toggled by name, like
	// any other command - no changes to CommandFreecam.cpp needed), and
	// captures wherever the camera itself ends up (CAMERA::
	// GET_FINAL_RENDERED_CAM_COORD()) rather than raycasting onto world
	// geometry - so picking a position here means flying the freecam to
	// exactly where you want it, then reopening the menu (Insert),
	// instead of aiming a reticle at a surface from a distance. No
	// in-world beacon is drawn while picking either - real Stand's own
	// persistent beacon needs a dedicated marker/blip system this
	// project has no equivalent wiring for yet - a real, disclosed gap.
	//
	// Same hidden always-on LoopedCommand ticker shape as CommandToggleAll.hpp/
	// CommandTogglePointerDuplex.hpp - needed here to notice "the menu
	// was just reopened" every tick while a pick is in progress, since
	// there's no other hook in this system for "GUI::IsOpen() just
	// became true" to run code from instead.
	class CommandPositionPick : public CommandLegacy
	{
	public:
		CommandPositionPick(std::string name, std::string label, std::string description, CommandPosition3d* target) :
		    CommandLegacy(name, label, description, 0),
		    m_Target(target),
		    m_Ticker(name + "_tick", label + " Ticker", "Internal - always on, watches for the menu reopening while a position pick is in progress", this)
		{
		}

		// Public only so the private Ticker member below can reach it -
		// nested classes get no special access to their own outer
		// class's private members in C++. Nothing else should call this.
		void Tick()
		{
			if (!m_Picking || !GUI::IsOpen())
				return;

			m_Picking = false;

			const auto pos = CAMERA::GET_FINAL_RENDERED_CAM_COORD();
			if (m_Target)
				m_Target->SetState(rage::fvector3{pos.x, pos.y, pos.z});

			if (auto* freecam = Commands::GetCommand<CommandToggleLegacy>("freecam"_J))
				if (freecam->GetState())
					freecam->SetState(false);
		}

	protected:
		void OnCall() override
		{
			if (m_Picking)
				return;

			auto* freecam = Commands::GetCommand<CommandToggleLegacy>("freecam"_J);
			if (!freecam)
				return;

			m_Picking = true;
			if (!freecam->GetState())
				freecam->SetState(true);

			if (GUI::IsOpen())
				GUI::Toggle();
		}

	private:
		class Ticker : public LoopedCommand
		{
		public:
			Ticker(std::string name, std::string label, std::string description, CommandPositionPick* owner) :
			    LoopedCommand(std::move(name), std::move(label), std::move(description)),
			    m_Owner(owner)
			{
				m_State = true;
			}

			bool ShouldSaveState() const override
			{
				return false;
			}

		protected:
			void OnTick() override
			{
				m_Owner->Tick();
			}

		private:
			CommandPositionPick* m_Owner;
		};

		CommandPosition3d* m_Target;
		bool m_Picking = false;
		Ticker m_Ticker;
	};
}

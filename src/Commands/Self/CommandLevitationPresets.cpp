#include "Commands/Self/CommandLevitationPresets.hpp"
#include "Commands/Self/CommandTabLevitation.hpp"

namespace Stand
{
	CommandLevitationPresets::PresetLevitation::PresetLevitation(CommandList* parent, CommandTabLevitation* tab)
		: CommandPhysical(COMMAND_ACTION, parent, LIT("Levitation"), {}, NOLABEL)
		, m_Tab(tab)
	{
	}

	void CommandLevitationPresets::PresetLevitation::onClick(Click& click)
	{
		m_Tab->levitate->setStateBool(click, true);
		m_Tab->speed->setValue(1.0f, click);
		m_Tab->sprintMultiplier->setValue(5.0f, click);
		m_Tab->ignorePitch->setStateBool(click, true);
		m_Tab->passiveMin->setValue(0.0f, click);
		m_Tab->passiveMax->setValue(0.6f, click);
		m_Tab->passiveSpeed->setValue(click, 5);
		m_Tab->assistUp->setValue(0.6f, click);
		m_Tab->assistDown->setValue(0.6f, click);
		m_Tab->assistDeadzone->setValue(click, 13);
		m_Tab->assistSnap->setValue(0.1f, click);
	}

	CommandLevitationPresets::PresetNoClip::PresetNoClip(CommandList* parent, CommandTabLevitation* tab)
		: CommandPhysical(COMMAND_ACTION, parent, LIT("No Clip"), {}, NOLABEL)
		, m_Tab(tab)
	{
	}

	void CommandLevitationPresets::PresetNoClip::onClick(Click& click)
	{
		m_Tab->levitate->setStateBool(click, true);
		m_Tab->speed->setValue(2.0f, click);
		m_Tab->sprintMultiplier->setValue(0.5f, click);
		m_Tab->ignorePitch->setStateBool(click, false);
		m_Tab->passiveMin->setValue(0.0f, click);
		m_Tab->passiveMax->setValue(0.0f, click);
		m_Tab->passiveSpeed->setValue(click, 0);
		m_Tab->assistUp->setValue(0.0f, click);
		m_Tab->assistDown->setValue(0.0f, click);
		m_Tab->assistDeadzone->setValue(click, 0);
		m_Tab->assistSnap->setValue(0.0f, click);
	}

	CommandLevitationPresets::CommandLevitationPresets(CommandList* parent, CommandTabLevitation* tab)
		: CommandList(parent, LIT("Presets"), {})
	{
		createChild<PresetLevitation>(tab);
		createChild<PresetNoClip>(tab);
	}
}

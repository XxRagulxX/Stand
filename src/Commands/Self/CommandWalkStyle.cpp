#include "Commands/Self/CommandWalkStyle.hpp"

#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	const CommandWalkStyle::Entry CommandWalkStyle::styles[56] = {
		{"", "Normal"},
		{"move_f@arrogant@a", "Arrogant"},
		{"move_m@bag", "Bag"},
		{"MOVE_M@BAIL_BOND_NOT_TAZERED", "Bail Bond Bold"},
		{"MOVE_M@BAIL_BOND_TAZERED", "Bail Bond Tazered"},
		{"move_m@brave", "Brave"},
		{"move_ped_bucket", "Bucket"},
		{"ANIM_GROUP_MOVE_BALLISTIC", "Ballistic"},
		{"move_m@buzzed", "Buzzed"},
		{"move_m@casual@d", "Casual"},
		{"move_m@confident", "Confident"},
		{"move_ped_crouched", "Crouched"},
		{"move_f@flee@a", "Fleeing"},
		{"move_m@gangster@var_i", "Gangster"},
		{"move_m@gangster@var_f", "Shy Gangster"},
		{"move_m@gangster@var_e", "Slow Gangster"},
		{"move_m@prisoner_cuffed", "Handcuffed"},
		{"move_f@heels@c", "Heels"},
		{"move_m@hiking", "Hiking"},
		{"move_m@hipster@a", "Hipster"},
		{"clipset@move@trash_fast_turn", "Trash"},
		{"move_injured_generic", "Injured"},
		{"move_m@JOG@", "Jogging"},
		{"move_ped_mop", "Mop"},
		{"MOVE_M@PRISON_GAURD", "Prison Guard"},
		{"move_m@business@a", "Relaxed"},
		{"FEMALE_FAST_RUNNER", "Running"},
		{"move_m@sad@a", "Sad"},
		{"move_m@sassy", "Sassy Male"},
		{"move_f@sassy", "Sassy Female"},
		{"move_f@scared", "Scared"},
		{"move_f@sexy@a", "Sexy"},
		{"move_m@shadyped@a", "Shady"},
		{"move_m@fire", "Tough"},
		{"MOVE_M@DRUNK@SLIGHTLYDRUNK", "Slightly Drunk"},
		{"MOVE_M@DRUNK@MODERATEDRUNK", "Moderately Drunk"},
		{"MOVE_M@DRUNK@MODERATEDRUNK_HEAD_UP", "Drunk Head Up"},
		{"MOVE_M@DRUNK@VERYDRUNK", "Very Drunk"},
		{"MOVE_P_M_ONE", "Franklin"},
		{"MOVE_P_M_ONE_BRIEFCASE", "Franklin Briefcase"},
		{"move_characters@franklin@fire", "Franklin Tough"},
		{"move_characters@Jimmy@slow@", "Jimmy"},
		{"ANIM_GROUP_MOVE_LEMAR_ALLEY", "Lamar"},
		{"move_heist_lester", "Lester"},
		{"move_lester_CaneUp", "Lester Cane"},
		{"move_characters@michael@fire", "Michael"},
		{"move_p_m_zero_janitor", "Janitor"},
		{"move_p_m_zero_slow", "Slow Michael"},
		{"MOVE_M@FEMME@", "Femme Male"},
		{"MOVE_F@FEMME@", "Femme Female"},
		{"MOVE_M@GANGSTER@NG", "Male Gangster"},
		{"MOVE_F@GANGSTER@NG", "Female Gangster"},
		{"MOVE_M@POSH@", "Posh Male"},
		{"MOVE_F@POSH@", "Posh Female"},
		{"MOVE_M@TOUGH_GUY@", "Tough Guy Male"},
		{"MOVE_F@TOUGH_GUY@", "Tough Guy Female"},
	};

	CommandWalkStyle::CommandWalkStyle(CommandList* parent)
		: CommandSlider(parent, LIT("Walk Style"), CMDNAMES("walkstyle"), NOLABEL, 0, 55, 0, 1, 0)
	{
		CommandTickDispatch::AddCommand(this);
	}

	CommandWalkStyle::~CommandWalkStyle()
	{
		CommandTickDispatch::RemoveCommand(this);
	}

	std::string CommandWalkStyle::getValueText() const
	{
		if (value >= 0 && value < 56)
			return styles[value].name;
		return CommandSlider::getValueText();
	}

	void CommandWalkStyle::onChange(Click& click, int prev_value)
	{
	}

	void CommandWalkStyle::onTick()
	{
		const int ped = Self::GetPed().GetHandle();
		if (value == 0)
		{
			if (m_applied != 0)
			{
				m_applied = 0;
				PED::RESET_PED_MOVEMENT_CLIPSET(ped, 0.0f);
			}
			return;
		}
		const char* clipset = styles[value].clipset;
		if (!STREAMING::HAS_CLIP_SET_LOADED(clipset))
		{
			STREAMING::REQUEST_CLIP_SET(clipset);
			return;
		}
		PED::SET_PED_MOVEMENT_CLIPSET(ped, clipset, 0.0f);
		m_applied = value;
	}

	void CommandWalkStyle::applyDefaultState()
	{
		m_applied = -1;
		value = 0;
	}
}

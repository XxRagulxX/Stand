//#include "CommandToggle.hpp"
#include "World/Stats.hpp"
#include "Scripting/Natives.hpp"
#include "Commands/CommandSliderLegacy.hpp"
#include "Scripting/ScriptGlobal.hpp"
#include "Commands/LoopedCommand.hpp"
#include "Rendering/Notifications.hpp"
#include "Scripting/FiberPool.hpp"

namespace Stand::Features
{

	static CommandSliderLegacy _SetRankValue{"setrankvalue", "Rank", "Enter your desired RP level", std::nullopt, std::nullopt, 1};
	static CommandSliderLegacy _SetCrewRankValue{"setcrewvalue", "Crew Rank", "Enter your desired Crew Rank", std::nullopt, std::nullopt, 1};

	static const int RP_TABLE[] = {
	    0,
	    800,
	    2100,
	    3800,
	    6100,
	    9500,
	    12500,
	    16000,
	    19800,
	    24000,
	    28500,
	    33400,
	    38700,
	    44200,
	    50200,
	    56400,
	    63000,
	    69900,
	    77100,
	    84700,
	    92500,
	    100700,
	    109200,
	    118000,
	    127100,
	    136500,
	    146200,
	    156200,
	    166500,
	    177100,
	    188000,
	    199200,
	    210700,
	    222400,
	    234500,
	    246800,
	    259400,
	    272300,
	    285500,
	    299000,
	    312700,
	    326800,
	    341000,
	    355600,
	    370500,
	    385600,
	    401000,
	    416600,
	    432600,
	    448800,
	    465200,
	    482000,
	    499000,
	    516300,
	    533800,
	    551600,
	    569600,
	    588000,
	    606500,
	    625400,
	    644500,
	    663800,
	    683400,
	    703300,
	    723400,
	    743800,
	    764500,
	    785400,
	    806500,
	    827900,
	    849600,
	    871500,
	    893600,
	    916000,
	    938700,
	    961600,
	    984700,
	    1008100,
	    1031800,
	    1055700,
	    1079800,
	    1104200,
	    1128800,
	    1153700,
	    1178800,
	    1204200,
	    1229800,
	    1255600,
	    1281700,
	    1308100,
	    1334600,
	    1361400,
	    1388500,
	    1415800,
	    1443300,
	    1471100,
	    1499100,
	    1527300};

	class SetRank : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			// int level = _SetRankValue.GetState();

			// if (level < 0 || level > 8000)
			// {
			// 	Notifications::Show("Set Rank", "Invalid level (0 - 8000)", NotificationType::Error);
			// 	return;
			// }

			// int rp;

			// if (level < 98)
			// 	rp = RP_TABLE[level];
			// else
			// 	rp = (25 * level * level) + (23575 * level) - 1023150;

			// Stats::SetInt("MPX_CHAR_SET_RP_GIFT_ADMIN", rp);
			// Stats::SetInt("MPPLY_GLOBALXP", rp);

			// Notifications::Show("Set Rank", "Rank updated. Changing session...", NotificationType::Success);

			// NETWORK::NETWORK_SESSION_LEAVE_SINGLE_PLAYER();

			Notifications::Show("Currently Disabled", "Sets Rank", NotificationType::Error);
		}
	};

	class SetCrewRank : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			// int level = _SetCrewRankValue.GetState();

			// if (level < 0 || level > 8000)
			// {
			// 	Notifications::Show("Set Crew Rank", "Invalid level (0 - 8000)", NotificationType::Error);
			// 	return;
			// }

			// int rp;

			// if (level < 98)
			// 	rp = RP_TABLE[level];
			// else
			// 	rp = (25 * level * level) + (23575 * level) - 1023150;

			// for (int i = 0; i < 5; i++)
			// {
			// 	Stats::SetInt(("MPPLY_CREW_LOCAL_XP_" + std::to_string(i)).c_str(), rp);
			// }

			// Notifications::Show("Set Crew Rank", "Crew rank updated. Changing session...", NotificationType::Success);

			// NETWORK::NETWORK_SESSION_LEAVE_SINGLE_PLAYER();

			Notifications::Show("Currently Disabled", "Sets Crew Rank", NotificationType::Error);
		}
	};

	class BunkerResearch : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			FiberPool::queueJob([] {
				const int research[] = {
				    15381,
				    15382,
				    15428,
				    15429,
				    15430,
				    15431,
				    15432,
				    15433,
				    15434,
				    15435,
				    15436,
				    15437,
				    15438,
				    15439,
				    15447,
				    15448,
				    15449,
				    15450,
				    15451,
				    15452,
				    15453,
				    15454,
				    15455,
				    15456,
				    15457,
				    15458,
				    15459,
				    15460,
				    15461,
				    15462,
				    15463,
				    15464,
				    15465,
				    15466,
				    15467,
				    15468,
				    15469,
				    15470,
				    15471,
				    15472,
				    15473,
				    15474,
				    15491,
				    15492,
				    15493,
				    15494,
				    15495,
				    15496,
				    15497,
				    15498,
				    15499};

				for (int id : research)
					Stats::SetPackedBool(id, true);

				Notifications::Show("Bunker Research", "All bunker research unlocked successfully.", NotificationType::Success);
			});
		}
	};

	class UnlockClothing : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			Notifications::Show("Clothing", "Unlocking Clothing InProgress. Please wait until Completion", NotificationType::Info);
			*ScriptGlobal(262145).At(23812).As<int*>() = 1;
			int ids[] = {
			    3593,
			    3608,
			    3616,
			    3750,
			    6097,
			    6106,
			    6169,
			    6181,
			    7475,
			    7551,
			    9443,
			    15412,
			    15708,
			    15710,
			    15717,
			    15735,
			    15739,
			    15741,
			    15743,
			    18125,
			    22058,
			    22063,
			    22150,
			    22152,
			    22159,
			    22162,
			    22166,
			    22170,
			    22172,
			    22174,
			    25000,
			    25005,
			    25006,
			    26968,
			    26969,
			    27068,
			    27082,
			    27084,
			    28231,
			    28254,
			    28255,
			    30240,
			    30254,
			    30407,
			    31736,
			    31830,
			    31835,
			    31840,
			    31843,
			    31845,
			    31850,
			    31855,
			    31857,
			    31858,
			    31865,
			    31870,
			    31874,
			    31875,
			    31878,
			    31879,
			    31880,
			    31884,
			    31885,
			    31889,
			    31890,
			    31892,
			    31894,
			    31895,
			    31899,
			    31900,
			    31905,
			    31915,
			    31920,
			    31925,
			    31928,
			    31930,
			    31932,
			    31935,
			    31937,
			    31940,
			    31942,
			    31945,
			    31947,
			    31952,
			    31955,
			    31957,
			    31960,
			    31962,
			    31965,
			    31967,
			    31970,
			    31972,
			    31977,
			    31980,
			    31982,
			    31985,
			    31987,
			    31990,
			    31992,
			    31995,
			    32000,
			    32002,
			    32005,
			    32007,
			    32010,
			    32012,
			    32015,
			    32018,
			    32084,
			    32104,
			    32114,
			    32124,
			    32134,
			    32144,
			    32154,
			    32164,
			    32174,
			    32224,
			    32273,
			    32275,
			    32316,
			    32407,
			    32408,
			    34372,
			    34375,
			    34380,
			    34382,
			    34389,
			    36759,
			    36763,
			    36809,
			    42062,
			    42191,
			    42211,
			    42257,
			    42268,
			    42286,
			    42287,
			    54591,
			    54623,
			    54629,
			    54651,
			    24970,
			    24977,
			    25348,
			    25349};
			for (int i = 0; i < sizeof(ids) / sizeof(ids[0]); ++i)
			{
				Stats::SetPackedBool(ids[i], TRUE);
			}
			for (int i = 3783; i <= 3802; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 4257; i <= 4259; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 4262; i <= 4269; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 7467; i <= 7481; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 9426; i <= 9481; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 22124; i <= 22132; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 25244; i <= 25258; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 25265; i <= 25367; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 28171; i <= 28175; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 27184; i <= 27193; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 28185; i <= 28191; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 28197; i <= 28249; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 28393; i <= 28427; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 28447; i <= 28478; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 30254; i <= 30258; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 30355; i <= 30372; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 30418; i <= 30422; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 30429; i <= 30433; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 31760; i <= 31764; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 31827; i <= 32174; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 51215; i <= 51221; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 51234; i <= 51239; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 51240; i <= 51258; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int i = 15562; i <= 15568; i++)
			{
				Stats::SetPackedBool(i, TRUE);
			}
			for (int id = 110; id <= 113; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 3483; id <= 3492; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 3496; id <= 3505; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 3765; id <= 3779; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 3783; id <= 3802; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 4257; id <= 4269; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 4333; id <= 4335; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 6082; id <= 6083; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 6091; id <= 6092; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 6303; id <= 6304; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 6316; id <= 6317; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 7467; id <= 7469; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 7471; id <= 7495; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 7515; id <= 7528; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 7595; id <= 7601; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 9362; id <= 9385; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 9426; id <= 9440; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 9462; id <= 9481; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 15389; id <= 15390; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 15392; id <= 15396; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 15398; id <= 15399; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 15402; id <= 15407; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 15409; id <= 15410; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 15414; id <= 15423; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 15425; id <= 15427; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 15719; id <= 15721; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 15730; id <= 15732; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 18121; id <= 18125; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 18134; id <= 18137; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 22050; id <= 22051; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 22124; id <= 22132; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 25018; id <= 25021; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 25032; id <= 25099; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 27109; id <= 27115; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 27195; id <= 27197; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 27208; id <= 27211; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 30259; id <= 30289; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 30294; id <= 30295; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 30524; id <= 30557; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 30563; id <= 30583; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 30634; id <= 30637; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 30699; id <= 30701; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 30703; id <= 30704; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31768; id <= 31777; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31779; id <= 31782; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31784; id <= 31790; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31805; id <= 31808; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31827; id <= 31828; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31832; id <= 31833; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31837; id <= 31838; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31847; id <= 31848; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31852; id <= 31853; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31862; id <= 31863; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31867; id <= 31868; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31872; id <= 31873; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31882; id <= 31883; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31887; id <= 31888; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31897; id <= 31898; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31902; id <= 31903; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31907; id <= 31908; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31912; id <= 31913; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31917; id <= 31918; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 31922; id <= 31923; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 32020; id <= 32023; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 32025; id <= 32028; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 32030; id <= 32033; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 32035; id <= 32038; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 32040; id <= 32043; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 32045; id <= 32048; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 32050; id <= 32053; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 32055; id <= 32058; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 32060; id <= 32063; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 32065; id <= 32074; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 32307; id <= 32311; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 34394; id <= 34399; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 34401; id <= 34410; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 34505; id <= 34510; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 36703; id <= 36704; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 36717; id <= 36718; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 36737; id <= 36738; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 36751; id <= 36752; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 36768; id <= 36769; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 36774; id <= 36776; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 36782; id <= 36784; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 41885; id <= 41898; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 41902; id <= 41913; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 42193; id <= 42201; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 42203; id <= 42209; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 42213; id <= 42216; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 51248; id <= 51257; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 54572; id <= 54621; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 54625; id <= 54627; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 54631; id <= 54635; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			for (int id = 28232; id <= 28249; ++id)
			{
				Stats::SetPackedBool(id, TRUE);
			}
			Notifications::Show("Clothing", "Unlocking Clothing Completed", NotificationType::Success);
		}
	};
	class UnlockCareerProgressAwards : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			// Victory
			Stats::SetInt("MPX_AWD_FM_DM_WINS", 50);
			Stats::SetInt("MPX_AWD_FMWINAIRRACE", 25);
			Stats::SetInt("MPX_AWD_FMRALLYWONDRIVE", 25);
			Stats::SetInt("MPX_AWD_FMRALLYWONNAV", 25);
			Stats::SetInt("MPX_AWD_FMWINSEARACE", 25);
			Stats::SetInt("MPX_AWD_FM_TDM_WINS", 50);
			Stats::SetInt("MPX_AWD_FM_TDM_MVP", 50);
			Stats::SetInt("MPX_AWD_FM_GTA_RACES_WON", 50);
			Stats::SetInt("MPX_AWD_FMWINRACETOPOINTS", 25);
			Stats::SetBool("MPX_AWD_FMWINCUSTOMRACE", true);
			Stats::SetBool("MPX_AWD_FMKILL3ANDWINGTARACE", true);
			Stats::SetInt("MPX_AWD_RACES_WON", 50);
			Stats::SetInt("MPX_AWD_FM_RACE_LAST_FIRST", 25);
			Stats::SetInt("MPX_AWD_FMHORDWAVESSURVIVE", 10);
			Stats::SetInt("MPX_AWD_FM_TENNIS_WON", 25);
			Stats::SetBool("MPX_AWD_FM_TENNIS_5_SET_WINS", true);
			Stats::SetInt("MPX_MOST_ARM_WRESTLING_WINS", 25);
			Stats::SetBool("MPX_AWD_FMRACEWORLDRECHOLDER", true);
			Stats::SetInt("MPX_AWD_FM_RACES_FASTEST_LAP", 50);
			Stats::SetInt("MPX_AWD_WIN_AT_DARTS", 25);
			Stats::SetBool("MPX_AWD_FM_TENNIS_STASETWIN", true);
			Stats::SetInt("MPX_AWD_FM_SHOOTRANG_TG_WON", 25);
			Stats::SetInt("MPX_AWD_FM_GOLF_WON", 25);
			Stats::SetBool("MPX_AWD_FMWINALLRACEMODES", true);
			Stats::SetInt("MPX_AWD_FM_SHOOTRANG_CT_WON", 25);
			Stats::SetInt("MPX_AWD_WIN_LAST_TEAM_STANDINGS", 50);
			Stats::SetInt("MPX_AWD_WIN_CAPTURE_DONT_DYING", 25);
			Stats::SetInt("MPX_AWD_ONLY_PLAYER_ALIVE_LTS", 50);
			Stats::SetBool("MPX_AWD_FM_SHOOTRANG_GRAN_WON", true);
			Stats::SetInt("MPX_AWD_FM_SHOOTRANG_RT_WON", 25);
			Stats::SetInt("MPX_AWD_KILL_TEAM_YOURSELF_LTS", 25);
			Stats::SetInt("MPX_AWD_WIN_CAPTURES", 50);
			Stats::SetBool("MPX_AWD_FMWINEVERYGAMEMODE", true);


			// General
			Stats::SetInt("MPX_CR_DIFFERENT_DM", 25);
			Stats::SetBool("MPX_AWD_FM25DIFFERENTDM", true);
			Stats::SetInt("MPX_AWD_FMBASEJMP", 25);
			Stats::SetBool("MPX_AWD_FM_GOLF_HOLE_IN_1", true);
			Stats::SetInt("MPX_AWD_FM_TENNIS_ACE", 25);
			Stats::SetBool("MPX_AWD_FMATTGANGHQ", true);
			Stats::SetInt("MPX_CR_DIFFERENT_RACES", 25);
			Stats::SetBool("MPX_AWD_FM25DIFFERENTRACES", true);
			Stats::SetInt("MPX_AWD_PARACHUTE_JUMPS_20M", 25);
			Stats::SetBool("MPX_AWD_FM6DARTCHKOUT", true);
			Stats::SetInt("MPX_AWD_FMBBETWIN", 50000);
			Stats::SetInt("MPX_AWD_LAPDANCES", 25);
			Stats::SetInt("MPX_AWD_FM_GOLF_BIRDIES", 25);
			Stats::SetInt("MPX_AWD_PARACHUTE_JUMPS_50M", 25);
			Stats::SetBool("MPX_AWD_FM25DIFITEMSCLOTHES", true);
			Stats::SetInt("MPX_AWD_FM_CR_DM_MADE", 25);
			Stats::SetInt("MPX_AWD_DROPOFF_CAP_PACKAGES", 100);
			Stats::SetInt("MPX_AWD_PICKUP_CAP_PACKAGES", 100);
			Stats::SetInt("MPX_AWD_FM_CR_RACES_MADE", 25);
			Stats::SetInt("MPX_AWD_NO_HAIRCUTS", 25);
			Stats::SetBool("MPX_AWD_BUY_EVERY_GUN", true);
			Stats::SetInt("MPX_AWD_MENTALSTATE_TO_NORMAL", 50);
			Stats::SetBool("MPX_AWD_FMTATTOOALLBODYPARTS", true);
			Stats::SetInt("MPPLY_AWD_FM_CR_PLAYED_BY_PEEP", 100);
			Stats::SetBool("MPX_AWD_STORE_20_CAR_IN_GARAGES", true);
			Stats::SetInt("MPX_AWD_TRADE_IN_YOUR_PROPERTY", 25);
			Stats::SetInt("MPX_COMPLETEDAILYOBJ", 100);
			Stats::SetInt("MPX_COMPLETEDAILYOBJTOTAL", 100);
			Stats::SetInt("MPX_TOTALDAYCOMPLETED", 100);
			Stats::SetInt("MPX_TOTALWEEKCOMPLETED", 400);
			Stats::SetInt("MPX_TOTALMONTHCOMPLETED", 1800);
			Stats::SetInt("MPX_CONSECUTIVEDAYCOMPLETED", 30);
			Stats::SetInt("MPX_CONSECUTIVEWEEKCOMPLETED", 4);
			Stats::SetInt("MPX_CONSECUTIVEMONTHCOMPLETE", 1);
			Stats::SetInt("MPX_COMPLETEDAILYOBJSA", 100);
			Stats::SetInt("MPX_COMPLETEDAILYOBJTOTALSA", 100);
			Stats::SetInt("MPX_TOTALDAYCOMPLETEDSA", 100);
			Stats::SetInt("MPX_TOTALWEEKCOMPLETEDSA", 400);
			Stats::SetInt("MPX_TOTALMONTHCOMPLETEDSA", 1800);
			Stats::SetInt("MPX_CONSECUTIVEDAYCOMPLETEDSA", 30);
			Stats::SetInt("MPX_CONSECUTIVEWEEKCOMPLETEDSA", 4);
			Stats::SetInt("MPX_CONSECUTIVEMONTHCOMPLETESA", 1);
			Stats::SetInt("MPX_AWD_DAILYOBJCOMPLETEDSA", 100);
			Stats::SetInt("MPX_AWD_DAILYOBJCOMPLETED", 100);
			Stats::SetBool("MPX_AWD_DAILYOBJMONTHBONUS", true);
			Stats::SetBool("MPX_AWD_DAILYOBJWEEKBONUS", true);
			Stats::SetBool("MPX_AWD_DAILYOBJWEEKBONUSSA", true);
			Stats::SetBool("MPX_AWD_DAILYOBJMONTHBONUSSA", true);

			// Vehicle
			Stats::SetInt("MPX_CARS_EXPLODED", 500);
			Stats::SetInt("MPX_AWD_TIME_IN_HELICOPTER", 4);
			Stats::SetInt("MPX_MOST_SPINS_IN_ONE_JUMP", 5);
			Stats::SetBool("MPX_AWD_FMFURTHESTWHEELIE", true);
			Stats::SetBool("MPX_AWD_FMFULLYMODDEDCAR", true);
			Stats::SetInt("MPX_AWD_CARS_EXPORTED", 50);
			Stats::SetInt("MPX_AWD_FMDRIVEWITHOUTCRASH", 30);
			Stats::SetInt("MPX_AIR_LAUNCHES_OVER_40M", 25);
			Stats::SetInt("MPX_MOST_FLIPS_IN_ONE_JUMP", 5);
			Stats::SetInt("MPX_AWD_PASSENGERTIME", 4);

			// Combat
			Stats::SetInt("MPX_AWD_100_HEADSHOTS", 500);
			Stats::SetInt("MPX_AWD_FM_DM_KILLSTREAK", 100);
			Stats::SetBool("MPX_AWD_FMMOSTKILLSGANGHIDE", true);
			Stats::SetBool("MPX_AWD_FMMOSTKILLSSURVIVE", true);
			Stats::SetInt("MPX_AWD_FM_DM_STOLENKILL", 50);
			Stats::SetInt("MPX_KILLS_PLAYERS", 1000);
			Stats::SetInt("MPX_AWD_FMKILLBOUNTY", 25);
			Stats::SetInt("MPX_AWD_FM_DM_TOTALKILLS", 500);
			Stats::SetInt("MPX_AWD_FMREVENGEKILLSDM", 50);
			Stats::SetInt("MPX_PISTOL50_ENEMY_KILLS", 500);
			Stats::SetBool("MPX_AWD_FMKILLSTREAKSDM", true);
			Stats::SetInt("MPX_AWD_FM_DM_3KILLSAMEGUY", 50);
			Stats::SetInt("MPX_AWD_100_KILLS_SMG", 500);
			Stats::SetInt("MPX_CMBTMG_ENEMY_KILLS", 500);
			Stats::SetInt("MPX_UNARMED_ENEMY_KILLS", 50);
			Stats::SetInt("MPX_AWD_CAR_BOMBS_ENEMY_KILLS", 25);
			Stats::SetInt("MPX_AWD_25_KILLS_STICKYBOMBS", 50);
			Stats::SetInt("MPX_ASLTSHTGN_ENEMY_KILLS", 500);
			Stats::SetInt("MPX_ASLTRIFLE_ENEMY_KILLS", 500);
			Stats::SetInt("MPX_GRENADE_ENEMY_KILLS", 50);
			Stats::SetInt("MPX_AWD_KILL_CARRIER_CAPTURE", 100);
			Stats::SetInt("MPX_AWD_NIGHTVISION_KILLS", 100);
			Stats::SetInt("MPX_RPG_ENEMY_KILLS", 500);
			Stats::SetInt("MPX_SNIPERRFL_ENEMY_KILLS", 500);
			Stats::SetInt("MPX_AWD_KILL_PSYCHOPATHS", 100);

			// Apartment Awards
			Stats::SetInt("MPX_AWD_FINISH_HEISTS", 900);
			Stats::SetInt("MPX_MPPLY_WIN_GOLD_MEDAL_HEISTS", 900);
			Stats::SetInt("MPX_AWD_DO_HEIST_AS_MEMBER", 900);
			Stats::SetInt("MPX_AWD_DO_HEIST_AS_THE_LEADER", 900);
			Stats::SetInt("MPX_AWD_FINISH_HEIST_SETUP_JOB", 900);
			Stats::SetInt("MPX_AWD_FINISH_HEIST", 900);
			Stats::SetInt("MPX_HEIST_COMPLETION", 900);
			Stats::SetInt("MPX_HEISTS_ORGANISED", 900);
			Stats::SetInt("MPX_AWD_CONTROL_CROWDS", 900);
			Stats::SetInt("MPX_AWD_WIN_GOLD_MEDAL_HEISTS", 900);
			Stats::SetInt("MPX_AWD_COMPLETE_HEIST_NOT_DIE", 900);
			Stats::SetInt("MPX_HEIST_START", 900);
			Stats::SetInt("MPX_HEIST_END", 900);
			Stats::SetInt("MPX_CUTSCENE_MID_PRISON", 900);
			Stats::SetInt("MPX_CUTSCENE_MID_HUMANE", 900);
			Stats::SetInt("MPX_CUTSCENE_MID_NARC", 900);
			Stats::SetInt("MPX_CUTSCENE_MID_ORNATE", 900);
			Stats::SetInt("MPX_CR_FLEECA_PREP_1", 5000);
			Stats::SetInt("MPX_CR_FLEECA_PREP_2", 5000);
			Stats::SetInt("MPX_CR_FLEECA_FINALE", 5000);
			Stats::SetInt("MPX_CR_PRISON_PLANE", 5000);
			Stats::SetInt("MPX_CR_PRISON_BUS", 5000);
			Stats::SetInt("MPX_CR_PRISON_STATION", 5000);
			Stats::SetInt("MPX_CR_PRISON_UNFINISHED_BIZ", 5000);
			Stats::SetInt("MPX_CR_PRISON_FINALE", 5000);
			Stats::SetInt("MPX_CR_HUMANE_KEY_CODES", 5000);
			Stats::SetInt("MPX_CR_HUMANE_ARMORDILLOS", 5000);
			Stats::SetInt("MPX_CR_HUMANE_EMP", 5000);
			Stats::SetInt("MPX_CR_HUMANE_VALKYRIE", 5000);
			Stats::SetInt("MPX_CR_HUMANE_FINALE", 5000);
			Stats::SetInt("MPX_CR_NARC_COKE", 5000);
			Stats::SetInt("MPX_CR_NARC_TRASH_TRUCK", 5000);
			Stats::SetInt("MPX_CR_NARC_BIKERS", 5000);
			Stats::SetInt("MPX_CR_NARC_WEED", 5000);
			Stats::SetInt("MPX_CR_NARC_STEAL_METH", 5000);
			Stats::SetInt("MPX_CR_NARC_FINALE", 5000);
			Stats::SetInt("MPX_CR_PACIFIC_TRUCKS", 5000);
			Stats::SetInt("MPX_CR_PACIFIC_WITSEC", 5000);
			Stats::SetInt("MPX_CR_PACIFIC_HACK", 5000);
			Stats::SetInt("MPX_CR_PACIFIC_BIKES", 5000);
			Stats::SetInt("MPX_CR_PACIFIC_CONVOY", 5000);
			Stats::SetInt("MPX_CR_PACIFIC_FINALE", 5000);
			Stats::SetInt("MPX_MPPLY_HEIST_ACH_TRACKER", -1);
			Stats::SetBool("MPX_MPPLY_AWD_COMPLET_HEIST_MEM", true);
			Stats::SetBool("MPX_MPPLY_AWD_COMPLET_HEIST_1STPER", true);
			Stats::SetBool("MPX_MPPLY_AWD_FLEECA_FIN", true);
			Stats::SetBool("MPX_MPPLY_AWD_HST_ORDER", true);
			Stats::SetBool("MPX_MPPLY_AWD_HST_SAME_TEAM", true);
			Stats::SetBool("MPX_MPPLY_AWD_HST_ULT_CHAL", true);
			Stats::SetBool("MPX_MPPLY_AWD_HUMANE_FIN", true);
			Stats::SetBool("MPX_MPPLY_AWD_PACIFIC_FIN", true);
			Stats::SetBool("MPX_MPPLY_AWD_PRISON_FIN", true);
			Stats::SetBool("MPX_MPPLY_AWD_SERIESA_FIN", true);
			Stats::SetBool("MPX_AWD_FINISH_HEIST_NO_DAMAGE", true);
			Stats::SetBool("MPX_AWD_SPLIT_HEIST_TAKE_EVENLY", true);
			Stats::SetBool("MPX_AWD_ALL_ROLES_HEIST", true);
			Stats::SetBool("MPX_AWD_MATCHING_OUTFIT_HEIST", true);
			Stats::SetBool("MPX_HEIST_PLANNING_DONE_PRINT", true);
			Stats::SetBool("MPX_HEIST_PLANNING_DONE_HELP_0", true);
			Stats::SetBool("MPX_HEIST_PLANNING_DONE_HELP_1", true);
			Stats::SetBool("MPX_HEIST_PRE_PLAN_DONE_HELP_0", true);
			Stats::SetBool("MPX_HEIST_CUTS_DONE_FINALE", true);
			Stats::SetBool("MPX_HEIST_IS_TUTORIAL", false);
			Stats::SetBool("MPX_HEIST_STRAND_INTRO_DONE", true);
			Stats::SetBool("MPX_HEIST_CUTS_DONE_ORNATE", true);
			Stats::SetBool("MPX_HEIST_CUTS_DONE_PRISON", true);
			Stats::SetBool("MPX_HEIST_CUTS_DONE_BIOLAB", true);
			Stats::SetBool("MPX_HEIST_CUTS_DONE_NARCOTIC", true);
			Stats::SetBool("MPX_HEIST_CUTS_DONE_TUTORIAL", true);
			Stats::SetBool("MPX_HEIST_AWARD_DONE_PREP", true);
			Stats::SetBool("MPX_HEIST_AWARD_BOUGHT_IN", true);


			// Doomsday Awards
			Stats::SetInt("MPX_GANGOPS_FM_MISSION_PROG", -1);
			Stats::SetInt("MPX_GANGOPS_FLOW_MISSION_PROG", -1);
			Stats::SetInt("MPPLY_GANGOPS_ALLINORDER", 100);
			Stats::SetInt("MPPLY_GANGOPS_LOYALTY", 100);
			Stats::SetInt("MPPLY_GANGOPS_CRIMMASMD", 100);
			Stats::SetInt("MPPLY_GANGOPS_LOYALTY2", 100);
			Stats::SetInt("MPPLY_GANGOPS_LOYALTY3", 100);
			Stats::SetInt("MPPLY_GANGOPS_CRIMMASMD2", 100);
			Stats::SetInt("MPPLY_GANGOPS_CRIMMASMD3", 100);
			Stats::SetInt("MPX_MPPLY_GANGOPS_SUPPORT", 100);
			Stats::SetInt("MPX_CR_GANGOP_MORGUE", 10);
			Stats::SetInt("MPX_CR_GANGOP_DELUXO", 10);
			Stats::SetInt("MPX_CR_GANGOP_SERVERFARM", 10);
			Stats::SetInt("MPX_CR_GANGOP_IAABASE_FIN", 10);
			Stats::SetInt("MPX_CR_GANGOP_STEALOSPREY", 10);
			Stats::SetInt("MPX_CR_GANGOP_FOUNDRY", 10);
			Stats::SetInt("MPX_CR_GANGOP_RIOTVAN", 10);
			Stats::SetInt("MPX_CR_GANGOP_SUBMARINECAR", 10);
			Stats::SetInt("MPX_CR_GANGOP_SUBMARINE_FIN", 10);
			Stats::SetInt("MPX_CR_GANGOP_PREDATOR", 10);
			Stats::SetInt("MPX_CR_GANGOP_BMLAUNCHER", 10);
			Stats::SetInt("MPX_CR_GANGOP_BCCUSTOM", 10);
			Stats::SetInt("MPX_CR_GANGOP_STEALTHTANKS", 10);
			Stats::SetInt("MPX_CR_GANGOP_SPYPLANE", 10);
			Stats::SetInt("MPX_CR_GANGOP_FINALE", 10);
			Stats::SetInt("MPX_CR_GANGOP_FINALE_P2", 10);
			Stats::SetInt("MPX_CR_GANGOP_FINALE_P3", 10);
			Stats::SetBool("MPPLY_AWD_GANGOPS_IAA", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_SUBMARINE", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_MISSILE", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_ALLINORDER", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_LOYALTY", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_LOYALTY2", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_LOYALTY3", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_CRIMMASMD", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_CRIMMASMD2", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_CRIMMASMD3", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_SUPPORT", true);

			// Nightclub Awards
			Stats::SetInt("MPX_AWD_DANCE_TO_SOLOMUN", 120);
			Stats::SetInt("MPX_AWD_DANCE_TO_TALEOFUS", 120);
			Stats::SetInt("MPX_AWD_DANCE_TO_DIXON", 120);
			Stats::SetInt("MPX_AWD_DANCE_TO_BLKMAD", 120);
			Stats::SetInt("MPX_AWD_CLUB_DRUNK", 200);
			Stats::SetInt("MPX_NIGHTCLUB_VIP_APPEAR", 700);
			Stats::SetInt("MPX_NIGHTCLUB_JOBS_DONE", 700);
			Stats::SetInt("MPX_NIGHTCLUB_EARNINGS", 20721002);
			Stats::SetInt("MPX_HUB_SALES_COMPLETED", 1001);
			Stats::SetInt("MPX_HUB_EARNINGS", 320721002);
			Stats::SetInt("MPX_DANCE_COMBO_DURATION_MINS", 3600000);
			Stats::SetInt("MPX_NIGHTCLUB_PLAYER_APPEAR", 500);
			Stats::SetInt("MPX_LIFETIME_HUB_GOODS_SOLD", 784672);
			Stats::SetInt("MPX_LIFETIME_HUB_GOODS_MADE", 507822);
			Stats::SetInt("MPX_DANCEPERFECTOWNCLUB", 120);
			Stats::SetInt("MPX_NUMUNIQUEPLYSINCLUB", 120);
			Stats::SetInt("MPX_DANCETODIFFDJS", 4);
			Stats::SetInt("MPX_NIGHTCLUB_HOTSPOT_TIME_MS", 3600000);
			Stats::SetInt("MPX_NIGHTCLUB_CONT_TOTAL", 20);
			Stats::SetInt("MPX_NIGHTCLUB_CONT_MISSION", -1);
			Stats::SetInt("MPX_CLUB_CONTRABAND_MISSION", 1000);
			Stats::SetInt("MPX_HUB_CONTRABAND_MISSION", 1000);
			Stats::SetBool("MPX_AWD_CLUB_HOTSPOT", true);
			Stats::SetBool("MPX_AWD_CLUB_CLUBBER", true);
			Stats::SetBool("MPX_AWD_CLUB_COORD", true);


			Stats::SetPackedBool(15554, TRUE); // Battler Trophy Bronze
			Stats::SetPackedBool(15557, TRUE); // Battler Trophy Silver
			Stats::SetPackedBool(15560, TRUE); // Battler Trophy Gold
			Stats::SetPackedBool(15553, TRUE); // Number One Nightclub Trophy Bronze
			Stats::SetPackedBool(15556, TRUE); // Number One Nightclub Trophy Silver
			Stats::SetPackedBool(15559, TRUE); // Number One Nightclub Trophy Gold
			Stats::SetPackedBool(15552, TRUE); // Greatest Dancer Trophy Bronze
			Stats::SetPackedBool(15555, TRUE); // Greatest Dancer Trophy Silver
			Stats::SetPackedBool(15558, TRUE); // Greatest Dancer Trophy Gold


			// Arena War Awards
			Stats::SetInt("MPX_ARN_BS_TRINKET_TICKERS", -1);
			Stats::SetInt("MPX_ARN_BS_TRINKET_SAVED", -1);
			Stats::SetInt("MPX_AWD_WATCH_YOUR_STEP", 50);
			Stats::SetInt("MPX_AWD_TOWER_OFFENSE", 50);
			Stats::SetInt("MPX_AWD_READY_FOR_WAR", 50);
			Stats::SetInt("MPX_AWD_THROUGH_A_LENS", 50);
			Stats::SetInt("MPX_AWD_SPINNER", 50);
			Stats::SetInt("MPX_AWD_YOUMEANBOOBYTRAPS", 50);
			Stats::SetInt("MPX_AWD_MASTER_BANDITO", 50);
			Stats::SetInt("MPX_AWD_SITTING_DUCK", 50);
			Stats::SetInt("MPX_AWD_CROWDPARTICIPATION", 50);
			Stats::SetInt("MPX_AWD_KILL_OR_BE_KILLED", 50);
			Stats::SetInt("MPX_AWD_MASSIVE_SHUNT", 50);
			Stats::SetInt("MPX_AWD_YOURE_OUTTA_HERE", 200);
			Stats::SetInt("MPX_AWD_WEVE_GOT_ONE", 50);
			Stats::SetInt("MPX_AWD_ARENA_WAGEWORKER", 1000000);
			Stats::SetInt("MPX_AWD_TIME_SERVED", 1000);
			Stats::SetInt("MPX_AWD_TOP_SCORE", 55000);
			Stats::SetInt("MPX_AWD_CAREER_WINNER", 1000);
			Stats::SetInt("MPX_ARENAWARS_SP", 209);
			Stats::SetInt("MPX_ARENAWARS_SKILL_LEVEL", 20);
			Stats::SetInt("MPX_ARENAWARS_SP_LIFETIME", 209);
			Stats::SetInt("MPX_ARENAWARS_AP_TIER", 1000);
			Stats::SetInt("MPX_ARENAWARS_AP_LIFETIME", 47551850);
			Stats::SetInt("MPX_ARENAWARS_CARRER_UNLK", 44);
			Stats::SetInt("MPX_ARN_W_THEME_SCIFI", 1000);
			Stats::SetInt("MPX_ARN_W_THEME_APOC", 1000);
			Stats::SetInt("MPX_ARN_W_THEME_CONS", 1000);
			Stats::SetInt("MPX_ARN_W_PASS_THE_BOMB", 1000);
			Stats::SetInt("MPX_ARN_W_DETONATION", 1000);
			Stats::SetInt("MPX_ARN_W_ARCADE_RACE", 1000);
			Stats::SetInt("MPX_ARN_W_CTF", 1000);
			Stats::SetInt("MPX_ARN_W_TAG_TEAM", 1000);
			Stats::SetInt("MPX_ARN_W_DESTR_DERBY", 1000);
			Stats::SetInt("MPX_ARN_W_CARNAGE", 1000);
			Stats::SetInt("MPX_ARN_W_MONSTER_JAM", 1000);
			Stats::SetInt("MPX_ARN_W_GAMES_MASTERS", 1000);
			Stats::SetInt("MPX_ARN_L_PASS_THE_BOMB", 500);
			Stats::SetInt("MPX_ARN_L_DETONATION", 500);
			Stats::SetInt("MPX_ARN_L_ARCADE_RACE", 500);
			Stats::SetInt("MPX_ARN_L_CTF", 500);
			Stats::SetInt("MPX_ARN_L_TAG_TEAM", 500);
			Stats::SetInt("MPX_ARN_L_DESTR_DERBY", 500);
			Stats::SetInt("MPX_ARN_L_CARNAGE", 500);
			Stats::SetInt("MPX_ARN_L_MONSTER_JAM", 500);
			Stats::SetInt("MPX_ARN_L_GAMES_MASTERS", 500);
			Stats::SetInt("MPX_NUMBER_OF_CHAMP_BOUGHT", 1000);
			Stats::SetInt("MPX_ARN_SPECTATOR_KILLS", 1000);
			Stats::SetInt("MPX_ARN_LIFETIME_KILLS", 1000);
			Stats::SetInt("MPX_ARN_LIFETIME_DEATHS", 500);
			Stats::SetInt("MPX_ARENAWARS_CARRER_WINS", 1000);
			Stats::SetInt("MPX_ARENAWARS_CARRER_WINT", 1000);
			Stats::SetInt("MPX_ARENAWARS_MATCHES_PLYD", 1000);
			Stats::SetInt("MPX_ARENAWARS_MATCHES_PLYDT", 1000);
			Stats::SetInt("MPX_ARN_SPEC_BOX_TIME_MS", 86400000);
			Stats::SetInt("MPX_ARN_SPECTATOR_DRONE", 1000);
			Stats::SetInt("MPX_ARN_SPECTATOR_CAMS", 1000);
			Stats::SetInt("MPX_ARN_SMOKE", 1000);
			Stats::SetInt("MPX_ARN_DRINK", 1000);
			Stats::SetInt("MPX_ARN_VEH_MONSTER", 31000);
			Stats::SetInt("MPX_ARN_VEH_MONSTER", 41000);
			Stats::SetInt("MPX_ARN_VEH_MONSTER", 51000);
			Stats::SetInt("MPX_ARN_VEH_CERBERUS", 1000);
			Stats::SetInt("MPX_ARN_VEH_CERBERUS2", 1000);
			Stats::SetInt("MPX_ARN_VEH_CERBERUS3", 1000);
			Stats::SetInt("MPX_ARN_VEH_BRUISER", 1000);
			Stats::SetInt("MPX_ARN_VEH_BRUISER2", 1000);
			Stats::SetInt("MPX_ARN_VEH_BRUISER3", 1000);
			Stats::SetInt("MPX_ARN_VEH_SLAMVAN4", 1000);
			Stats::SetInt("MPX_ARN_VEH_SLAMVAN5", 1000);
			Stats::SetInt("MPX_ARN_VEH_SLAMVAN6", 1000);
			Stats::SetInt("MPX_ARN_VEH_BRUTUS", 1000);
			Stats::SetInt("MPX_ARN_VEH_BRUTUS2", 1000);
			Stats::SetInt("MPX_ARN_VEH_BRUTUS3", 1000);
			Stats::SetInt("MPX_ARN_VEH_SCARAB", 1000);
			Stats::SetInt("MPX_ARN_VEH_SCARAB2", 1000);
			Stats::SetInt("MPX_ARN_VEH_SCARAB3", 1000);
			Stats::SetInt("MPX_ARN_VEH_DOMINATOR4", 1000);
			Stats::SetInt("MPX_ARN_VEH_DOMINATOR5", 1000);
			Stats::SetInt("MPX_ARN_VEH_DOMINATOR6", 1000);
			Stats::SetInt("MPX_ARN_VEH_IMPALER2", 1000);
			Stats::SetInt("MPX_ARN_VEH_IMPALER3", 1000);
			Stats::SetInt("MPX_ARN_VEH_IMPALER4", 1000);
			Stats::SetInt("MPX_ARN_VEH_ISSI4", 1000);
			Stats::SetInt("MPX_ARN_VEH_ISSI5", 1000);
			Stats::SetInt("MPX_ARN_VEH_ISSI", 61000);
			Stats::SetInt("MPX_ARN_VEH_IMPERATOR", 1000);
			Stats::SetInt("MPX_ARN_VEH_IMPERATOR2", 1000);
			Stats::SetInt("MPX_ARN_VEH_IMPERATOR3", 1000);
			Stats::SetInt("MPX_ARN_VEH_ZR380", 1000);
			Stats::SetInt("MPX_ARN_VEH_ZR3802", 1000);
			Stats::SetInt("MPX_ARN_VEH_ZR3803", 1000);
			Stats::SetInt("MPX_ARN_VEH_DEATHBIKE", 1000);
			Stats::SetInt("MPX_ARN_VEH_DEATHBIKE2", 1000);
			Stats::SetInt("MPX_ARN_VEH_DEATHBIKE3", 1000);
			Stats::SetBool("MPX_AWD_BEGINNER", true);
			Stats::SetBool("MPX_AWD_FIELD_FILLER", true);
			Stats::SetBool("MPX_AWD_ARMCHAIR_RACER", true);
			Stats::SetBool("MPX_AWD_LEARNER", true);
			Stats::SetBool("MPX_AWD_SUNDAY_DRIVER", true);
			Stats::SetBool("MPX_AWD_THE_ROOKIE", true);
			Stats::SetBool("MPX_AWD_BUMP_AND_RUN", true);
			Stats::SetBool("MPX_AWD_GEAR_HEAD", true);
			Stats::SetBool("MPX_AWD_DOOR_SLAMMER", true);
			Stats::SetBool("MPX_AWD_HOT_LAP", true);
			Stats::SetBool("MPX_AWD_ARENA_AMATEUR", true);
			Stats::SetBool("MPX_AWD_PAINT_TRADER", true);
			Stats::SetBool("MPX_AWD_SHUNTER", true);
			Stats::SetBool("MPX_AWD_JOCK", true);
			Stats::SetBool("MPX_AWD_WARRIOR", true);
			Stats::SetBool("MPX_AWD_T_BONE", true);
			Stats::SetBool("MPX_AWD_MAYHEM", true);
			Stats::SetBool("MPX_AWD_WRECKER", true);
			Stats::SetBool("MPX_AWD_CRASH_COURSE", true);
			Stats::SetBool("MPX_AWD_ARENA_LEGEND", true);
			Stats::SetBool("MPX_AWD_PEGASUS", true);
			Stats::SetBool("MPX_AWD_UNSTOPPABLE", true);
			Stats::SetBool("MPX_AWD_CONTACT_SPORT", true);

			// Casino Heist Awards
			Stats::SetBool("MPX_AWD_KINGOFQUB3D", true);
			Stats::SetBool("MPX_AWD_QUBISM", true);
			Stats::SetBool("MPX_AWD_QUIBITS", true);
			Stats::SetBool("MPX_AWD_GODOFQUB3D", true);
			Stats::SetBool("MPX_AWD_GOFOR11TH", true);
			Stats::SetBool("MPX_AWD_ELEVENELEVEN", true);
			Stats::SetBool("MPX_AWD_SCOPEOUT", true);
			Stats::SetBool("MPX_AWD_CREWEDUP", true);
			Stats::SetBool("MPX_AWD_MOVINGON", true);
			Stats::SetBool("MPX_AWD_PROMOCAMP", true);
			Stats::SetBool("MPX_AWD_GUNMAN", true);
			Stats::SetBool("MPX_AWD_SMASHNGRAB", true);
			Stats::SetBool("MPX_AWD_INPLAINSI", true);
			Stats::SetBool("MPX_AWD_UNDETECTED", true);
			Stats::SetBool("MPX_AWD_ALLROUND", true);
			Stats::SetBool("MPX_AWD_ELITETHEIF", true);
			Stats::SetBool("MPX_AWD_PRO", true);
			Stats::SetBool("MPX_AWD_SUPPORTACT", true);
			Stats::SetBool("MPX_AWD_SHAFTED", true);
			Stats::SetBool("MPX_AWD_COLLECTOR", true);
			Stats::SetBool("MPX_AWD_DEADEYE", true);
			Stats::SetBool("MPX_AWD_PISTOLSATDAWN", true);
			Stats::SetBool("MPX_AWD_TRAFFICAVOI", true);
			Stats::SetBool("MPX_AWD_CANTCATCHBRA", true);
			Stats::SetBool("MPX_AWD_WIZHARD", true);
			Stats::SetBool("MPX_AWD_APEESCAPE", true);
			Stats::SetBool("MPX_AWD_MONKEYKIND", true);
			Stats::SetBool("MPX_AWD_AQUAAPE", true);
			Stats::SetBool("MPX_AWD_KEEPFAITH", true);
			Stats::SetBool("MPX_AWD_TRUELOVE", true);
			Stats::SetBool("MPX_AWD_NEMESIS", true);
			Stats::SetBool("MPX_AWD_FRIENDZONED", true);
			Stats::SetInt("MPX_AWD_ASTROCHIMP", 3000000);
			Stats::SetInt("MPX_AWD_MASTERFUL", 40000);
			Stats::SetInt("MPX_AWD_PREPARATION", 40);
			Stats::SetInt("MPX_AWD_ASLEEPONJOB", 20);
			Stats::SetInt("MPX_AWD_DAICASHCRAB", 100000);
			Stats::SetInt("MPX_AWD_BIGBRO", 40);
			Stats::SetInt("MPX_AWD_SHARPSHOOTER", 40);
			Stats::SetInt("MPX_AWD_RACECHAMP", 40);
			Stats::SetInt("MPX_AWD_BATSWORD", 1000000);
			Stats::SetInt("MPX_AWD_COINPURSE", 950000);
			for (int i = 0; i < 10; i++)
			{
				Stats::SetInt(("MPX_IAP_INITIALS_" + std::to_string(i)).c_str(), 50);
				Stats::SetInt(("MPX_DG_DEFENDER_INITIALS_" + std::to_string(i)).c_str(), 69644);
				Stats::SetInt(("MPX_DG_DEFENDER_SCORE_" + std::to_string(i)).c_str(), 50);
				Stats::SetInt(("MPX_DG_MONKEY_INITIALS_" + std::to_string(i)).c_str(), 69644);
				Stats::SetInt(("MPX_DG_MONKEY_SCORE_" + std::to_string(i)).c_str(), 50);
				Stats::SetInt(("MPX_DG_PENETRATOR_INITIALS_" + std::to_string(i)).c_str(), 69644);
				Stats::SetInt(("MPX_DG_PENETRATOR_SCORE_" + std::to_string(i)).c_str(), 50);
				Stats::SetInt(("MPX_GGSM_INITIALS_" + std::to_string(i)).c_str(), 69644);
				Stats::SetInt(("MPX_GGSM_SCORE_" + std::to_string(i)).c_str(), 50);
				Stats::SetInt(("MPX_TWR_INITIALS_" + std::to_string(i)).c_str(), 69644);
				Stats::SetInt(("MPX_TWR_SCORE_" + std::to_string(i)).c_str(), 50);
			}
			Stats::SetInt("MPX_IAP_SCORE_0", 69644);
			Stats::SetInt("MPX_IAP_SCORE_1", 50333);
			Stats::SetInt("MPX_IAP_SCORE_2", 63512);
			Stats::SetInt("MPX_IAP_SCORE_3", 46136);
			Stats::SetInt("MPX_IAP_SCORE_4", 21638);
			Stats::SetInt("MPX_IAP_SCORE_5", 2133);
			Stats::SetInt("MPX_IAP_SCORE_6", 1215);
			Stats::SetInt("MPX_IAP_SCORE_7", 2444);
			Stats::SetInt("MPX_IAP_SCORE_8", 38023);
			Stats::SetInt("MPX_IAP_SCORE_9", 2233);
			Stats::SetInt("MPX_SCGW_SCORE_1", 50);
			Stats::SetInt("MPX_SCGW_SCORE_2", 50);
			Stats::SetInt("MPX_SCGW_SCORE_3", 50);
			Stats::SetInt("MPX_SCGW_SCORE_4", 50);
			Stats::SetInt("MPX_SCGW_SCORE_5", 50);
			Stats::SetInt("MPX_SCGW_SCORE_6", 50);
			Stats::SetInt("MPX_SCGW_SCORE_7", 50);
			Stats::SetInt("MPX_SCGW_SCORE_8", 50);
			Stats::SetInt("MPX_SCGW_SCORE_9", 50);
			Stats::SetInt("SCGW_NUM_WINS_GANG_0", 50);
			Stats::SetInt("SCGW_NUM_WINS_GANG_1", 50);
			Stats::SetInt("SCGW_NUM_WINS_GANG_2", 50);
			Stats::SetInt("SCGW_NUM_WINS_GANG_3", 50);
			Stats::SetInt("IAP_MAX_MOON_DIST", INT_MAX);
			Stats::SetBool("SCGW_WON_NO_DEATHS", true);
			Stats::SetBool("IAP_CHALLENGE_0", true);
			Stats::SetBool("IAP_CHALLENGE_1", true);
			Stats::SetBool("IAP_CHALLENGE_2", true);
			Stats::SetBool("IAP_CHALLENGE_3", true);
			Stats::SetBool("IAP_CHALLENGE_4", true);
			Stats::SetBool("IAP_GOLD_TANK", true);
			Stats::SetInt("CH_ARC_CAB_CLAW_TROPHY", -1);
			Stats::SetInt("CH_ARC_CAB_LOVE_TROPHY", -1);
			Stats::SetInt("CAS_HEIST_NOTS", -1);
			Stats::SetInt("MPX_CAS_HEIST_NOTS", -1);
			Stats::SetInt("MPX_CH_ARC_CAB_CLAW_TROPHY", -1);
			Stats::SetInt("MPX_CH_ARC_CAB_LOVE_TROPHY", -1);
			Stats::SetInt("MPX_SIGNAL_JAMMERS_COLLECTED", 50);
			Stats::SetInt("MPX_AWD_ODD_JOBS", 52);
			Stats::SetInt("MPX_H3_BOARD_DIALOGUE0", -1);
			Stats::SetInt("MPX_H3_BOARD_DIALOGUE1", -1);
			Stats::SetInt("MPX_H3_BOARD_DIALOGUE2", -1);
			Stats::SetInt("MPX_H3_VEHICLESUSED", -1);
			Stats::SetBool("MPX_AWD_FIRST_TIME1", true);
			Stats::SetBool("MPX_AWD_FIRST_TIME2", true);
			Stats::SetBool("MPX_AWD_FIRST_TIME3", true);
			Stats::SetBool("MPX_AWD_FIRST_TIME4", true);
			Stats::SetBool("MPX_AWD_FIRST_TIME5", true);
			Stats::SetBool("MPX_AWD_FIRST_TIME6", true);
			Stats::SetBool("MPX_AWD_ALL_IN_ORDER", true);
			Stats::SetBool("MPX_AWD_SUPPORTING_ROLE", true);
			Stats::SetBool("MPX_AWD_LEADER", true);
			Stats::SetBool("MPX_AWD_ODD_JOBS", true);
			Stats::SetBool("MPX_AWD_SURVIVALIST", true);
			Stats::SetBool("MPX_AWD_SCOPEOUT", true);
			Stats::SetBool("MPX_AWD_CREWEDUP", true);
			Stats::SetBool("MPX_AWD_MOVINGON", true);
			Stats::SetBool("MPX_AWD_PROMOCAMP", true);
			Stats::SetBool("MPX_AWD_GUNMAN", true);
			Stats::SetBool("MPX_AWD_SMASHNGRAB", true);
			Stats::SetBool("MPX_AWD_INPLAINSI", true);
			Stats::SetBool("MPX_AWD_UNDETECTED", true);
			Stats::SetBool("MPX_AWD_ALLROUND", true);
			Stats::SetBool("MPX_AWD_ELITETHEIF", true);
			Stats::SetBool("MPX_AWD_PRO", true);
			Stats::SetBool("MPX_AWD_SUPPORTACT", true);
			Stats::SetBool("MPX_AWD_SHAFTED", true);
			Stats::SetBool("MPX_AWD_COLLECTOR", true);
			Stats::SetBool("MPX_AWD_DEADEYE", true);
			Stats::SetBool("MPX_AWD_PISTOLSATDAWN", true);
			Stats::SetBool("MPX_AWD_TRAFFICAVOI", true);
			Stats::SetBool("MPX_AWD_CANTCATCHBRA", true);
			Stats::SetBool("MPX_AWD_WIZHARD", true);
			Stats::SetBool("MPX_AWD_APEESCAPE", true);
			Stats::SetBool("MPX_AWD_MONKEYKIND", true);
			Stats::SetBool("MPX_AWD_AQUAAPE", true);
			Stats::SetBool("MPX_AWD_KEEPFAITH", true);
			Stats::SetBool("MPX_AWD_TRUELOVE", true);
			Stats::SetBool("MPX_AWD_NEMESIS", true);
			Stats::SetBool("MPX_AWD_FRIENDZONED", true);
			Stats::SetBool("MPX_VCM_FLOW_CS_RSC_SEEN", true);
			Stats::SetBool("MPX_VCM_FLOW_CS_BWL_SEEN", true);
			Stats::SetBool("MPX_VCM_FLOW_CS_MTG_SEEN", true);
			Stats::SetBool("MPX_VCM_FLOW_CS_OIL_SEEN", true);
			Stats::SetBool("MPX_VCM_FLOW_CS_DEF_SEEN", true);
			Stats::SetBool("MPX_VCM_FLOW_CS_FIN_SEEN", true);
			Stats::SetBool("MPX_CAS_VEHICLE_REWARD", false);
			Stats::SetBool("MPX_HELP_FURIA", true);
			Stats::SetBool("MPX_HELP_MINITAN", true);
			Stats::SetBool("MPX_HELP_YOSEMITE2", true);
			Stats::SetBool("MPX_HELP_ZHABA", true);
			Stats::SetBool("MPX_HELP_IMORGEN", true);
			Stats::SetBool("MPX_HELP_SULTAN2", true);
			Stats::SetBool("MPX_HELP_VAGRANT", true);
			Stats::SetBool("MPX_HELP_VSTR", true);
			Stats::SetBool("MPX_HELP_STRYDER", true);
			Stats::SetBool("MPX_HELP_SUGOI", true);
			Stats::SetBool("MPX_HELP_KANJO", true);
			Stats::SetBool("MPX_HELP_FORMULA", true);
			Stats::SetBool("MPX_HELP_FORMULA2", true);
			Stats::SetBool("MPX_HELP_JB7002", true);

			Stats::SetPackedBool(27184, TRUE); // Oil Barrels (Invade and Persuade II) (Arcade Trophy)
			Stats::SetPackedBool(27185, TRUE);
			Stats::SetPackedBool(27186, TRUE);
			Stats::SetPackedBool(27187, TRUE);
			Stats::SetPackedBool(27188, TRUE);
			Stats::SetPackedBool(27189, TRUE); // Spray Paint (Street Crimes: Gang Wars Edition) (Arcade Trophy)
			Stats::SetPackedBool(27190, TRUE);
			Stats::SetPackedBool(27191, TRUE);
			Stats::SetPackedBool(27192, TRUE);
			Stats::SetPackedBool(27193, TRUE);
			Stats::SetPackedBool(27247, TRUE); // Madam Nazar (Arcade Trophy)
			Stats::SetPackedBool(28176, TRUE); // Plushie Grindy Tee (incl. Arcade & Penthouse Decoration)
			Stats::SetPackedBool(28177, TRUE); // Plushie Saki Tee (incl. Arcade & Penthouse Decoration)
			Stats::SetPackedBool(28178, TRUE); // Plushie Humpy Tee (incl. Arcade & Penthouse Decoration)
			Stats::SetPackedBool(28179, TRUE); // Plushie Smoker Tee (incl. Arcade & Penthouse Decoration)
			Stats::SetPackedBool(28180, TRUE); // Plushie [removed]ie Tee (incl. Arcade & Penthouse Decoration)
			Stats::SetPackedBool(28181, TRUE); // Plushie Muffy Tee (incl. Arcade & Penthouse Decoration)
			Stats::SetPackedBool(28182, TRUE); // Plushie Wasabi Kitty Tee (incl. Arcade & Penthouse Decoration)
			Stats::SetPackedBool(28183, TRUE); // Plushie Princess Tee (incl. Arcade & Penthouse Decoration)
			Stats::SetPackedBool(28184, TRUE); // Plushie Master Tee (incl. Arcade & Penthouse Decoration)

			// Los Santos Summer Special
			Stats::SetBool("MPX_AWD_KINGOFQUB3D", true);
			Stats::SetBool("MPX_AWD_QUBISM", true);
			Stats::SetBool("MPX_AWD_QUIBITS", true);
			Stats::SetBool("MPX_AWD_GODOFQUB3D", true);
			Stats::SetBool("MPX_AWD_GOFOR11TH", true);
			Stats::SetBool("MPX_AWD_ELEVENELEVEN", true);

			// Cayo Perico Awards
			Stats::SetBool("MPX_AWD_INTELGATHER", true);
			Stats::SetBool("MPX_AWD_COMPOUNDINFILT", true);
			Stats::SetBool("MPX_AWD_LOOT_FINDER", true);
			Stats::SetBool("MPX_AWD_MAX_DISRUPT", true);
			Stats::SetBool("MPX_AWD_THE_ISLAND_HEIST", true);
			Stats::SetBool("MPX_AWD_GOING_ALONE", true);
			Stats::SetBool("MPX_AWD_TEAM_WORK", true);
			Stats::SetBool("MPX_AWD_MIXING_UP", true);
			Stats::SetBool("MPX_AWD_PRO_THIEF", true);
			Stats::SetBool("MPX_AWD_CAT_BURGLAR", true);
			Stats::SetBool("MPX_AWD_ONE_OF_THEM", true);
			Stats::SetBool("MPX_AWD_GOLDEN_GUN", true);
			Stats::SetBool("MPX_AWD_ELITE_THIEF", true);
			Stats::SetBool("MPX_AWD_PROFESSIONAL", true);
			Stats::SetBool("MPX_AWD_HELPING_OUT", true);
			Stats::SetBool("MPX_AWD_COURIER", true);
			Stats::SetBool("MPX_AWD_PARTY_VIBES", true);
			Stats::SetBool("MPX_AWD_HELPING_HAND", true);
			Stats::SetInt("MPX_AWD_SUNSET", 60);
			Stats::SetInt("MPX_AWD_TREASURE_HUNTER", 60);
			Stats::SetInt("MPX_AWD_WRECK_DIVING", 1000000);
			Stats::SetInt("MPX_AWD_KEINEMUSIK", 60);
			Stats::SetInt("MPX_AWD_PALMS_TRAX", 60);
			Stats::SetInt("MPX_AWD_MOODYMANN", 60);
			Stats::SetInt("MPX_AWD_FILL_YOUR_BAGS", 20000000);
			Stats::SetInt("MPX_AWD_WELL_PREPARED", 50);

			// Los Santos Tuners Awards
			Stats::SetBool("MPX_AWD_CAR_CLUB", true);
			Stats::SetBool("MPX_AWD_PRO_CAR_EXPORT", true);
			Stats::SetBool("MPX_AWD_UNION_DEPOSITORY", true);
			Stats::SetBool("MPX_AWD_MILITARY_CONVOY", true);
			Stats::SetBool("MPX_AWD_FLEECA_BANK", true);
			Stats::SetBool("MPX_AWD_FREIGHT_TRAIN", true);
			Stats::SetBool("MPX_AWD_BOLINGBROKE_ASS", true);
			Stats::SetBool("MPX_AWD_IAA_RAID", true);
			Stats::SetBool("MPX_AWD_METH_JOB", true);
			Stats::SetBool("MPX_AWD_BUNKER_RAID", true);
			Stats::SetBool("MPX_AWD_STRAIGHT_TO_VIDEO", true);
			Stats::SetBool("MPX_AWD_MONKEY_C_MONKEY_DO", true);
			Stats::SetBool("MPX_AWD_TRAINED_TO_KILL", true);
			Stats::SetBool("MPX_AWD_DIRECTOR", true);
			Stats::SetInt("MPX_AWD_CAR_CLUB_MEM", 100);
			Stats::SetInt("MPX_AWD_SPRINTRACER", 50);
			Stats::SetInt("MPX_AWD_STREETRACER", 50);
			Stats::SetInt("MPX_AWD_PURSUITRACER", 50);
			Stats::SetInt("MPX_AWD_TEST_CAR", 240);
			Stats::SetInt("MPX_AWD_AUTO_SHOP", 50);
			Stats::SetInt("MPX_AWD_CAR_EXPORT", 100);
			Stats::SetInt("MPX_AWD_GROUNDWORK", 40);
			Stats::SetInt("MPX_AWD_ROBBERY_CONTRACT", 100);
			Stats::SetInt("MPX_AWD_FACES_OF_DEATH", 50);

			//The Contract Awards
			Stats::SetBool("MPX_AWD_TEEING_OFF", true);
			Stats::SetBool("MPX_AWD_PARTY_NIGHT", true);
			Stats::SetBool("MPX_AWD_BILLIONAIRE_GAMES", true);
			Stats::SetBool("MPX_AWD_HOOD_PASS", true);
			Stats::SetBool("MPX_AWD_STUDIO_TOUR", true);
			Stats::SetBool("MPX_AWD_DONT_MESS_DRE", true);
			Stats::SetBool("MPX_AWD_BACKUP", true);
			Stats::SetBool("MPX_AWD_SHORTFRANK_1", true);
			Stats::SetBool("MPX_AWD_SHORTFRANK_2", true);
			Stats::SetBool("MPX_AWD_SHORTFRANK_3", true);
			Stats::SetBool("MPX_AWD_CONTR_KILLER", true);
			Stats::SetBool("MPX_AWD_DOGS_BEST_FRIEND", true);
			Stats::SetBool("MPX_AWD_MUSIC_STUDIO", true);
			Stats::SetBool("MPX_AWD_SHORTLAMAR_1", true);
			Stats::SetBool("MPX_AWD_SHORTLAMAR_2", true);
			Stats::SetBool("MPX_AWD_SHORTLAMAR_3", true);
			Stats::SetInt("MPX_AWD_CONTRACTOR", 50);
			Stats::SetInt("MPX_AWD_COLD_CALLER", 50);
			Stats::SetInt("MPX_AWD_PRODUCER", 60);
			Stats::SetInt("MPX_FIXERTELEPHONEHITSCOMPL", 10);
			Stats::SetInt("MPX_PAYPHONE_BONUS_KILL_METHOD", 10);
			Stats::SetInt("MPX_FIXER_COUNT", 501);
			Stats::SetInt("MPX_FIXER_SC_VEH_RECOVERED", 501);
			Stats::SetInt("MPX_FIXER_SC_VAL_RECOVERED", 501);
			Stats::SetInt("MPX_FIXER_SC_GANG_TERMINATED", 501);
			Stats::SetInt("MPX_FIXER_SC_VIP_RESCUED", 501);
			Stats::SetInt("MPX_FIXER_SC_ASSETS_PROTECTED", 501);
			Stats::SetInt("MPX_FIXER_SC_EQ_DESTROYED", 501);
			Stats::SetInt("MPX_FIXER_EARNINGS", 300000);
			Stats::SetBool("MPX_BS_FRANKLIN_DIALOGUE_0", true);
			Stats::SetBool("MPX_BS_FRANKLIN_DIALOGUE_1", true);
			Stats::SetBool("MPX_BS_FRANKLIN_DIALOGUE_2", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_SETUP", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_STRAND", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_PARTY", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_PARTY_2", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_PARTY_F", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_BILL", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_BILL_2", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_BILL_F", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_HOOD", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_HOOD_2", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_HOOD_F", true);


			// Los Santos Drug Wars Awards
			Stats::SetBool("MPX_AWD_ACELIQUOR", true);
			Stats::SetBool("MPX_AWD_TRUCKAMBUSH", true);
			Stats::SetBool("MPX_AWD_LOSTCAMPREV", true);
			Stats::SetBool("MPX_AWD_ACIDTRIP", true);
			Stats::SetBool("MPX_AWD_HIPPYRIVALS", true);
			Stats::SetBool("MPX_AWD_TRAINCRASH", true);
			Stats::SetBool("MPX_AWD_BACKUPB", true);
			Stats::SetBool("MPX_AWD_GETSTARTED", true);
			Stats::SetBool("MPX_AWD_CHEMREACTION", true);
			Stats::SetBool("MPX_AWD_WAREHODEFEND", true);
			Stats::SetBool("MPX_AWD_ATTACKINVEST", true);
			Stats::SetBool("MPX_AWD_RESCUECOOK", true);
			Stats::SetBool("MPX_AWD_DRUGTRIPREHAB", true);
			Stats::SetBool("MPX_AWD_CARGOPLANE", true);
			Stats::SetBool("MPX_AWD_BACKUPB2", true);
			Stats::SetBool("MPX_AWD_TAXISTAR", true);
			Stats::SetInt("MPX_AWD_RUNRABBITRUN", 5);
			Stats::SetInt("MPX_AWD_CALLME", 50);
			Stats::SetInt("MPX_AWD_CHEMCOMPOUNDS", 50);
			Stats::SetInt("MPX_AWD_STASHHORAID", 50);
			Stats::SetInt("MPX_AWD_DEADDROP", 50);
			Stats::SetInt("MPX_AWD_GOODSAMARITAN", 5);
			Stats::SetInt("MPX_AWD_OWNWORSTENEMY", 60);
			Stats::SetInt("MPX_AWD_TAXIDRIVER", 50);

			// The Chop Shop Awards
			Stats::SetBool("MPX_AWD_ACELIQUOR", true);
			Stats::SetBool("MPX_AWD_TRUCKAMBUSH", true);
			Stats::SetBool("MPX_AWD_LOSTCAMPREV", true);
			Stats::SetBool("MPX_AWD_ACIDTRIP", true);
			Stats::SetBool("MPX_AWD_HIPPYRIVALS", true);
			Stats::SetBool("MPX_AWD_TRAINCRASH", true);
			Stats::SetBool("MPX_AWD_BACKUPB", true);
			Stats::SetBool("MPX_AWD_GETSTARTED", true);
			Stats::SetBool("MPX_AWD_CHEMREACTION", true);
			Stats::SetBool("MPX_AWD_WAREHODEFEND", true);
			Stats::SetBool("MPX_AWD_ATTACKINVEST", true);
			Stats::SetBool("MPX_AWD_RESCUECOOK", true);
			Stats::SetBool("MPX_AWD_DRUGTRIPREHAB", true);
			Stats::SetBool("MPX_AWD_CARGOPLANE", true);
			Stats::SetBool("MPX_AWD_BACKUPB2", true);
			Stats::SetBool("MPX_AWD_TAXISTAR", true);
			Stats::SetInt("MPX_AWD_RUNRABBITRUN", 5);
			Stats::SetInt("MPX_AWD_CALLME", 50);
			Stats::SetInt("MPX_AWD_CHEMCOMPOUNDS", 50);
			Stats::SetInt("MPX_AWD_STASHHORAID", 50);
			Stats::SetInt("MPX_AWD_DEADDROP", 50);
			Stats::SetInt("MPX_AWD_GOODSAMARITAN", 5);
			Stats::SetInt("MPX_AWD_OWNWORSTENEMY", 60);
			Stats::SetInt("MPX_AWD_TAXIDRIVER", 50);


			// Bottom Dollar Bounties Awards
			Stats::SetBool("MPX_AWD_HIVALBOUNT1", true);
			Stats::SetBool("MPX_AWD_HIVALBOUNT2", true);
			Stats::SetBool("MPX_AWD_HIVALBOUNT3", true);
			Stats::SetBool("MPX_AWD_HIVALBOUNT4", true);
			Stats::SetBool("MPX_AWD_HIVALBOUNT5", true);
			Stats::SetBool("MPX_AWD_HIVALBOUNT6", true);
			Stats::SetBool("MPX_AWD_ASSONATTACKSWIN", true);
			Stats::SetBool("MPX_AWD_ASSONDEFENDWIN", true);
			Stats::SetBool("MPX_AWD_DISEASECONTROL", true);
			Stats::SetInt("MPX_AWD_BOUNTIES", 50);
			Stats::SetInt("MPX_AWD_STANBOUNTIES", 50);
			Stats::SetInt("MPX_AWD_BOUNTEARNS", 5000000);
			Stats::SetInt("MPX_AWD_BAILOFFICSTAFF", 1000000);
			Stats::SetInt("MPX_AWD_DISPATCHWORK", 50);
			Stats::SetInt("MPX_AWD_PIZZATHIS", 50);
			Stats::SetInt("MPX_AWD_ASSONBONUSOBJ", 20);
			Stats::SetInt("MPX_AWD_ASSONHARDDRIVE", 50);
			Stats::SetInt("MPX_AWD_FROSTBITE", 15);


			// Agent Sabotage Awards
			Stats::SetBool("MPX_AWD_FINEART", true);
			Stats::SetBool("MPX_AWD_BRUTEFORCE", true);
			Stats::SetBool("MPX_AWD_PROJECTBREAK", true);
			Stats::SetBool("MPX_AWD_BLACKBOXFILE", true);
			Stats::SetBool("MPX_AWD_BONUSPOINTS", true);
			Stats::SetBool("MPX_AWD_UPRUNNING", true);
			Stats::SetBool("MPX_AWD_MOGUL", true);
			Stats::SetBool("MPX_AWD_INTEL", true);
			Stats::SetBool("MPX_AWD_IRONMULE", true);
			Stats::SetBool("MPX_AWD_AMMUNITION", true);
			Stats::SetBool("MPX_AWD_DIRDELIVERY", true);
			Stats::SetBool("MPX_AWD_TITANJOB", true);
			Stats::SetBool("MPX_AWD_PERMANENTCON", true);
			Stats::SetInt("MPX_AWD_DARNELLBROSINC", 50);
			Stats::SetInt("MPX_AWD_GET_READY", 50);
			Stats::SetInt("MPX_AWD_CASHINHAND", 5000000);
			Stats::SetInt("MPX_AWD_BROTHERLYLOVE", 50000);
			Stats::SetInt("MPX_AWD_NOTOUTDPT", 5);
			Stats::SetInt("MPX_AWD_FULSTOCKED", 8);
			Stats::SetInt("MPX_AWD_ARMSINARMS", 50);


			// Money Fronts

			Stats::SetInt("MPX_AWD_JUNKSEARCH", 50);
			Stats::SetInt("MPX_AWD_CASASS", 50);
			Stats::SetInt("MPX_AWD_LOCKUPINT", 50);
			Stats::SetInt("MPX_AWD_ESCORTFLATBED", 50);
			Stats::SetInt("MPX_AWD_LEFT4DEAD", 50);
			Stats::SetInt("MPX_AWD_TRACKER", 50);
			Stats::SetInt("MPX_AWD_CLEARCOMP", 50);
			Stats::SetInt("MPX_AWD_CASHBONUS", 50);
			Stats::SetInt("MPX_AWD_BUSINEXPAND", 50);
			Stats::SetInt("MPX_AWD_LOSTPRODUC", 50);
			Stats::SetInt("MPX_AWD_EMERGENCYSERV", 50);
			Stats::SetInt("MPX_AWD_SECUREDEL", 50);
			Stats::SetInt("MPX_AWD_TWOOFAKIND", 50);
			Stats::SetInt("MPX_AWD_DOGSDINNER", 50);
			Stats::SetInt("MPX_AWD_HEATAROUDC", 50);
			Stats::SetInt("MPX_AWD_TYCOON", 50);
			Stats::SetInt("MPX_AWD_CASHCLEAN", 50);
			Stats::SetInt("MPX_AWD_MEDICOURI", 50);
			Stats::SetInt("MPX_AWD_TRANSPORT", 50);
			Stats::SetInt("MPX_AWD_SUNBURNED", 50);
			Stats::SetInt("MPX_AWD_UNDEADPARTY", 50);


			// A Safehouse in the Hills awards

			Stats::SetBool("MPX_AWD_NEGATIVEPRESS", true);
			Stats::SetBool("MPX_AWD_USESELFDRIVINGVEH", true);
			Stats::SetBool("MPX_AWD_LIONSDEN", true);
			Stats::SetBool("MPX_AWD_TAKEOUTLEADEXPLO", true);
			Stats::SetBool("MPX_AWD_SUBWAYTRAIN", true);
			Stats::SetBool("MPX_AWD_SUBWAYSTUNTJUMP", true);
			Stats::SetBool("MPX_AWD_TRASHCOLLECTORS", true);
			Stats::SetBool("MPX_AWD_ALLTRASHCOLLECTED", true);
			Stats::SetBool("MPX_AWD_FIBFINALE", true);
			Stats::SetBool("MPX_AWD_HELPER", true);
			Stats::SetBool("MPX_AWD_FIBFINALECHALLENGE", true);
			Stats::SetBool("MPX_AWD_MANSIONDEFEND", true);
			Stats::SetBool("MPX_AWD_DEFLOWMOTIONKILLS", true);

			Stats::SetInt("MPX_AWD_TAXIDESTRUCTION", 20);
			Stats::SetBool("MPX_AWD_YOUVEMADEIT", true);
			Stats::SetBool("MPX_AWD_ANIMALLOVER", true);
			Stats::SetInt("MPX_AWD_YOGA", 50);
			Stats::SetInt("MPX_AWD_JUICEDUP", 100);
			Stats::SetInt("MPX_AWD_MANADVERSARYDEFUSE", 10);
			Stats::SetBool("MPX_AWD_MANSADVERSARYWIN", true);
			Stats::SetInt("MPX_AWD_FIREFIGHTER", 20);
			Stats::SetInt("MPX_AWD_ALPHAMAIL", 20);
			Stats::SetInt("MPX_AWD_LOSSANTOSMETEOR", 20);
			Stats::SetBool("MPX_AWD_ONTHECLOCK", true);


			//Character Stats
			Stats::SetInt("MPX_AWD_MENTALSTATE_TO_NORMAL", 25);
			Stats::SetInt("MPX_AWD_TRADE_IN_YOUR_PROPERTY", 25);
			Stats::SetInt("MPX_AWD_100_KILLS_PISTOL", 100);
			Stats::SetInt("MPX_AWD_100_KILLS_SNIPER", 100);
			Stats::SetInt("MPX_AWD_50_KILLS_GRENADES", 50);
			Stats::SetInt("MPX_AWD_100_KILLS_SHOTGUN", 100);
			Stats::SetInt("MPX_AWD_100_KILLS_SMG", 100);
			Stats::SetInt("MPX_AWD_50_KILLS_ROCKETLAUNCH", 50);
			Stats::SetInt("MPX_AWD_25_KILLS_STICKYBOMBS", 50);
			Stats::SetInt("MPX_AWD_20_KILLS_MELEE", 20);
			Stats::SetInt("MPX_AWD_50_VEHICLES_BLOWNUP", 50);
			Stats::SetInt("MPX_AWD_ENEMYDRIVEBYKILLS", 10);
			Stats::SetInt("MPX_AWD_VEHICLES_JACKEDR", 100);
			Stats::SetInt("MPX_AWD_HOLD_UP_SHOPS", 19);
			Stats::SetInt("MPX_AWD_COPS_KILLED", 200);
			Stats::SetInt("MPX_PISTOL_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_CMBTPISTOL_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_APPISTOL_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_MICROSMG_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_SMG_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_ASLTRIFLE_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_CRBNRIFLE_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_ADVRIFLE_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_MG_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_CMBTMG_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_ASLTSMG_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_PUMP_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_ASLTSHTGN_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_SNIPERRFL_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_HVYSNIPER_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_GRNLAUNCH_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_RPG_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_MINIGUNS_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_SAWNOFF_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_CHAR_FM_CARMOD_1_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CARMOD_2_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CARMOD_3_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CARMOD_4_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CARMOD_5_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CARMOD_6_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CARMOD_7_UNLCK", -1);
			Stats::SetInt("MPX_AWD_FMRALLYWONDRIVE", 1);
			Stats::SetInt("MPX_AWD_FMRALLYWONNAV", 1);
			Stats::SetInt("MPX_AWD_FMWINSEARACE", 1);
			Stats::SetInt("MPX_AWD_FMWINAIRRACE", 1);
			Stats::SetInt("MPX_NUMBER_TURBO_STARTS_IN_RACE", 50);
			Stats::SetInt("MPX_USJS_COMPLETED", 50);
			Stats::SetInt("MPX_RACES_WON", 50);
			Stats::SetInt("MPPLY_TOTAL_RACES_WON", 50); // Global stat, no MPx prefix
			Stats::SetInt("MPX_AWD_FM_RACES_FASTEST_LAP", 50);
			Stats::SetInt("MPX_NUMBER_SLIPSTREAMS_IN_RACE", 100);
			Stats::SetInt("MPX_PLAYER_HEADSHOTS", 500);
		}
	};

	class UnlockEverything : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			for (int i = 110; i <= 113; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 115; i <= 115; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 124; i <= 124; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 129; i <= 129; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 135; i <= 137; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 3593; i <= 3599; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 3604; i <= 3605; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 3608; i <= 3609; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 3615; i <= 3616; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 3750; i <= 3750; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 3765; i <= 3769; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 3770; i <= 3781; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 3783; i <= 3802; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 4247; i <= 4269; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 4300; i <= 4327; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 4333; i <= 4335; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 7467; i <= 7495; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 7515; i <= 7528; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 7551; i <= 7551; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 7595; i <= 7601; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 9362; i <= 9385; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 9426; i <= 9440; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 9443; i <= 9443; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 9461; i <= 9481; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15381; i <= 15382; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15388; i <= 15423; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15425; i <= 15439; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15447; i <= 15474; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15491; i <= 15499; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15552; i <= 15560; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 18099; i <= 18099; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 18116; i <= 18118; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 18121; i <= 18125; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 18134; i <= 18137; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 22124; i <= 22132; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 22137; i <= 22139; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 22147; i <= 22178; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 24963; i <= 25000; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 25002; i <= 25002; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 25004; i <= 25006; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 25008; i <= 25009; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 25018; i <= 25099; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 25101; i <= 25109; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 25111; i <= 25134; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 25136; i <= 25179; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 25181; i <= 25237; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 25244; i <= 25400; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 25406; i <= 25406; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 25407; i <= 25511; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 25516; i <= 25516; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 25520; i <= 25521; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 26811; i <= 26964; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 26968; i <= 27088; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 27109; i <= 27115; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 27120; i <= 27145; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 27147; i <= 27182; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 27184; i <= 27213; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 27247; i <= 27247; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 28099; i <= 28148; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 28158; i <= 28158; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 28171; i <= 28191; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 28194; i <= 28196; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 28197; i <= 28223; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 28224; i <= 28227; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 28229; i <= 28249; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 28254; i <= 28255; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 28272; i <= 28272; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 28319; i <= 28321; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 30230; i <= 30251; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 30254; i <= 30295; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 30524; i <= 30557; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 30563; i <= 30693; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 30699; i <= 30704; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31708; i <= 31714; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31736; i <= 31736; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31755; i <= 31755; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31760; i <= 31764; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31766; i <= 31777; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31779; i <= 31796; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31805; i <= 31808; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31810; i <= 31824; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31826; i <= 31858; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31860; i <= 31863; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31865; i <= 31868; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31870; i <= 31928; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31930; i <= 31933; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31935; i <= 31938; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31940; i <= 31943; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31945; i <= 31948; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31950; i <= 31953; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31955; i <= 31958; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31960; i <= 31963; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31965; i <= 31968; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31970; i <= 31973; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31975; i <= 31978; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31980; i <= 31983; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31985; i <= 31988; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31990; i <= 31993; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 31995; i <= 31998; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32000; i <= 32003; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32005; i <= 32008; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32010; i <= 32013; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32015; i <= 32018; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32020; i <= 32023; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32025; i <= 32028; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32030; i <= 32033; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32035; i <= 32038; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32040; i <= 32043; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32045; i <= 32048; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32050; i <= 32053; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32055; i <= 32058; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32060; i <= 32063; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32065; i <= 32074; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32084; i <= 32084; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32094; i <= 32094; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32104; i <= 32104; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32114; i <= 32114; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32124; i <= 32124; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32134; i <= 32134; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32144; i <= 32144; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32154; i <= 32154; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32164; i <= 32164; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32174; i <= 32174; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32224; i <= 32224; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32273; i <= 32273; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32275; i <= 32275; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32287; i <= 32291; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32295; i <= 32311; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32314; i <= 32316; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32319; i <= 32323; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32366; i <= 32366; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32407; i <= 32408; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 32409; i <= 32409; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 34138; i <= 34139; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 34143; i <= 34144; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 34150; i <= 34151; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 34262; i <= 34361; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 34372; i <= 34375; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 34378; i <= 34411; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 34415; i <= 34510; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 34703; i <= 34705; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 34730; i <= 34737; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 34761; i <= 34761; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 36630; i <= 36654; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 36699; i <= 36770; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 36774; i <= 36788; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 36809; i <= 36809; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 36825; i <= 36825; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 41316; i <= 41325; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 41593; i <= 41593; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 41656; i <= 41659; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 41671; i <= 41671; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 41674; i <= 41675; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 41720; i <= 41805; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 41806; i <= 41810; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 41884; i <= 41980; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 41994; i <= 41996; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42016; i <= 42016; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42052; i <= 42054; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42055; i <= 42058; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42062; i <= 42063; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42068; i <= 42069; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42111; i <= 42111; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42119; i <= 42123; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42125; i <= 42125; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42128; i <= 42146; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42148; i <= 42149; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42152; i <= 42190; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42191; i <= 42216; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42233; i <= 42234; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42239; i <= 42242; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42244; i <= 42247; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42249; i <= 42249; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42257; i <= 42268; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42280; i <= 42284; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42286; i <= 42289; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42294; i <= 42297; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 51189; i <= 51189; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 51196; i <= 51197; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 51215; i <= 51258; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 51273; i <= 51275; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 51283; i <= 51283; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 51285; i <= 51285; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 51302; i <= 51337; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 54569; i <= 54569; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 54570; i <= 54570; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 54572; i <= 54579; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 54580; i <= 54593; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 54594; i <= 54611; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 54614; i <= 54634; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 54635; i <= 54635; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 54637; i <= 54638; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 54651; i <= 54651; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 54664; i <= 54664; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 54682; i <= 54707; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 54711; i <= 54713; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 3483; i <= 3492; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 6082; i <= 6083; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 6097; i <= 6097; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 6169; i <= 6169; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 6303; i <= 6304; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15708; i <= 15708; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15710; i <= 15710; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15717; i <= 15717; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15720; i <= 15720; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15724; i <= 15724; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15728; i <= 15728; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15730; i <= 15730; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15732; i <= 15732; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15737; i <= 15737; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15887; i <= 15887; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15894; i <= 15894; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 28393; i <= 28416; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 28447; i <= 28451; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 28452; i <= 28478; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 30355; i <= 30361; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 30407; i <= 30410; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 30418; i <= 30422; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 41273; i <= 41284; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 3496; i <= 3505; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 6091; i <= 6092; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 6106; i <= 6106; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 6181; i <= 6181; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 6316; i <= 6317; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15719; i <= 15719; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15721; i <= 15721; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15728; i <= 15728; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15731; i <= 15731; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15735; i <= 15735; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15739; i <= 15739; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15741; i <= 15741; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15743; i <= 15743; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15748; i <= 15748; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15898; i <= 15898; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 22108; i <= 22108; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 15905; i <= 15905; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 28404; i <= 28427; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 28458; i <= 28462; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 30418; i <= 30421; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 28463; i <= 28478; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 30355; i <= 30372; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 30429; i <= 30433; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 41285; i <= 41296; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetPackedInt(7315, 6);
			Stats::SetPackedInt(18981, 4);
			Stats::SetPackedInt(18982, 3);
			Stats::SetPackedInt(18983, 3);
			Stats::SetPackedInt(18984, 3);
			Stats::SetPackedInt(18985, 3);
			Stats::SetPackedInt(18986, 3);
			Stats::SetPackedInt(22050, 5);
			Stats::SetPackedInt(22051, 50);
			Stats::SetPackedInt(22052, 100);
			Stats::SetPackedInt(22053, 20);
			Stats::SetPackedInt(22054, 80);
			Stats::SetPackedInt(22055, 60);
			Stats::SetPackedInt(22056, 40);
			Stats::SetPackedInt(22057, 10);
			Stats::SetPackedInt(22058, 20);
			Stats::SetPackedInt(22063, 20);
			Stats::SetPackedInt(41237, 10);
			Stats::SetPackedInt(3032, 100);
			Stats::SetPackedInt(24946, 100);
			Stats::SetPackedInt(24947, 100);
			Stats::SetPackedInt(24948, 100);
			Stats::SetInt("MPPLY_GANGOPS_LOYALTY2", -1);
			Stats::SetInt("MPPLY_GANGOPS_LOYALTY3", -1);
			Stats::SetInt("MPPLY_GANGOPS_CRIMMASMD2", -1);
			Stats::SetInt("MPPLY_GANGOPS_CRIMMASMD3", -1);
			Stats::SetInt("MPPLY_GANGOPS_SUPPORT", -1);
			Stats::SetInt("MPPLY_GANGOPS_ALLINORDER", -1);
			Stats::SetInt("MPPLY_GANGOPS_LOYALTY", -1);
			Stats::SetInt("MPPLY_GANGOPS_CRIMMASMD", -1);
			Stats::SetInt("MPPLY_CREW_NO_HEISTS_0", 2);
			Stats::SetInt("MPPLY_CREW_NO_HEISTS_1", 5);
			Stats::SetInt("MPPLY_CREW_NO_HEISTS_2", 5);
			Stats::SetInt("MPPLY_CREW_NO_HEISTS_3", 5);
			Stats::SetInt("MPPLY_CREW_NO_HEISTS_4", 5);
			Stats::SetInt("MPX_FM_ACT_PHN", -1);
			Stats::SetInt("MPX_FM_ACT_PH2", -1);
			Stats::SetInt("MPX_FM_ACT_PH3", -1);
			Stats::SetInt("MPX_FM_ACT_PH4", -1);
			Stats::SetInt("MPX_FM_ACT_PH5", -1);
			Stats::SetInt("MPX_FM_VEH_TX1", -1);
			Stats::SetInt("MPX_FM_ACT_PH6", -1);
			Stats::SetInt("MPX_FM_ACT_PH7", -1);
			Stats::SetInt("MPX_FM_ACT_PH8", -1);
			Stats::SetInt("MPX_FM_ACT_PH9", -1);
			Stats::SetInt("MPX_FM_ACT_PH10", -1);
			Stats::SetInt("MPX_FM_ACT_PH11", -1);
			Stats::SetInt("MPX_FM_ACT_PH12", -1);
			Stats::SetInt("MPX_FM_CUT_DONE", -1);
			Stats::SetInt("MPX_FM_CUT_DONE_2", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES0", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES1", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES2", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES3", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES4", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES5", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES6", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES7", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES8", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES9", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES10", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES11", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES12", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES13", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES14", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES15", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES16", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES17", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES18", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES19", -1);
			Stats::SetInt("MPPLY_XMASLIVERIES20", -1);
			Stats::SetInt("MPPLY_XMAS22CPAINT0", -1);
			Stats::SetInt("MPPLY_XMAS22CPAINT1", -1);
			Stats::SetInt("MPPLY_SUM23WHEELCPAINT0", -1);
			Stats::SetInt("MPPLY_SUM23WHEELCPAINT1", -1);
			Stats::SetInt("MPX_BIKES_EXPLODED", 100);
			Stats::SetInt("MPX_BOATS_EXPLODED", 100);
			Stats::SetInt("MPX_HELIS_EXPLODED", 100);
			Stats::SetInt("MPX_PLANES_EXPLODED", 100);
			Stats::SetInt("MPX_QUADBIKE_EXPLODED", 100);
			Stats::SetInt("MPX_BICYCLE_EXPLODED", 100);
			Stats::SetInt("MPX_SUBMARINE_EXPLODED", 100);
			Stats::SetInt("MPX_TRAIN_EXPLODED", 100);
			Stats::SetInt("MPX_CARS_COPS_EXPLODED", 100);
			Stats::SetInt("MPX_NUMBER_STOLEN_BIKES", 100);
			Stats::SetInt("MPX_NUMBER_STOLEN_BOATS", 100);
			Stats::SetInt("MPX_NUMBER_STOLEN_HELIS", 100);
			Stats::SetInt("MPX_NUMBER_STOLEN_PLANES", 100);
			Stats::SetInt("MPX_NUMBER_STOLEN_QUADBIKES", 100);
			Stats::SetInt("MPX_NUMBER_STOLEN_BICYCLES", 100);
			Stats::SetInt("MPX_NUMBER_STOLEN_SUBMARINES", 100);
			Stats::SetInt("MPX_NO_STOLEN_VEH_SCRIPT", 100);
			Stats::SetInt("MPX_DB_PLAYER_KILLS", 100);
			Stats::SetInt("MPX_HOLDUPS_BITSET", -1);
			Stats::SetInt("MPX_BREATHING_APPAR_BOUGHT", 20);
			Stats::SetInt("MPX_CHAR_ABILITY_1_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_ABILITY_2_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_ABILITY_3_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_WEAP_UNLOCKED", -1);
			Stats::SetInt("MPX_CHAR_WEAP_UNLOCKED2", -1);
			Stats::SetInt("MPX_CHAR_WEAP_ADDON_1_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_WEAP_ADDON_2_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_WEAP_ADDON_3_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_WEAP_ADDON_4_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_UNLOCKED", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_UNLOCKED2", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_UNLOCKED3", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_UNLOCKED4", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_UNLOCKED5", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_UNLOCKED6", -1);
			Stats::SetInt("MPX_GCLUB_FM_AMMO_BOUGHT", 1);
			Stats::SetInt("MPX_CHAR_WEAP_EQUIPPED", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_EQUIPPED", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_EQUIPPED2", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_EQUIPPED3", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_EQUIPPED4", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_EQUIPPED5", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_EQUIPPED6", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_1_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_2_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_3_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_4_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_5_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_6_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_7_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_8_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_9_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_10_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_11_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_12_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_13_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_14_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_15_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_16_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_17_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_18_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_19_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_WEAP_ADDON_20_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_HAIRCUT_1_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_HAIR_UNLCK1", -1);
			Stats::SetInt("MPX_CHAR_HAIR_UNLCK2", -1);
			Stats::SetInt("MPX_CHAR_HAIR_UNLCK3", -1);
			Stats::SetInt("MPX_CHAR_HAIR_UNLCK4", -1);
			Stats::SetInt("MPX_CHAR_HAIR_UNLCK5", -1);
			Stats::SetInt("MPX_CHAR_HAIR_UNLCK6", -1);
			Stats::SetInt("MPX_CHAR_HAIR_UNLCK7", -1);
			Stats::SetInt("MPX_CHAR_HAIR_UNLCK8", -1);
			Stats::SetInt("MPX_CHAR_HAIR_UNLCK9", -1);
			Stats::SetInt("MPX_CHAR_HAIR_UNLCK10", -1);
			Stats::SetInt("MPX_CHAR_HAIR_UNLCK11", -1);
			Stats::SetInt("MPX_CHAR_HAIR_UNLCK12", -1);
			Stats::SetInt("MPX_CHAR_HAIR_UNLCK13", -1);
			Stats::SetInt("MPX_CHAR_HAIR_UNLCK14", -1);
			Stats::SetInt("MPX_CHAR_HAIR_UNLCK15", -1);
			Stats::SetInt("MPX_CHAR_HAIR_UNLCK16", -1);
			Stats::SetInt("MPX_CHAR_HAIR_UNLCK17", -1);
			Stats::SetInt("MPX_CHAR_FM_HEALTH_1_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_HEALTH_2_UNLCK", -1);
			Stats::SetInt("MPX_CRDEADLINE", 5);
			Stats::SetInt("MPX_CHAR_CREWUNLOCK_1_FM_EQUIP", -1);
			Stats::SetInt("MPX_CHAR_CREWUNLOCK_2_FM_EQUIP", -1);
			Stats::SetInt("MPX_CHAR_CREWUNLOCK_3_FM_EQUIP", -1);
			Stats::SetInt("MPX_CHAR_CREWUNLOCK_4_FM_EQUIP", -1);
			Stats::SetInt("MPX_CHAR_CREWUNLOCK_5_FM_EQUIP", -1);
			Stats::SetInt("MPX_CHAR_KIT_1_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_2_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_3_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_4_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_5_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_6_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_7_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_8_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_9_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_10_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_11_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_12_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_13_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_14_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_15_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_16_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_17_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_18_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_19_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_20_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_21_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_22_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_23_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_24_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_25_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_26_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_27_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_28_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_29_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_30_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_31_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_32_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_33_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_34_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_35_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_36_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_37_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_38_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_39_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_40_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_KIT_41_FM_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_ABILITY_1_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_ABILITY_2_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_ABILITY_3_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CLOTHES_1_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CLOTHES_2_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CLOTHES_3_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CLOTHES_4_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CLOTHES_5_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CLOTHES_6_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CLOTHES_7_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CLOTHES_8_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CLOTHES_9_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CLOTHES_10_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CLOTHES_11_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_VEHICLE_1_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_VEHICLE_2_UNLCK", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_0", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_1", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_2", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_3", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_4", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_5", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_6", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_7", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_8", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_9", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_10", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_11", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_12", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_13", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_14", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_15", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_16", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_17", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_18", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_19", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_20", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_21", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_22", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_23", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_24", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_25", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_26", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_27", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_28", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_29", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_30", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_31", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_32", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_33", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_34", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_35", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_36", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_37", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_38", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_39", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_40", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_41", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_42", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_43", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_44", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_45", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_46", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_47", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_48", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_49", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_50", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_51", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_52", -1);
			Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_53", -1);
			Stats::SetBool("MPX_FILM4SHIRTUNLOCK", true);
			Stats::SetBool("MPX_FILM5SHIRTUNLOCK", true);
			Stats::SetBool("MPX_FILM6SHIRTUNLOCK", true);
			Stats::SetBool("MPX_FILM7SHIRTUNLOCK", true);
			Stats::SetBool("MPX_FILM8SHIRTUNLOCK", true);
			Stats::SetBool("MPX_FILM9SHIRTUNLOCK", true);
			Stats::SetBool("ACCOUNTANTSHIRTUNLOCK", true);
			Stats::SetBool("BAHAMAMAMASHIRTUNLOCK", true);
			Stats::SetBool("DRONESHIRTUNLOCK", true);
			Stats::SetBool("GROTTISHIRTUNLOCK", true);
			Stats::SetBool("GOLFSHIRTUNLOCK", true);
			Stats::SetBool("MAISONETTESHIRTUNLOCK", true);
			Stats::SetBool("MANOPAUSESHIRTUNLOCK", true);
			Stats::SetBool("MELTDOWNSHIRTUNLOCK", true);
			Stats::SetBool("PACIFICBLUFFSSHIRTUNLOCK", true);
			Stats::SetBool("PROLAPSSHIRTUNLOCK", true);
			Stats::SetBool("TENNISSHIRTUNLOCK", true);
			Stats::SetBool("TOESHOESSHIRTUNLOCK", true);
			Stats::SetBool("VANILLAUNICORNSHIRTUNLOCK", true);
			Stats::SetBool("MARLOWESHIRTUNLOCK", true);
			Stats::SetBool("CRESTSHIRTUNLOCK", true);
			Stats::SetInt("MPX_RANKAP_UNLK_0", -1);
			Stats::SetInt("MPX_RANKAP_UNLK_1", -1);
			Stats::SetInt("MPX_RANKAP_UNLK_2", -1);
			Stats::SetInt("MPX_RANKAP_UNLK_3", -1);
			Stats::SetInt("MPX_CHAR_CREWUNLOCK_1_UNLCK", -1);
			Stats::SetInt("MPX_PISTOL_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_CMBTPISTOL_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_APPISTOL_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_MICROSMG_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_SMG_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_ASLTSMG_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_CRBNRIFLE_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_ADVRIFLE_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_MG_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_PUMP_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_SAWNOFF_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_HVYSNIPER_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_GRNLAUNCH_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_MINIGUNS_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_SR_WEAPON_BIT_SET", 262143);
			Stats::SetBool("MPX_SR_TIER_1_REWARD", true);
			Stats::SetBool("MPX_SR_INCREASE_THROW_CAP", true);
			Stats::SetBool("MPX_SR_TIER_3_REWARD", true);
			Stats::SetInt("MPX_PILOT_SCHOOL_MEDAL_0", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_MEDAL_1", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_MEDAL_2", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_MEDAL_3", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_MEDAL_4", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_MEDAL_5", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_MEDAL_6", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_MEDAL_7", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_MEDAL_8", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_MEDAL_9", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_LASTMEDAL_0", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_LASTMEDAL_1", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_LASTMEDAL_2", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_LASTMEDAL_3", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_LASTMEDAL_4", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_LASTMEDAL_5", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_LASTMEDAL_6", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_LASTMEDAL_7", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_LASTMEDAL_8", 3);
			Stats::SetInt("MPX_PILOT_SCHOOL_LASTMEDAL_9", 3);
			Stats::SetInt("MPX_CRPILOTSCHOOL", 1);
			Stats::SetInt("MPX_PILOT_CHECKPOINTCOUNT_9", 27);
			Stats::SetBool("MPX_PILOT_ASPASSEDLESSON_0", true);
			Stats::SetBool("MPX_PILOT_ASPASSEDLESSON_1", true);
			Stats::SetBool("MPX_PILOT_ASPASSEDLESSON_2", true);
			Stats::SetBool("MPX_PILOT_ASPASSEDLESSON_3", true);
			Stats::SetBool("MPX_PILOT_ASPASSEDLESSON_4", true);
			Stats::SetBool("MPX_PILOT_ASPASSEDLESSON_5", true);
			Stats::SetBool("MPX_PILOT_ASPASSEDLESSON_6", true);
			Stats::SetBool("MPX_PILOT_ASPASSEDLESSON_7", true);
			Stats::SetBool("MPX_PILOT_ASPASSEDLESSON_8", true);
			Stats::SetBool("MPX_PILOT_ASPASSEDLESSON_9", true);
			Stats::SetInt("MPX_CAR_CLUB_REP", 997430);
			Stats::SetBool("MPPLY_MELEECHLENGECOMPLETED", true);
			Stats::SetBool("MPPLY_HEADSHOTCHLENGECOMPLETED", true);
			Stats::SetBool("MPPLY_NAVYREVOLVERCOMPLETED", true);
			Stats::SetInt("MPX_CHAR_HEIST_1_UNLCK", -1);
			Stats::SetInt("MPX_CRHEIST", 50);
			Stats::SetInt("MPX_CR_BANKHEIST1", 10);
			Stats::SetInt("MPX_CR_COUNTHEIST1", 10);
			Stats::SetInt("MPX_HEIST_COMPLETION", 26);
			Stats::SetInt("MPX_HEIST_TOTAL_TIME", 86400000);
			Stats::SetInt("MPX_HEISTS_ORGANISED", 50);
			Stats::SetInt("MPX_RACES_WON", 50);
			Stats::SetInt("MPX_CHAR_FM_PACKAGE_1_COLLECT", -1);
			Stats::SetInt("MPX_CHAR_FM_PACKAGE_2_COLLECT", -1);
			Stats::SetInt("MPX_CHAR_FM_PACKAGE_3_COLLECT", -1);
			Stats::SetInt("MPX_CHAR_FM_PACKAGE_4_COLLECT", -1);
			Stats::SetInt("MPX_CHAR_FM_PACKAGE_5_COLLECT", -1);
			Stats::SetInt("MPX_CHAR_FM_PACKAGE_6_COLLECT", -1);
			Stats::SetInt("MPX_CHAR_FM_PACKAGE_7_COLLECT", -1);
			Stats::SetInt("MPX_CHAR_FM_PACKAGE_8_COLLECT", -1);
			Stats::SetInt("MPX_CHAR_NO_FM_PACKAGES_COL", -1);
			Stats::SetInt("MPX_CHAR_FM_CARMOD_1_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CARMOD_2_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CARMOD_3_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CARMOD_4_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CARMOD_5_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CARMOD_6_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_CARMOD_7_UNLCK", -1);
			Stats::SetInt("MPX_NUMBER_SLIPSTREAMS_IN_RACE", 110);
			Stats::SetInt("MPX_NUMBER_TURBO_STARTS_IN_RACE", 90);
			Stats::SetInt("MPX_USJS_FOUND", 50);
			Stats::SetInt("MPX_USJS_COMPLETED", 50);
			Stats::SetInt("MPPLY_TIMES_RACE_BEST_LAP", 101);
			Stats::SetInt("MPX_SCRIPT_INCREASE_STAM", 100);
			Stats::SetInt("MPX_SCRIPT_INCREASE_STRN", 100);
			Stats::SetInt("MPX_SCRIPT_INCREASE_FLY", 100);
			Stats::SetInt("MPX_SCRIPT_INCREASE_STL", 100);
			Stats::SetInt("MPX_SCRIPT_INCREASE_LUNG", 100);
			Stats::SetInt("MPX_SCRIPT_INCREASE_DRIV", 100);
			Stats::SetInt("MPX_SCRIPT_INCREASE_SHO", 100);
			Stats::SetInt("MPX_NIGHTCLUB_HOTSPOT_TIME_MS", 40320000);
			Stats::SetInt("MPX_CASINO_DECORATION_GIFT_1", -1);
			Stats::SetInt("MPX_CAS_HEIST_FLOW", -1);
			Stats::SetBool("MPX_COMPLETE_H4_F_USING_VETIR", true);
			Stats::SetBool("MPX_COMPLETE_H4_F_USING_LONGFIN", true);
			Stats::SetBool("MPX_COMPLETE_H4_F_USING_ANNIH", true);
			Stats::SetBool("MPX_COMPLETE_H4_F_USING_ALKONOS", true);
			Stats::SetBool("MPX_COMPLETE_H4_F_USING_PATROLB", true);
			Stats::SetInt("MPX_ARENAWARS_AP_LIFETIME", 5055000);
			Stats::SetInt("MPX_ARENAWARS_AP_TIER", 1000);
			Stats::SetInt("MPX_ARENAWARS_AP", 10040);
			Stats::SetInt("MPX_ARENAWARS_SKILL_LEVEL", 20);
			Stats::SetInt("MPX_ARENAWARS_SP", 210);
			Stats::SetInt("MPX_ARENAWARS_SP_LIFETIME", 210);
			Stats::SetInt("MPX_ARENAWARS_MATCHES_PLYD", 2000);
			Stats::SetInt("MPX_ARENAWARS_MATCHES_PLYDT", 2000);
			Stats::SetInt("MPX_ARENAWARS_CARRER_WINS", 2000);
			Stats::SetInt("MPX_ARENAWARS_CARRER_WINT", 2000);
			Stats::SetInt("MPX_ARN_LIFETIME_KILLS", 2250);
			Stats::SetInt("MPX_ARN_LIFETIME_DEATHS", 250);
			Stats::SetInt("MPX_ARN_SPECTATOR_KILLS", 500);
			Stats::SetInt("MPX_ARN_W_PASS_THE_BOMB", 1000);
			Stats::SetInt("MPX_ARN_W_DETONATION", 1000);
			Stats::SetInt("MPX_ARN_W_ARCADE_RACE", 1000);
			Stats::SetInt("MPX_ARN_W_CTF", 1000);
			Stats::SetInt("MPX_ARN_W_TAG_TEAM", 1000);
			Stats::SetInt("MPX_ARN_W_DESTR_DERBY", 1000);
			Stats::SetInt("MPX_ARN_W_CARNAGE", 1000);
			Stats::SetInt("MPX_ARN_W_MONSTER_JAM", 1000);
			Stats::SetInt("MPX_ARN_W_GAMES_MASTERS", 1000);
			Stats::SetInt("MPX_ARN_L_PASS_THE_BOMB", 100);
			Stats::SetInt("MPX_ARN_L_DETONATION", 100);
			Stats::SetInt("MPX_ARN_L_ARCADE_RACE", 100);
			Stats::SetInt("MPX_ARN_L_CTF", 100);
			Stats::SetInt("MPX_ARN_L_TAG_TEAM", 200);
			Stats::SetInt("MPX_ARN_L_DESTR_DERBY", 100);
			Stats::SetInt("MPX_ARN_L_CARNAGE", 100);
			Stats::SetInt("MPX_ARN_L_MONSTER_JAM", 1005);
			Stats::SetInt("MPX_ARN_L_GAMES_MASTERS", 100);
			Stats::SetInt("MPX_ARN_VEH_MONSTER3", 90000);
			Stats::SetInt("MPX_ARN_VEH_MONSTER4", 500);
			Stats::SetInt("MPX_ARN_VEH_MONSTER5", 500);
			Stats::SetInt("MPX_ARN_VEH_CERBERUS", 500);
			Stats::SetInt("MPX_ARN_VEH_CERBERUS2", 500);
			Stats::SetInt("MPX_ARN_VEH_CERBERUS3", 500);
			Stats::SetInt("MPX_ARN_VEH_BRUISER", 500);
			Stats::SetInt("MPX_ARN_VEH_BRUISER2", 500);
			Stats::SetInt("MPX_ARN_VEH_BRUISER3", 500);
			Stats::SetInt("MPX_ARN_VEH_SLAMVAN4", 500);
			Stats::SetInt("MPX_ARN_VEH_SLAMVAN5", 500);
			Stats::SetInt("MPX_ARN_VEH_SLAMVAN6", 500);
			Stats::SetInt("MPX_ARN_VEH_BRUTUS", 500);
			Stats::SetInt("MPX_ARN_VEH_BRUTUS2", 500);
			Stats::SetInt("MPX_ARN_VEH_BRUTUS3", 500);
			Stats::SetInt("MPX_ARN_VEH_SCARAB", 500);
			Stats::SetInt("MPX_ARN_VEH_SCARAB2", 500);
			Stats::SetInt("MPX_ARN_VEH_SCARAB3", 500);
			Stats::SetInt("MPX_ARN_VEH_DOMINATOR4", 500);
			Stats::SetInt("MPX_ARN_VEH_DOMINATOR5", 500);
			Stats::SetInt("MPX_ARN_VEH_DOMINATOR6", 500);
			Stats::SetInt("MPX_ARN_VEH_IMPALER2", 500);
			Stats::SetInt("MPX_ARN_VEH_IMPALER3", 500);
			Stats::SetInt("MPX_ARN_VEH_IMPALER4", 500);
			Stats::SetInt("MPX_ARN_VEH_ISSI4", 500);
			Stats::SetInt("MPX_ARN_VEH_ISSI5", 500);
			Stats::SetInt("MPX_ARN_VEH_ISSI6", 500);
			Stats::SetInt("MPX_ARN_VEH_IMPERATOR", 500);
			Stats::SetInt("MPX_ARN_VEH_IMPERATOR2", 500);
			Stats::SetInt("MPX_ARN_VEH_IMPERATOR3", 500);
			Stats::SetInt("MPX_ARN_VEH_ZR380", 500);
			Stats::SetInt("MPX_ARN_VEH_ZR3802", 500);
			Stats::SetInt("MPX_ARN_VEH_ZR3803", 500);
			Stats::SetInt("MPX_ARN_VEH_DEATHBIKE", 500);
			Stats::SetInt("MPX_ARN_VEH_DEATHBIKE2", 400);
			Stats::SetInt("MPX_ARN_VEH_DEATHBIKE3", 400);
			Stats::SetInt("MPX_ARN_SPECTATOR_DRONE", 60);
			Stats::SetInt("MPX_ARN_SPECTATOR_CAMS", 60);
			Stats::SetInt("MPX_ARN_SMOKE", 50);
			Stats::SetInt("MPX_ARN_DRINK", 65);
			Stats::SetInt("MPX_ARN_SPEC_BOX_TIME_MS", 10800000);
			Stats::SetInt("MPX_ARN_W_THEME_SCIFI", 10);
			Stats::SetInt("MPX_ARN_W_THEME_APOC", 10);
			Stats::SetInt("MPX_ARN_W_THEME_CONS", 10);
			Stats::SetInt("MPX_ARN_BS_TRINKET_SAVED", -1);
			for (int i = 25010; i <= 25010; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 25014; i <= 25014; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_ARCADE_MAC_0", 1);
			Stats::SetInt("MPX_ARCADE_MAC_1", 1);
			Stats::SetInt("MPX_ARCADE_MAC_2", 1);
			Stats::SetInt("MPX_ARCADE_MAC_3", 1);
			Stats::SetInt("MPX_ARCADE_MAC_4", 1);
			Stats::SetInt("MPX_ARCADE_MAC_5", 1);
			Stats::SetInt("MPX_ARCADE_MAC_6", 1);
			Stats::SetInt("MPX_ARCADE_MAC_7", 1);
			Stats::SetInt("MPX_ARCADE_MAC_8", 1);
			Stats::SetInt("MPX_ARCADE_MAC_9", 1);
			Stats::SetInt("MPX_ARCADE_MAC_10", 1);
			Stats::SetInt("MPX_ARCADE_MAC_11", 1);
			Stats::SetBool("MPX_SCGW_WON_NO_DEATHS", true);
			Stats::SetBool("MPX_IAP_CHALLENGE_0", true);
			Stats::SetBool("MPX_IAP_CHALLENGE_1", true);
			Stats::SetBool("MPX_IAP_CHALLENGE_2", true);
			Stats::SetBool("MPX_IAP_CHALLENGE_3", true);
			Stats::SetBool("MPX_IAP_CHALLENGE_4", true);
			Stats::SetInt("MPX_SCGW_NUM_WINS_GANG_0", 55);
			Stats::SetInt("MPX_SCGW_NUM_WINS_GANG_1", 56);
			Stats::SetInt("MPX_SCGW_NUM_WINS_GANG_2", 57);
			Stats::SetInt("MPX_SCGW_NUM_WINS_GANG_3", 58);
			Stats::SetInt("MPX_IAP_MAX_MOON_DIST", 2147483647);
			Stats::SetInt("MPX_LAST_ANIMAL", 108);
			Stats::SetInt("MPX_CH_ARC_CAB_CLAW_TROPHY", -1);
			Stats::SetInt("MPX_CH_ARC_CAB_LOVE_TROPHY", -1);
			Stats::SetInt("MPX_FIXER_SC_VEH_RECOVERED", 100);
			Stats::SetInt("MPX_FIXER_SC_VAL_RECOVERED", 100);
			Stats::SetInt("MPX_FIXER_SC_GANG_TERMINATED", 100);
			Stats::SetInt("MPX_FIXER_SC_VIP_RESCUED", 100);
			Stats::SetInt("MPX_FIXER_SC_ASSETS_PROTECTED", 100);
			Stats::SetInt("MPX_FIXER_SC_EQ_DESTROYED", 100);
			Stats::SetInt("MPX_FIXER_COUNT", 600);
			Stats::SetInt("MPX_FIXER_STORY_BS", 4092);
			Stats::SetInt("MPX_PAYPHONE_BONUS_KILL_METHOD", -1);
			Stats::SetInt("MPX_FIXER_HQ_OWNED", 1);
			Stats::SetInt("MPX_FIXER_GENERAL_BS", -1);
			Stats::SetInt("MPX_FIXER_COMPLETED_BS", -1);
			Stats::SetBool("MPX_BS_IMANI_D_APP_SETUP", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_STRAND", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_PARTY", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_PARTY_2", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_PARTY_F", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_BILL", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_BILL_2", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_BILL_F", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_HOOD", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_HOOD_2", true);
			Stats::SetBool("MPX_BS_IMANI_D_APP_HOOD_F", true);
			for (int i = 32312; i <= 32312; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_REV_NV_KILLS", 50);
			Stats::SetInt("MPX_XM22_FLOW", -1);
			Stats::SetInt("MPX_XM22_MISSIONS", -1);
			Stats::SetInt("MPX_XM22_MISSIONS_SA", -1);
			Stats::SetInt("MPX_XM22_CURRENT", -1);
			for (int i = 36668; i <= 36668; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_H3_VEHICLESUSED", -1);
			Stats::SetInt("MPX_H4_H4_DJ_MISSIONS", 127);
			Stats::SetInt("MPX_H4_PROGRESS", -1);
			Stats::SetInt("MPX_TUNER_GEN_BS", 12543);
			Stats::SetInt("MPX_GANGOPS_FLOW_BITSET_MISS0", -1);
			Stats::SetInt("MPPLY_HEISTFLOWORDERPROGRESS", -1);
			Stats::SetInt("MPPLY_HEISTNODEATHPROGREITSET", -1);
			Stats::SetInt("MPPLY_HEISTTEAMPROGRESSBITSET", -1);
			Stats::SetInt("MPX_AT_FLOW_VEHICLE_BS", -1);
			Stats::SetInt("MPX_LFETIME_HANGAR_BUY_COMPLET", 100);
			Stats::SetInt("MPX_DCTL_WINS", 500);
			Stats::SetInt("MPX_DCTL_PLAY_COUNT", 750);
			Stats::SetInt("MPX_SALV23_GEN_BS", -2123778);
			Stats::SetInt("MPX_SALV23_SCOPE_BS", -1);
			Stats::SetInt("MPX_MOST_TIME_ON_3_PLUS_STARS", 300000);
			Stats::SetInt("MPX_LOWRIDER_FLOW_COMPLETE", 1);
			Stats::SetInt("MPX_AT_FLOW_MISSION_PROGRESS", 50);
			Stats::SetInt("MPX_AT_FLOW_IMPEXP_NUM", 50);
			Stats::SetInt("MPX_AT_FLOW_BITSET_MISSIONS0", -1);
			Stats::SetInt("MPX_WVM_FLOW_MISSION_PROGRESS", 50);
			Stats::SetInt("MPX_WVM_FLOW_IMPEXP_NUM", 50);
			Stats::SetInt("MPX_WVM_FLOW_BITSET_MISSIONS0", -1);
			Stats::SetInt("MPX_WVM_FLOW_VEHICLE_BS", -1);
			Stats::SetInt("MPX_GANGOPS_FLOW_MISSION_PROG", -1);
			Stats::SetInt("MPX_GANGOPS_FLOW_IMPEXP_NUM", 50);
			Stats::SetInt("MPX_WAM_FLOW_VEHICLE_BS", -1);
			Stats::SetInt("MPX_GANGOPS_FLOW_PASSED_BITSET", -1);
			Stats::SetInt("MPX_VCM_FLOW_PROGRESS", -1);
			Stats::SetInt("MPX_TUNER_FLOW_BS", -1);
			Stats::SetInt("MPX_TUNER_MIS_BS", -1);
			Stats::SetInt("MPX_TUNER_COMP_BS", 255);
			Stats::SetInt("MPX_GANGOPS_FM_MISSION_PROG", -1);
			Stats::SetInt("MPX_GANGOPS_FM_BITSET_MISS0", -1);
			Stats::SetBool("MPX_UNLOCKED_MESSAGE_FLEECA", true);
			Stats::SetBool("MPX_CARMEET_PV_CHLLGE_CMPLT", true);
			Stats::SetInt("MPX_HEIST_SAVED_STRAND_0_L", 5);
			Stats::SetInt("MPX_HEIST_SAVED_STRAND_1_L", 5);
			Stats::SetInt("MPX_HEIST_SAVED_STRAND_2_L", 5);
			Stats::SetInt("MPX_HEIST_SAVED_STRAND_3_L", 5);
			Stats::SetInt("MPX_HEIST_SAVED_STRAND_4_L", 5);
			Stats::SetInt("MPX_LIFETIME_BUY_COMPLETE", 1000);
			Stats::SetInt("MPX_LIFETIME_BUY_UNDERTAKEN", 1000);
			Stats::SetInt("MPX_LIFETIME_SELL_COMPLETE", 1000);
			Stats::SetInt("MPX_LIFETIME_SELL_UNDERTAKEN", 1000);
			Stats::SetInt("MPX_LIFETIME_CONTRA_EARNINGS", 50000000);
			Stats::SetInt("MPX_LFETIME_BIKER_BUY_COMPLET1", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_BUY_UNDERTA1", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_SELL_COMPLET1", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_SELL_UNDERTA1", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_BUY_COMPLET2", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_BUY_UNDERTA2", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_SELL_COMPLET2", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_SELL_UNDERTA2", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_BUY_COMPLET3", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_BUY_UNDERTA3", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_SELL_COMPLET3", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_SELL_UNDERTA3", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_BUY_COMPLET4", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_BUY_UNDERTA4", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_SELL_COMPLET4", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_SELL_UNDERTA4", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_BUY_COMPLET5", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_BUY_UNDERTA5", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_SELL_COMPLET5", 500);
			Stats::SetInt("MPX_LFETIME_BIKER_SELL_UNDERTA5", 500);
			Stats::SetInt("MPX_LIFETIME_BKR_SELL_EARNINGS0", 25000000);
			Stats::SetInt("MPX_LIFETIME_BKR_SELL_EARNINGS1", 25000000);
			Stats::SetInt("MPX_LIFETIME_BKR_SELL_EARNINGS2", 25000000);
			Stats::SetInt("MPX_LIFETIME_BKR_SELL_EARNINGS3", 25000000);
			Stats::SetInt("MPX_LIFETIME_BKR_SELL_EARNINGS4", 25000000);
			Stats::SetInt("MPX_SR_HIGHSCORE_1", 690);
			Stats::SetInt("MPX_SR_HIGHSCORE_2", 1860);
			Stats::SetInt("MPX_SR_HIGHSCORE_3", 2690);
			Stats::SetInt("MPX_SR_HIGHSCORE_4", 2660);
			Stats::SetInt("MPX_SR_HIGHSCORE_5", 2650);
			Stats::SetInt("MPX_SR_HIGHSCORE_6", 450);
			Stats::SetInt("MPX_SR_TARGETS_HIT", 269);
			Stats::SetInt("MPX_LFETIME_BIKER_BUY_COMPLET6", 100);
			Stats::SetInt("MPX_LFETIME_BIKER_SELL_COMPLET6", 100);
			Stats::SetPackedInt(41241, 5);
			Stats::SetInt("MPX_LIFETIME_BKR_SELL_EARNINGS6", 10000000);
			Stats::SetPackedInt(7666, 25);
			for (int i = 7553; i <= 7594; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetPackedInt(9357, 4);
			for (int i = 9400; i <= 9414; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_XMAS2023_ADV_MODE_WINS", 6);
			Stats::SetInt("MPPLY_XMAS23_PLATES0", 3);
			Stats::SetInt("MPX_COUNT_HOTRING_RACE", 20);
			Stats::SetInt("MPX_FINISHED_SASS_RACE_TOP_3", 20);
			Stats::SetPackedInt(7671, 100);
			Stats::SetInt("MPX_PROG_HUB_BOUNTIES_ALIVE_BS", -1);
			Stats::SetInt("MPX_TIMES_PREV_PLAY_AS_BOSS", 500);
			Stats::SetInt("MPX_GBTELTIMESPLAYEDGOONPREV", 500);
			Stats::SetInt("MPX_WILDLIFEPH_COUNT", 10);
			Stats::SetInt("MPX_LOW_FLOW_CURRENT_PROG", 9);
			Stats::SetInt("MPX_LOW_FLOW_CURRENT_CALL", 9);
			Stats::SetInt("MPX_HUB_SALES_COMPLETED", 10);
			Stats::SetInt("MPX_NIGHTCLUB_JOBS_DONE", 25);
			Stats::SetInt("MPX_AWD_FM_DM_WINS", 50);
			Stats::SetInt("MPX_AWD_FM_TDM_WINS", 50);
			Stats::SetInt("MPX_AWD_FM_TDM_MVP", 50);
			Stats::SetInt("MPX_AWD_RACES_WON", 50);
			Stats::SetInt("MPX_AWD_FMWINAIRRACE", 25);
			Stats::SetInt("MPX_AWD_FMWINSEARACE", 25);
			Stats::SetInt("MPX_AWD_FM_GTA_RACES_WON", 50);
			Stats::SetBool("MPX_AWD_FMKILL3ANDWINGTARACE", true);
			Stats::SetInt("MPX_AWD_FMRALLYWONDRIVE", 25);
			Stats::SetInt("MPX_AWD_FMRALLYWONNAV", 25);
			Stats::SetInt("MPX_AWD_FMWINRACETOPOINTS", 25);
			Stats::SetBool("MPX_AWD_FMWINCUSTOMRACE", true);
			Stats::SetInt("MPX_AWD_FM_RACE_LAST_FIRST", 25);
			Stats::SetBool("MPX_AWD_FMRACEWORLDRECHOLDER", true);
			Stats::SetInt("MPX_AWD_FM_RACES_FASTEST_LAP", 50);
			Stats::SetBool("MPX_AWD_FMWINALLRACEMODES", true);
			Stats::SetInt("MPX_AWD_FMHORDWAVESSURVIVE", 10);
			Stats::SetInt("MPX_MOST_ARM_WRESTLING_WINS", 25);
			Stats::SetInt("MPX_AWD_WIN_AT_DARTS", 25);
			Stats::SetInt("MPX_AWD_FM_GOLF_WON", 25);
			Stats::SetInt("MPX_AWD_FM_TENNIS_WON", 25);
			Stats::SetBool("MPX_AWD_FM_TENNIS_5_SET_WINS", true);
			Stats::SetBool("MPX_AWD_FM_TENNIS_STASETWIN", true);
			Stats::SetInt("MPX_AWD_FM_SHOOTRANG_TG_WON", 25);
			Stats::SetInt("MPX_AWD_FM_SHOOTRANG_CT_WON", 25);
			Stats::SetBool("MPX_AWD_FM_SHOOTRANG_GRAN_WON", true);
			Stats::SetInt("MPX_AWD_FM_SHOOTRANG_RT_WON", 25);
			Stats::SetBool("MPX_AWD_FMWINEVERYGAMEMODE", true);
			Stats::SetInt("MPX_AWD_WIN_CAPTURES", 50);
			Stats::SetInt("MPX_AWD_WIN_CAPTURE_DONT_DYING", 25);
			Stats::SetInt("MPX_AWD_WIN_LAST_TEAM_STANDINGS", 50);
			Stats::SetInt("MPX_AWD_ONLY_PLAYER_ALIVE_LTS", 50);
			Stats::SetInt("MPX_AWD_KILL_TEAM_YOURSELF_LTS", 25);
			Stats::SetBool("MPX_AWD_FM25DIFFERENTDM", true);
			Stats::SetBool("MPX_AWD_FM25DIFFERENTRACES", true);
			Stats::SetInt("MPX_AWD_PARACHUTE_JUMPS_20M", 25);
			Stats::SetInt("MPX_AWD_PARACHUTE_JUMPS_50M", 25);
			Stats::SetInt("MPX_AWD_FMBASEJMP", 25);
			Stats::SetBool("MPX_AWD_FMATTGANGHQ", true);
			Stats::SetBool("MPX_AWD_FM6DARTCHKOUT", true);
			Stats::SetInt("MPX_AWD_FM_GOLF_BIRDIES", 25);
			Stats::SetBool("MPX_AWD_FM_GOLF_HOLE_IN_1", true);
			Stats::SetInt("MPX_AWD_FM_TENNIS_ACE", 25);
			Stats::SetInt("MPX_AWD_FMBBETWIN", 50000);
			Stats::SetInt("MPX_AWD_LAPDANCES", 25);
			Stats::SetBool("MPX_AWD_FM25DIFITEMSCLOTHES", true);
			Stats::SetInt("MPX_AWD_NO_HAIRCUTS", 25);
			Stats::SetBool("MPX_AWD_BUY_EVERY_GUN", true);
			Stats::SetBool("MPX_AWD_FMTATTOOALLBODYPARTS", true);
			Stats::SetInt("MPPLY_AWD_FM_CR_DM_MADE", 25);
			Stats::SetInt("MPPLY_AWD_FM_CR_RACES_MADE", 25);
			Stats::SetInt("MPPLY_AWD_FM_CR_PLAYED_BY_PEEP", 100);
			Stats::SetInt("MPPLY_AWD_FM_CR_MISSION_SCORE", 100);
			Stats::SetInt("MPX_AWD_DROPOFF_CAP_PACKAGES", 100);
			Stats::SetInt("MPX_AWD_PICKUP_CAP_PACKAGES", 100);
			Stats::SetInt("MPX_AWD_MENTALSTATE_TO_NORMAL", 50);
			Stats::SetBool("MPX_AWD_STORE_20_CAR_IN_GARAGES", true);
			Stats::SetInt("MPX_AWD_TRADE_IN_YOUR_PROPERTY", 25);
			Stats::SetInt("MPX_COMPLETEDAILYOBJ", 100);
			Stats::SetInt("MPX_COMPLETEDAILYOBJTOTAL", 100);
			Stats::SetInt("MPX_TOTALDAYCOMPLETED", 100);
			Stats::SetInt("MPX_TOTALWEEKCOMPLETED", 400);
			Stats::SetInt("MPX_TOTALMONTHCOMPLETED", 1800);
			Stats::SetInt("MPX_CONSECUTIVEDAYCOMPLETED", 30);
			Stats::SetInt("MPX_CONSECUTIVEWEEKCOMPLETED", 4);
			Stats::SetInt("MPX_CONSECUTIVEMONTHCOMPLETE", 1);
			Stats::SetInt("MPX_COMPLETEDAILYOBJSA", 100);
			Stats::SetInt("MPX_COMPLETEDAILYOBJTOTALSA", 100);
			Stats::SetInt("MPX_TOTALDAYCOMPLETEDSA", 100);
			Stats::SetInt("MPX_TOTALWEEKCOMPLETEDSA", 400);
			Stats::SetInt("MPX_TOTALMONTHCOMPLETEDSA", 1800);
			Stats::SetInt("MPX_CONSECUTIVEDAYCOMPLETEDSA", 30);
			Stats::SetInt("MPX_CONSECUTIVEWEEKCOMPLETEDSA", 4);
			Stats::SetInt("MPX_CONSECUTIVEMONTHCOMPLETESA", 1);
			Stats::SetInt("MPX_AWD_DAILYOBJCOMPLETED", 100);
			Stats::SetInt("MPX_AWD_DAILYOBJCOMPLETEDSA", 100);
			Stats::SetBool("MPX_AWD_DAILYOBJWEEKBONUS", true);
			Stats::SetBool("MPX_AWD_DAILYOBJWEEKBONUSSA", true);
			Stats::SetBool("MPX_AWD_DAILYOBJMONTHBONUS", true);
			Stats::SetBool("MPX_AWD_DAILYOBJMONTHBONUSSA", true);
			Stats::SetInt("MPX_CHAR_WANTED_LEVEL_TIME5STAR", 7200000);
			Stats::SetInt("MPX_AWD_5STAR_WANTED_AVOIDANCE", 50);
			Stats::SetInt("MPX_AWD_FMSHOOTDOWNCOPHELI", 25);
			Stats::SetInt("MPX_PASS_DB_PLAYER_KILLS", 100);
			Stats::SetInt("MPX_NUMBER_STOLEN_CARS", 500);
			Stats::SetInt("MPX_AWD_HOLD_UP_SHOPS", 20);
			Stats::SetInt("MPX_CARS_EXPLODED", 500);
			Stats::SetInt("MPX_AWD_CARS_EXPORTED", 50);
			Stats::SetInt("MPX_AWD_FMDRIVEWITHOUTCRASH", 30);
			Stats::SetInt("MPX_AWD_PASSENGERTIME", 4);
			Stats::SetInt("MPX_AWD_TIME_IN_HELICOPTER", 4);
			Stats::SetBool("MPX_AWD_FMFULLYMODDEDCAR", true);
			Stats::SetInt("MPX_AIR_LAUNCHES_OVER_40M", 25);
			Stats::SetInt("MPX_MOST_FLIPS_IN_ONE_JUMP", 5);
			Stats::SetInt("MPX_MOST_SPINS_IN_ONE_JUMP", 5);
			Stats::SetBool("MPX_AWD_FMFURTHESTWHEELIE", true);
			Stats::SetInt("MPX_AWD_100_HEADSHOTS", 500);
			Stats::SetInt("MPX_KILLS_PLAYERS", 1000);
			Stats::SetInt("MPX_AWD_FMKILLBOUNTY", 25);
			Stats::SetInt("MPX_AWD_FMREVENGEKILLSDM", 50);
			Stats::SetInt("MPX_AWD_FM_DM_KILLSTREAK", 100);
			Stats::SetInt("MPX_AWD_FM_DM_STOLENKILL", 50);
			Stats::SetInt("MPX_AWD_FM_DM_TOTALKILLS", 500);
			Stats::SetBool("MPX_AWD_FMKILLSTREAKSDM", true);
			Stats::SetBool("MPX_AWD_FMMOSTKILLSGANGHIDE", true);
			Stats::SetBool("MPX_AWD_FMMOSTKILLSSURVIVE", true);
			Stats::SetInt("MPX_AWD_FM_DM_3KILLSAMEGUY", 50);
			Stats::SetInt("MPX_AWD_KILL_CARRIER_CAPTURE", 100);
			Stats::SetInt("MPX_AWD_NIGHTVISION_KILLS", 100);
			Stats::SetInt("MPX_AWD_KILL_PSYCHOPATHS", 100);
			Stats::SetInt("MPX_PISTOL50_ENEMY_KILLS", 500);
			Stats::SetInt("MPX_AWD_100_KILLS_SMG", 500);
			Stats::SetInt("MPX_ASLTSHTGN_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_ASLTRIFLE_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_SNIPERRFL_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_CMBTMG_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_AWD_25_KILLS_STICKYBOMBS", 50);
			Stats::SetInt("MPX_GRENADE_ENEMY_KILLS", 50);
			Stats::SetInt("MPX_RPG_ENEMY_KILLS", 600);
			Stats::SetInt("MPX_UNARMED_ENEMY_KILLS", 50);
			Stats::SetInt("MPX_AWD_CAR_BOMBS_ENEMY_KILLS", 25);
			Stats::SetInt("MPX_AWD_FINISH_HEISTS", 50);
			Stats::SetInt("MPX_AWD_FINISH_HEIST_SETUP_JOB", 50);
			Stats::SetBool("MPX_AWD_FINISH_HEIST_NO_DAMAGE", true);
			Stats::SetInt("MPX_AWD_WIN_GOLD_MEDAL_HEISTS", 25);
			Stats::SetInt("MPX_AWD_DO_HEIST_AS_MEMBER", 25);
			Stats::SetInt("MPX_AWD_DO_HEIST_AS_THE_LEADER", 25);
			Stats::SetBool("MPX_AWD_SPLIT_HEIST_TAKE_EVENLY", true);
			Stats::SetBool("MPX_AWD_ACTIVATE_2_PERSON_KEY", true);
			Stats::SetInt("MPX_AWD_CONTROL_CROWDS", 25);
			Stats::SetBool("MPX_AWD_ALL_ROLES_HEIST", true);
			Stats::SetBool("MPPLY_AWD_FLEECA_FIN", true);
			Stats::SetBool("MPPLY_AWD_PRISON_FIN", true);
			Stats::SetBool("MPPLY_AWD_HUMANE_FIN", true);
			Stats::SetBool("MPPLY_AWD_SERIESA_FIN", true);
			Stats::SetBool("MPPLY_AWD_PACIFIC_FIN", true);
			Stats::SetBool("MPPLY_AWD_HST_ORDER", true);
			Stats::SetBool("MPPLY_AWD_HST_SAME_TEAM", true);
			Stats::SetBool("MPPLY_AWD_HST_ULT_CHAL", true);
			Stats::SetBool("MPPLY_AWD_COMPLET_HEIST_MEM", true);
			Stats::SetBool("MPPLY_AWD_COMPLET_HEIST_1STPER", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_IAA", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_SUBMARINE", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_MISSILE", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_ALLINORDER", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_LOYALTY", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_LOYALTY2", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_LOYALTY3", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_CRIMMASMD", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_CRIMMASMD2", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_CRIMMASMD3", true);
			Stats::SetBool("MPPLY_AWD_GANGOPS_SUPPORT", true);
			Stats::SetInt("MPX_AWD_CLUB_DRUNK", 200);
			Stats::SetInt("MPX_DANCEPERFECTOWNCLUB", 100);
			Stats::SetInt("MPX_NIGHTCLUB_PLAYER_APPEAR", 500);
			Stats::SetInt("MPX_AWD_DANCE_TO_SOLOMUN", 100);
			Stats::SetInt("MPX_AWD_DANCE_TO_TALEOFUS", 100);
			Stats::SetInt("MPX_AWD_DANCE_TO_DIXON", 100);
			Stats::SetInt("MPX_AWD_DANCE_TO_BLKMAD", 100);
			Stats::SetInt("MPX_DANCETODIFFDJS", 4);
			Stats::SetInt("MPX_AWD_CAREER_WINNER", 1000);
			Stats::SetInt("MPX_AWD_TOP_SCORE", 500000);
			Stats::SetInt("MPX_AWD_TIME_SERVED", 1000);
			Stats::SetInt("MPX_AWD_ARENA_WAGEWORKER", 20000000);
			Stats::SetInt("MPX_AWD_WEVE_GOT_ONE", 50);
			Stats::SetInt("MPX_AWD_YOURE_OUTTA_HERE", 200);
			Stats::SetInt("MPX_AWD_MASSIVE_SHUNT", 50);
			Stats::SetInt("MPX_AWD_KILL_OR_BE_KILLED", 50);
			Stats::SetInt("MPX_AWD_CROWDPARTICIPATION", 50);
			Stats::SetInt("MPX_AWD_SITTING_DUCK", 50);
			Stats::SetInt("MPX_AWD_YOUMEANBOOBYTRAPS", 50);
			Stats::SetInt("MPX_AWD_MASTER_BANDITO", 50);
			Stats::SetInt("MPX_AWD_SPINNER", 50);
			Stats::SetInt("MPX_AWD_THROUGH_A_LENS", 50);
			Stats::SetInt("MPX_AWD_READY_FOR_WAR", 50);
			Stats::SetBool("MPX_AWD_UNSTOPPABLE", true);
			Stats::SetBool("MPX_AWD_CONTACT_SPORT", true);
			Stats::SetInt("MPX_AWD_TOWER_OFFENSE", 50);
			Stats::SetInt("MPX_AWD_WATCH_YOUR_STEP", 50);
			Stats::SetBool("MPX_AWD_PEGASUS", true);
			Stats::SetBool("MPX_AWD_BEGINNER", true);
			Stats::SetBool("MPX_AWD_FIELD_FILLER", true);
			Stats::SetBool("MPX_AWD_ARMCHAIR_RACER", true);
			Stats::SetBool("MPX_AWD_LEARNER", true);
			Stats::SetBool("MPX_AWD_SUNDAY_DRIVER", true);
			Stats::SetBool("MPX_AWD_THE_ROOKIE", true);
			Stats::SetBool("MPX_AWD_BUMP_AND_RUN", true);
			Stats::SetBool("MPX_AWD_GEAR_HEAD", true);
			Stats::SetBool("MPX_AWD_DOOR_SLAMMER", true);
			Stats::SetBool("MPX_AWD_HOT_LAP", true);
			Stats::SetBool("MPX_AWD_ARENA_AMATEUR", true);
			Stats::SetBool("MPX_AWD_PAINT_TRADER", true);
			Stats::SetBool("MPX_AWD_SHUNTER", true);
			Stats::SetBool("MPX_AWD_JOCK", true);
			Stats::SetBool("MPX_AWD_WARRIOR", true);
			Stats::SetBool("MPX_AWD_T_BONE", true);
			Stats::SetBool("MPX_AWD_MAYHEM", true);
			Stats::SetBool("MPX_AWD_WRECKER", true);
			Stats::SetBool("MPX_AWD_CRASH_COURSE", true);
			Stats::SetBool("MPX_AWD_ARENA_LEGEND", true);
			Stats::SetBool("MPX_AWD_FIRST_TIME1", true);
			Stats::SetBool("MPX_AWD_FIRST_TIME2", true);
			Stats::SetBool("MPX_AWD_FIRST_TIME3", true);
			Stats::SetBool("MPX_AWD_FIRST_TIME4", true);
			Stats::SetBool("MPX_AWD_FIRST_TIME5", true);
			Stats::SetBool("MPX_AWD_FIRST_TIME6", true);
			Stats::SetBool("MPX_AWD_ALL_IN_ORDER", true);
			Stats::SetBool("MPX_AWD_SURVIVALIST", true);
			Stats::SetBool("MPX_AWD_SUPPORTING_ROLE", true);
			Stats::SetBool("MPX_AWD_LEADER", true);
			Stats::SetInt("MPX_AWD_ODD_JOBS", 100);
			Stats::SetBool("MPX_AWD_SCOPEOUT", true);
			Stats::SetInt("MPX_AWD_PREPARATION", 40);
			Stats::SetBool("MPX_AWD_CREWEDUP", true);
			Stats::SetBool("MPX_AWD_MOVINGON", true);
			Stats::SetBool("MPX_AWD_PROMOCAMP", true);
			Stats::SetBool("MPX_AWD_GUNMAN", true);
			Stats::SetBool("MPX_AWD_SMASHNGRAB", true);
			Stats::SetBool("MPX_AWD_INPLAINSI", true);
			Stats::SetBool("MPX_AWD_UNDETECTED", true);
			Stats::SetBool("MPX_AWD_ALLROUND", true);
			Stats::SetBool("MPX_AWD_ELITETHEIF", true);
			Stats::SetBool("MPX_AWD_PRO", true);
			Stats::SetBool("MPX_AWD_SUPPORTACT", true);
			Stats::SetBool("MPX_AWD_SHAFTED", true);
			Stats::SetInt("MPX_AWD_ASLEEPONJOB", 20);
			Stats::SetInt("MPX_AWD_DAICASHCRAB", 100000);
			Stats::SetInt("MPX_AWD_BIGBRO", 40);
			Stats::SetBool("MPX_AWD_COLLECTOR", true);
			Stats::SetBool("MPX_AWD_DEADEYE", true);
			Stats::SetBool("MPX_AWD_PISTOLSATDAWN", true);
			Stats::SetInt("MPX_AWD_SHARPSHOOTER", 40);
			Stats::SetInt("MPX_AWD_RACECHAMP", 40);
			Stats::SetBool("MPX_AWD_TRAFFICAVOI", true);
			Stats::SetBool("MPX_AWD_CANTCATCHBRA", true);
			Stats::SetBool("MPX_AWD_WIZHARD", true);
			Stats::SetInt("MPX_AWD_BATSWORD", 1000000);
			Stats::SetInt("MPX_AWD_COINPURSE", 950000);
			Stats::SetBool("MPX_AWD_APEESCAPE", true);
			Stats::SetBool("MPX_AWD_MONKEYKIND", true);
			Stats::SetInt("MPX_AWD_ASTROCHIMP", 3000000);
			Stats::SetBool("MPX_AWD_AQUAAPE", true);
			Stats::SetBool("MPX_AWD_KEEPFAITH", true);
			Stats::SetInt("MPX_AWD_MASTERFUL", 40000);
			Stats::SetBool("MPX_AWD_TRUELOVE", true);
			Stats::SetBool("MPX_AWD_NEMESIS", true);
			Stats::SetBool("MPX_AWD_FRIENDZONED", true);
			Stats::SetBool("MPX_AWD_KINGOFQUB3D", true);
			Stats::SetBool("MPX_AWD_QUBISM", true);
			Stats::SetBool("MPX_AWD_GODOFQUB3D", true);
			Stats::SetBool("MPX_AWD_QUIBITS", true);
			Stats::SetBool("MPX_AWD_ELEVENELEVEN", true);
			Stats::SetBool("MPX_AWD_GOFOR11TH", true);
			Stats::SetBool("MPX_AWD_INTELGATHER", true);
			Stats::SetBool("MPX_AWD_COMPOUNDINFILT", true);
			Stats::SetInt("MPX_AWD_WELL_PREPARED", 50);
			Stats::SetBool("MPX_AWD_LOOT_FINDER", true);
			Stats::SetBool("MPX_AWD_MAX_DISRUPT", true);
			Stats::SetBool("MPX_AWD_THE_ISLAND_HEIST", true);
			Stats::SetBool("MPX_AWD_GOING_ALONE", true);
			Stats::SetBool("MPX_AWD_TEAM_WORK", true);
			Stats::SetBool("MPX_AWD_MIXING_UP", true);
			Stats::SetBool("MPX_AWD_PRO_THIEF", true);
			Stats::SetBool("MPX_AWD_CAT_BURGLAR", true);
			Stats::SetBool("MPX_AWD_ONE_OF_THEM", true);
			Stats::SetInt("MPX_AWD_FILL_YOUR_BAGS", 20000000);
			Stats::SetBool("MPX_AWD_GOLDEN_GUN", true);
			Stats::SetBool("MPX_AWD_ELITE_THIEF", true);
			Stats::SetBool("MPX_AWD_PROFESSIONAL", true);
			Stats::SetInt("MPX_AWD_WRECK_DIVING", 1000000);
			Stats::SetBool("MPX_AWD_PARTY_VIBES", true);
			Stats::SetInt("MPX_AWD_SUNSET", 1800000);
			Stats::SetBool("MPX_AWD_HELPING_HAND", true);
			Stats::SetInt("MPX_AWD_MOODYMANN", 1800000);
			Stats::SetInt("MPX_AWD_PALMS_TRAX", 1800000);
			Stats::SetBool("MPX_AWD_HELPING_OUT", true);
			Stats::SetInt("MPX_AWD_KEINEMUSIK", 1800000);
			Stats::SetBool("MPX_AWD_COURIER", true);
			Stats::SetInt("MPX_AWD_TREASURE_HUNTER", 50);
			Stats::SetBool("MPX_AWD_CAR_CLUB", true);
			Stats::SetInt("MPX_AWD_CAR_CLUB_MEM", 1000);
			Stats::SetInt("MPX_AWD_SPRINTRACER", 50);
			Stats::SetInt("MPX_AWD_STREETRACER", 50);
			Stats::SetInt("MPX_AWD_PURSUITRACER", 50);
			Stats::SetInt("MPX_AWD_TEST_CAR", 1800000);
			Stats::SetInt("MPX_AWD_AUTO_SHOP", 100);
			Stats::SetInt("MPX_AWD_CAR_EXPORT", 100);
			Stats::SetBool("MPX_AWD_PRO_CAR_EXPORT", true);
			Stats::SetInt("MPX_AWD_GROUNDWORK", 250);
			Stats::SetBool("MPX_AWD_UNION_DEPOSITORY", true);
			Stats::SetBool("MPX_AWD_MILITARY_CONVOY", true);
			Stats::SetBool("MPX_AWD_FLEECA_BANK", true);
			Stats::SetBool("MPX_AWD_FREIGHT_TRAIN", true);
			Stats::SetBool("MPX_AWD_BOLINGBROKE_[removed]", true);
			Stats::SetBool("MPX_AWD_IAA_RAID", true);
			Stats::SetBool("MPX_AWD_METH_JOB", true);
			Stats::SetBool("MPX_AWD_BUNKER_RAID", true);
			Stats::SetInt("MPX_AWD_ROBBERY_CONTRACT", 100);
			Stats::SetInt("MPX_AWD_FACES_OF_DEATH", 100);
			Stats::SetBool("MPX_AWD_STRAIGHT_TO_VIDEO", true);
			Stats::SetBool("MPX_AWD_MONKEY_C_MONKEY_DO", true);
			Stats::SetBool("MPX_AWD_TRAINED_TO_KILL", true);
			Stats::SetBool("MPX_AWD_DIRECTOR", true);
			Stats::SetBool("MPX_AWD_TEEING_OFF", true);
			Stats::SetBool("MPX_AWD_PARTY_NIGHT", true);
			Stats::SetBool("MPX_AWD_BILLIONAIRE_GAMES", true);
			Stats::SetBool("MPX_AWD_HOOD_PASS", true);
			Stats::SetBool("MPX_AWD_STUDIO_TOUR", true);
			Stats::SetBool("MPX_AWD_DONT_MESS_DRE", true);
			Stats::SetBool("MPX_AWD_BACKUP", true);
			Stats::SetBool("MPX_AWD_SHORTFRANK_1", true);
			Stats::SetBool("MPX_AWD_SHORTLAMAR_1", true);
			Stats::SetBool("MPX_AWD_SHORTFRANK_2", true);
			Stats::SetBool("MPX_AWD_SHORTLAMAR_2", true);
			Stats::SetBool("MPX_AWD_SHORTFRANK_3", true);
			Stats::SetBool("MPX_AWD_SHORTLAMAR_3", true);
			Stats::SetInt("MPX_AWD_CONTRACTOR", 50);
			Stats::SetInt("MPX_AWD_COLD_CALLER", 50);
			Stats::SetBool("MPX_AWD_CONTR_KILLER", true);
			Stats::SetBool("MPX_AWD_DOGS_BEST_FRIEND", true);
			Stats::SetBool("MPX_AWD_MUSIC_STUDIO", true);
			Stats::SetInt("MPX_AWD_PRODUCER", 60);
			Stats::SetBool("MPX_AWD_ACELIQUOR", true);
			Stats::SetBool("MPX_AWD_TRUCKAMBUSH", true);
			Stats::SetBool("MPX_AWD_LOSTCAMPREV", true);
			Stats::SetBool("MPX_AWD_ACIDTRIP", true);
			Stats::SetInt("MPX_AWD_RUNRABBITRUN", 5);
			Stats::SetBool("MPX_AWD_HIPPYRIVALS", true);
			Stats::SetBool("MPX_AWD_TRAINCRASH", true);
			Stats::SetInt("MPX_AWD_CALLME", 50);
			Stats::SetBool("MPX_AWD_BACKUPB", true);
			Stats::SetBool("MPX_AWD_GETSTARTED", true);
			Stats::SetInt("MPX_AWD_CHEMCOMPOUNDS", 50);
			Stats::SetBool("MPX_AWD_CHEMREACTION", true);
			Stats::SetInt("MPX_AWD_STASHHORAID", 50);
			Stats::SetInt("MPX_AWD_DEADDROP", 50);
			Stats::SetInt("MPX_AWD_GOODSAMARITAN", 50);
			Stats::SetBool("MPX_AWD_WAREHODEFEND", true);
			Stats::SetBool("MPX_AWD_RESCUECOOK", true);
			Stats::SetBool("MPX_AWD_DRUGTRIPREHAB", true);
			Stats::SetBool("MPX_AWD_ATTACKINVEST", true);
			Stats::SetInt("MPX_AWD_OWNWORSTENEMY", 60);
			Stats::SetBool("MPX_AWD_CARGOPLANE", true);
			Stats::SetBool("MPX_AWD_BACKUPB2", true);
			Stats::SetInt("MPX_AWD_TAXIDRIVER", 50);
			Stats::SetBool("MPX_AWD_TAXISTAR", true);
			Stats::SetBool("MPX_AWD_MAZE_BANK_ROBBERY", true);
			Stats::SetBool("MPX_AWD_CARGO_SHIP_ROBBERY", true);
			Stats::SetBool("MPX_AWD_MISSION_ROW_ROBBERY", true);
			Stats::SetBool("MPX_AWD_PERFECT_RUN", true);
			Stats::SetBool("MPX_AWD_EXTRA_MILE", true);
			Stats::SetInt("MPX_AWD_VEHICLE_ROBBERIES", 50);
			Stats::SetInt("MPX_AWD_PREP_WORK", 50);
			Stats::SetInt("MPX_AWD_CAR_DEALER", 5000000);
			Stats::SetInt("MPX_AWD_SECOND_HAND_PARTS", 5000000);
			Stats::SetInt("MPX_AWD_TOW_TRUCK_SERVICE", 50);
			Stats::SetBool("MPX_AWD_SUBMARINE_ROBBERY", true);
			Stats::SetBool("MPX_AWD_DIAMOND_CASINO_ROBBERY", true);
			Stats::SetBool("MPX_AWD_BOLINGBROKE", true);
			Stats::SetBool("MPX_AWD_GETTING_SET_UP", true);
			Stats::SetBool("MPX_AWD_CHICKEN_FACTORY_RAID", true);
			Stats::SetBool("MPX_AWD_HELPING_HAND2", true);
			Stats::SetBool("MPX_AWD_SURPRISE_ATTACK", true);
			Stats::SetBool("MPX_AWD_ALL_OUT_RAID", true);
			Stats::SetBool("MPX_AWD_WEAPON_ARSENAL", true);
			Stats::SetBool("MPX_AWD_GETAWAY_VEHICLES", true);
			Stats::SetBool("MPX_AWD_HIVALBOUNT1", true);
			Stats::SetBool("MPX_AWD_HIVALBOUNT2", true);
			Stats::SetBool("MPX_AWD_HIVALBOUNT3", true);
			Stats::SetBool("MPX_AWD_HIVALBOUNT4", true);
			Stats::SetBool("MPX_AWD_HIVALBOUNT5", true);
			Stats::SetBool("MPX_AWD_HIVALBOUNT6", true);
			Stats::SetInt("MPX_AWD_BOUNTIES", 50);
			Stats::SetInt("MPX_AWD_STANBOUNTIES", 50);
			Stats::SetInt("MPX_AWD_BOUNTEARNS", 5000000);
			Stats::SetInt("MPX_AWD_BAILOFFICSTAFF", 1000000);
			Stats::SetInt("MPX_AWD_DISPATCHWORK", 50);
			Stats::SetInt("MPX_AWD_PIZZATHIS", 50);
			Stats::SetBool("MPX_AWD_ASSONATTACKSWIN", true);
			Stats::SetBool("MPX_AWD_ASSONDEFENDWIN", true);
			Stats::SetInt("MPX_AWD_ASSONBONUSOBJ", 20);
			Stats::SetInt("MPX_AWD_ASSONHARDDRIVE", 50);
			Stats::SetInt("MPX_AWD_FROSTBITE", 15);
			Stats::SetBool("MPX_AWD_DISEASECONTROL", true);
			Stats::SetBool("MPX_AWD_FINEART", true);
			Stats::SetBool("MPX_AWD_BRUTEFORCE", true);
			Stats::SetBool("MPX_AWD_PROJECTBREAK", true);
			Stats::SetBool("MPX_AWD_BLACKBOXFILE", true);
			Stats::SetBool("MPX_AWD_BONUSPOINTS", true);
			Stats::SetInt("MPX_AWD_DARNELLBROSINC", 50);
			Stats::SetInt("MPX_AWD_GET_READY", 50);
			Stats::SetInt("MPX_AWD_CASHINHAND", 5000000);
			Stats::SetInt("MPX_AWD_BROTHERLYLOVE", 50000);
			Stats::SetBool("MPX_AWD_UPRUNNING", true);
			Stats::SetBool("MPX_AWD_MOGUL", true);
			Stats::SetBool("MPX_AWD_INTEL", true);
			Stats::SetInt("MPX_AWD_NOTOUTDPT", 5);
			Stats::SetBool("MPX_AWD_IRONMULE", true);
			Stats::SetBool("MPX_AWD_AMMUNITION", true);
			Stats::SetBool("MPX_AWD_DIRDELIVERY", true);
			Stats::SetInt("MPX_AWD_FULSTOCKED", 8);
			Stats::SetBool("MPX_AWD_TITANJOB", true);
			Stats::SetBool("MPX_AWD_PERMANENTCON", true);
			Stats::SetInt("MPX_AWD_ARMSINARMS", 50);
			Stats::SetBool("MPX_AWD_JUNKSEARCH", true);
			Stats::SetInt("MPX_AWD_DOGSDINNER", 5);
			Stats::SetBool("MPX_AWD_CASASS", true);
			Stats::SetBool("MPX_AWD_LOCKUPINT", true);
			Stats::SetBool("MPX_AWD_ESCORTFLATBED", true);
			Stats::SetBool("MPX_AWD_LEFT4DEAD", true);
			Stats::SetBool("MPX_AWD_TRACKER", true);
			Stats::SetBool("MPX_AWD_CLEARCOMP", true);
			Stats::SetBool("MPX_AWD_CASHBONUS", true);
			Stats::SetInt("MPX_AWD_HEATAROUDC", 25);
			Stats::SetInt("MPX_AWD_TYCOON", 2000000);
			Stats::SetBool("MPX_AWD_BUSINEXPAND", true);
			Stats::SetBool("MPX_AWD_LOSTPRODUC", true);
			Stats::SetInt("MPX_AWD_CASHCLEAN", 500000);
			Stats::SetInt("MPX_AWD_MEDICOURI", 50);
			Stats::SetBool("MPX_AWD_EMERGENCYSERV", true);
			Stats::SetInt("MPX_AWD_TRANSPORT", 20);
			Stats::SetBool("MPX_AWD_SECUREDEL", true);
			Stats::SetInt("MPX_AWD_SUNBURNED", 15);
			Stats::SetBool("MPX_AWD_TWOOFAKIND", true);
			Stats::SetInt("MPX_AWD_UNDEADPARTY", 60);
			Stats::SetInt("MPX_CHAR_WEAP_FM_PURCHASE3", -872415233);
			Stats::SetInt("MPX_CHAR_WEAP_FM_PURCHASE4", 126);
			Stats::SetPackedBool(54653, true);
			Stats::SetInt("MPX_SB_CAR_WASH_OWNED", 1);
			Stats::SetInt("MPX_SB_HELI_TOURS_OWNED", 1);
			Stats::SetInt("MPX_SB_WEED_SHOP_OWNED", 1);
			for (int i = 28313; i <= 28318; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetPackedInt(24928, 10);
			Stats::SetInt("MPX_PROG_HUB_TYCOON25_EARNINGS", 2000000);
			Stats::SetInt("MPX_PROG_HUB_CWASH_SAFEEARNINGS", 500000);
			for (int i = 51278; i <= 51280; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 51285; i <= 51297; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_PROG_HUB_MFH_EARNINGS", 5000000);
			for (int i = 28272; i <= 28287; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_PROG_HUB_CBR_EARNINGS", 5000000);
			Stats::SetInt("MPX_SUM23_AVOP_PROGRESS", 4095);
			Stats::SetInt("MPX_ULP_MISSION_PROGRESS", 16383);
			Stats::SetInt("MPX_YACHT_MISSION_FLOW", 21845);
			for (int i = 41507; i <= 42022; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetPackedBool(51339, true);
			Stats::SetInt("MPX_HACKER_DEN_OWNED", 1);
			Stats::SetPackedBool(9539, true);
			Stats::SetPackedInt(24903, 25);
			Stats::SetPackedInt(24904, 2);
			Stats::SetInt("MPX_PROG_HUB_DEN24_SAFEEARNINGS", 100000);
			Stats::SetInt("MPX_HACKER24_GEN_BS", 8160);
			Stats::SetPackedBool(9549, true);
			for (int i = 9542; i <= 9546; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_BAIL_OFFICE_OWNED", 1);
			Stats::SetPackedBool(9537, true);
			Stats::SetPackedBool(7639, true);
			Stats::SetPackedInt(7669, 5);
			Stats::SetPackedInt(7672, 2);
			Stats::SetInt("MPX_PROG_HUB_BOUNTY_EARNINGS", 5000000);
			Stats::SetPackedInt(7670, 10);
			Stats::SetPackedInt(7674, 3);
			Stats::SetPackedInt(7671, 25);
			Stats::SetInt("MPX_BAIL_PROPERTY_EARNINGS", 1000000);
			Stats::SetInt("MPX_SALVAGE_YARD_OWNED", 1);
			for (int i = 42037; i <= 42038; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42041; i <= 42042; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 42044; i <= 42051; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_PROG_HUB_SALV23_EARN_SALV", 2500000);
			Stats::SetInt("MPX_PROG_HUB_SALV23_EARN_PERF", 5);
			Stats::SetInt("MPX_PROG_HUB_SALV23_EARN_SELL", 10000000);
			Stats::SetInt("MPX_PROG_HUB_LSDW_FJ_NO_DEATHS", 25);
			Stats::SetInt("MPX_PROG_HUB_DAX_CLONE_KILLS", 100);
			for (int i = 41660; i <= 41670; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetPackedBool(28257, true);
			Stats::SetInt("MPX_PROG_HUB_FXER_PAY_HIT_BONUS", 100);
			Stats::SetInt("MPX_PROG_HUB_FIXER_SEC_CON_SPEC", 100);
			Stats::SetInt("MPX_FIXER_EARNINGS", 5000000);
			for (int i = 22067; i <= 22068; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_PROG_HUB_DANCE_DUR", 5);
			Stats::SetPackedBool(15533, true);
			for (int i = 22082; i <= 22083; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetPackedBool(36944, true);
			Stats::SetInt("MPX_NIGHTCLUB_VIP_APPEAR", 25);
			Stats::SetInt("MPX_PROG_HUB_NCLUB_POP_MAX_TME", 600);
			Stats::SetInt("MPX_HUB_EARNINGS", 50000000);
			Stats::SetPackedBool(15966, true);
			Stats::SetPackedBool(41676, true);
			Stats::SetInt("MPX_LFETIME_HANGAR_SEL_COMPLET", 1);
			Stats::SetInt("MPX_PROG_HUB_SMUGGLER_CRATES", 1000);
			for (int i = 36924; i <= 36938; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_LFETIME_HANGAR_EARNINGS", 50000000);
			Stats::SetInt("MPX_FACTORYSETUP5", 1);
			Stats::SetPackedInt(9359, 25);
			for (int i = 36870; i <= 36917; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_LIFETIME_BKR_SEL_COMPLETBC5", 1);
			for (int i = 36831; i <= 36865; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_LIFETIME_BKR_SELL_EARNINGS5", 25000000);
			Stats::SetInt("MPX_OWNED_IE_WAREHOUSE", 1);
			Stats::SetInt("MPX_LFETIME_IE_EXPORT_COMPLETED", 25);
			for (int i = 42029; i <= 42036; ++i)
				Stats::SetPackedBool(i, true);
			for (int i = 41874; i <= 41883; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_PROG_HUB_VEH_CARGO_SELL_PER", 50);
			Stats::SetInt("MPX_PROG_HUB_VEH_CARGO_SPECIAL", 5);
			Stats::SetInt("MPX_LFETIME_IE_MISSION_EARNINGS", 25000000);
			Stats::SetInt("MPX_LIFETIME_BKR_SELL_COMPLETBC", 1);
			Stats::SetInt("MPX_LIFETIME_BKR_SEL_COMPLETBC1", 1);
			Stats::SetInt("MPX_LIFETIME_BKR_SEL_COMPLETBC2", 1);
			Stats::SetInt("MPX_LIFETIME_BKR_SEL_COMPLETBC3", 1);
			Stats::SetInt("MPX_LIFETIME_BKR_SEL_COMPLETBC4", 1);
			Stats::SetInt("MPX_BAR_RESUPPLY_CR", 10);
			Stats::SetInt("MPX_PROG_HUB_BIK_CUST_DEL_CASH", 2500000);
			Stats::SetInt("MPX_PROG_HUB_CLBH_BAR_EARNINGS", 500000);
			Stats::SetInt("MPX_PROG_HUB_BIK_CONTRACT_COUNT", 50);
			for (int i = 7553; i <= 7592; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_PROG_HUB_FAIFAF_CRATES_COL", 250);
			Stats::SetInt("MPX_CAR_CLUB_MEMBERSHIP", 1);
			Stats::SetPackedBool(31737, true);
			Stats::SetPackedBool(31753, true);
			for (int i = 32397; i <= 32399; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_TUNER_COUNT", 25);
			Stats::SetInt("MPX_PROG_HUB_TUNER_CUS_DEL_CASH", 5000000);
			for (int i = 27089; i <= 27090; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetPackedBool(30309, true);
			Stats::SetPackedBool(30522, true);
			Stats::SetInt("MPX_H4_MISSIONS", -1);
			Stats::SetInt("MPX_PROG_HUB_CAYO_PRP_NO_DEATHS", 25);
			Stats::SetInt("MPX_CR_SUBMARINE", 1);
			Stats::SetInt("MPX_CR_STRATEGIC_BOMBER", 1);
			Stats::SetInt("MPX_CR_SMUGGLER_PLANE", 1);
			Stats::SetInt("MPX_CR_STEALTH_HELI", 1);
			Stats::SetInt("MPX_CR_PATROL_BOAT", 1);
			Stats::SetInt("MPX_CR_SMUGGLER_BOAT", 1);
			Stats::SetInt("MPX_PROG_HUB_CAYO_H_EARNINGS", 50000000);
			Stats::SetPackedBool(28270, true);
			Stats::SetPackedBool(36842, true);
			Stats::SetPackedBool(42025, true);
			Stats::SetInt("MPX_PROG_HUB_CASINO_H_EARNINGS", 50000000);
			Stats::SetPackedBool(18139, true);
			Stats::SetPackedBool(36861, true);
			for (int i = 41712; i <= 41719; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_PROG_HUB_DOOM_PRP_NO_DEATHS", 14);
			Stats::SetInt("MPX_PROG_HUB_DOOMSDAY_ACTS", 15);
			for (int i = 36867; i <= 36868; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetPackedInt(42100, 5);
			Stats::SetInt("MPX_PROG_HUB_HEIST_EARNINGS", 50000000);
			Stats::SetPackedBool(36840, true);
			Stats::SetInt("MPX_PROG_HUB_ADV_WINS", 50);
			Stats::SetInt("MPX_PROG_HUB_SURVIVALS_PLAYED", 1);
			for (int i = 41331; i <= 41506; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_PROG_HUB_SURV_WAVES", 250);
			Stats::SetPackedBool(42023, true);
			Stats::SetInt("MPX_PROG_HUB_T_TRIAL_PAR_TIME", -1);
			Stats::SetInt("MPX_PROG_HUB_T_TRL_PAR_TIME_RC", -1);
			Stats::SetInt("MPX_PROG_HUB_T_TRL_PAR_TIME_HSW", -1);
			Stats::SetInt("MPX_PROG_HUB_T_TRL_PAR_TIME_BKE", -1);
			Stats::SetPackedInt(41246, 20);
			Stats::SetInt("MPX_PROG_HUB_FST_LPS_RSTAR_RAC", 50);
			Stats::SetInt("MPX_DM_END", 1);
			Stats::SetInt("MPX_PROG_HUB_DM_TDM_PLAYS", 1);
			Stats::SetInt("MPX_PROG_HUB_DM_COMMUNITY_PLAYS", 10);
			Stats::SetInt("MPX_PROG_HUB_DTHM_KILL_5_WO_DIE", 25);
			Stats::SetPackedInt(42092, 5);
			Stats::SetInt("MPX_HUB_VEH_ENTH_OWNED_VEHS", 99);
			Stats::SetInt("MPX_VEHICLES_CUSTOMISED", 50);
			for (int i = 36920; i <= 36922; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_PROG_HUB_WEAP_TYPE_ANSR", -1);
			Stats::SetPackedInt(41242, 255);
			for (int i = 36941; i <= 36942; ++i)
				Stats::SetPackedBool(i, true);
			Stats::SetInt("MPX_PROG_HUB_10_CHAL_ANSR", 10);
			for (int i = 15456; i <= 15460; ++i)
				Stats::SetPackedBool(i, true);
			Notifications::Show("UnlockEverything", "All stats applied", NotificationType::Success);
		}
	};

	class UnlockAllTattoos : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			FiberPool::queueJob([] {
				bool isOnline =
				    NETWORK::NETWORK_IS_SESSION_STARTED() && !NETWORK::NETWORK_IS_IN_TRANSITION() && !STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS();

				if (!isOnline)
				{
					Notifications::Show("Offline", "Please join any freemode session");
					return;
				}

				const std::pair<const char*, int> intStats[] = {
				    {"MPX_AWD_CAR_BOMBS_ENEMY_KILLS", 25},
				    {"MPX_AWD_HOLD_UP_SHOPS", 20},
				    {"MPX_AWD_FMBBETWIN", 50000},
				    {"MPX_AWD_100_HEADSHOTS", 500},
				    {"MPX_AWD_FM_DM_WINS", 50},
				    {"MPX_AWD_RACES_WON", 50},
				    {"MPX_AWD_FMREVENGEKILLSDM", 50},
				    {"MPX_AWD_FM_DM_TOTALKILLS", 500},
				    {"MPX_LAP_DANCED_BOUGHT", 25},
				    {"MPX_AWD_FM_TDM_MVP", 50},
				    {"MPX_AWD_FMKILLBOUNTY", 25},
				    {"MPX_SNIPERRFL_ENEMY_KILLS", 100},
				    {"MPX_KILLS_PLAYERS", 1000},
				    {"MPX_TATTOO_FM_CURRENT_32", -1}};

				const char* boolStats[] = {
				    "MPX_AWD_FMWINEVERYGAMEMODE",
				    "MPX_AWD_FMMOSTKILLSSURVIVE",
				    "MPX_AWD_FMRACEWORLDRECHOLDER",
				    "MPX_AWD_FMATTGANGHQ",
				    "MPX_AWD_FMKILL3ANDWINGTARACE",
				    "MPX_AWD_FMKILLSTREAKSDM",
				    "MPX_AWD_FMFULLYMODDEDCAR"};

				for (auto& s : intStats)
					Stats::SetInt(s.first, s.second);

				for (auto s : boolStats)
					Stats::SetBool(s, true);

				int packed[] = {15737, 15738, 15748, 15887, 15894, 15898, 15905};
				for (int p : packed)
					Stats::SetPackedBool(p, true);

				for (int i = 0; i <= 53; i++)
					Stats::SetInt("MPX_TATTOO_FM_UNLOCKS_" + std::to_string(i), -1);

				for (int i = 41273; i <= 41296; i++)
					Stats::SetPackedBool(i, true);

				Notifications::Show("Tattoos", "Success! All tattoos unlocked.");
			});
		}
	};

	class UnlockSpecialItems : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			// --- Basic awards ---
			Stats::SetInt("AWD_TAXIDRIVER", 100);
			Stats::SetInt("MPX_AWD_WIN_CAPTURES", 50);
			Stats::SetInt("MPX_AWD_DROPOFF_CAP_PACKAGES", 100);
			Stats::SetInt("MPX_AWD_KILL_CARRIER_CAPTURE", 100);
			Stats::SetInt("MPX_AWD_NIGHTVISION_KILLS", 100);
			Stats::SetInt("MPX_AWD_WIN_LAST_TEAM_STANDINGS", 50);
			Stats::SetInt("MPX_AWD_ONLY_PLAYER_ALIVE_LTS", 50);

			// Masked stat (unchanged)
			STATS::STAT_SET_MASKED_INT("MPX_DLC22022PSTAT_INT536"_J, 10, 16, 8, true);

			// Xmas plates
			Stats::SetInt("MPPLY_XMAS23_PLATES0", -1);

			for (int i = 0; i <= 20; ++i)
				Stats::SetInt("MPPLY_XMASLIVERIES" + std::to_string(i), -1);

			// Tunable unlock
			*ScriptGlobal(262145).At(33811).As<int*>() = 1;

			// ---- Packed unlocks ----
			static const int packedUnlocks[] = {
			    // Weapon finishes
			    36788,
			    36787,
			    41657,
			    42069,
			    36786,
			    42122,
			    42068,
			    41658,
			    41659,

			    // Arena War / vehicles / upgrades (full list preserved)
			    25134,
			    25137,
			    25138,
			    25155,
			    25156,
			    25157,
			    25168,
			    25169,
			    25170,
			    25209,
			    25212,
			    25213,
			    25214,
			    25485,
			    25486,
			    25487,
			    25488,
			    25489,
			    25490,
			    25491,
			    25173,
			    25174,
			    25175,
			    25431,
			    25432,
			    25433,
			    25434,
			    25435,
			    25436,
			    25437,
			    25438,
			    25439,
			    25440,
			    25441,
			    25442,
			    25443,
			    25444,
			    25445,
			    25446,
			    25447,
			    25448,
			    25449,
			    25450,
			    25451,
			    25452,
			    25453,
			    25149,
			    25150,
			    25154,
			    25198,
			    25202,
			    25203,
			    25204,
			    25473,
			    25158,
			    25159,
			    25163,
			    25217,
			    25221,
			    25222,
			    25223,
			    25492,
			    25493,
			    25494,
			    25495,
			    25496,
			    25497,
			    25498,
			    25499,
			    25500,
			    25501,
			    25502,
			    25503,
			    25504,
			    25505,
			    25506,
			    25507,
			    25508,
			    25509,
			    25510,
			    25128,
			    25129,
			    25190,
			    25194,
			    25195,
			    25196,
			    25197,
			    25199,
			    25200,
			    25201,
			    25205,
			    25206,
			    25207,
			    25208,
			    25210,
			    25211,
			    25215,
			    25216,
			    25218,
			    25219,

			    // Brutus
			    25131,
			    25132,
			    25133,
			    25160,
			    25161,
			    25162,

			    // Neon lights
			    24980,
			    24981,
			    24982,
			    24983,
			    24984,
			    24985,
			    24986,
			    24987,
			    24988,
			    24989,
			    24990,
			    24991,

			    // Mines & upgrades
			    25231,
			    25233,
			    25234,
			    25235,
			    25236,
			    25394,
			    25395,
			    25396,
			    25397,
			    25398,
			    25399,
			    25400,

			    // Sasquatch / Bruiser / Slamvan / Dominator / Impaler
			    24971,
			    25113,
			    25114,
			    25117,
			    25118,
			    25119,
			    25167,
			    25171,
			    25172,
			    25176,
			    25177,
			    25181,
			    25182,
			    25186,
			    25187,
			    25189,
			    25121,
			    25122,
			    25123,
			    25136,
			    25139,
			    25140,
			    25141,
			    25220,
			    25224,
			    25226,
			    25227,
			    25232,
			    25372,
			    25376,
			    25380,
			    25381,
			    25384,
			    25385,
			    25387,
			    25388,
			    25389,
			    25407,
			    25408,
			    25409,
			    25410,
			    25124,
			    25126,
			    25130,
			    25148,
			    25151,
			    25152,
			    25153,
			    25411,
			    25412,
			    25413,
			    25414,
			    25415,
			    25416,
			    25417,
			    25418,
			    25419,
			    25142,
			    25143,
			    25144,
			    25179,
			    25183,
			    25184,
			    25185,
			    25454,
			    25455,
			    25456,
			    25457,
			    25458,
			    25459,
			    25460,
			    25461,
			    25462,
			    25463,
			    25464,
			    25145,
			    25146,
			    25147,
			    25188,
			    25191,
			    25192,
			    25193,

			    // Bunker Research
			    15381,
			    15382,
			    15428,
			    15429,
			    15430,
			    15431,
			    15432,
			    15433,
			    15434,
			    15435,
			    15436,
			    15437,
			    15438,
			    15439,
			    15447,
			    15448,
			    15449,
			    15450,
			    15451,
			    15452,
			    15453,
			    15454,
			    15455,
			    15456,
			    15457,
			    15458,
			    15459,
			    15460,
			    15461,
			    15462,
			    15463,
			    15464,
			    15465,
			    15466,
			    15467,
			    15468,
			    15469,
			    15470,
			    15471,
			    15472,
			    15473,
			    15474,
			    15491,
			    15492,
			    15493,
			    15494,
			    15495,
			    15496,
			    15497,
			    15498,
			    15499,

			    // Wheels + liveries
			    31831,
			    31846,
			    31861,
			    31876,
			    31891,
			    31904,
			    31909,
			    31914,
			    31919,
			    31924,
			    31836,
			    31906,
			    31943,
			    31968,
			    31993,
			    31901,
			    31941,
			    31966,
			    31991,
			    32016,
			    31866,
			    31926,
			    31953,
			    31978,
			    32003,
			    31856,
			    31921,
			    31951,
			    31976,
			    32001,
			    31881,
			    31933,
			    31958,
			    31983,
			    32008,
			    31886,
			    31936,
			    31961,
			    31986,
			    32011,
			    31841,
			    31911,
			    31946,
			    31971,
			    31996,
			    31851,
			    31916,
			    31948,
			    31973,
			    31998,
			    31871,
			    31931,
			    31956,
			    31981,
			    32006,
			    31896,
			    31938,
			    31963,
			    31988,
			    32013};

			for (int id : packedUnlocks)
				Stats::SetPackedBool(id, TRUE);

			Notifications::Show("Unlock Special Items", "All special items unlocked successfully", NotificationType::Success);
		}
	};

	class UnlockAllParachutes : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			Stats::SetPackedBool(3609, TRUE);
			Stats::SetPackedBool(31791, TRUE);
			Stats::SetPackedBool(31792, TRUE);
			Stats::SetPackedBool(31793, TRUE);
			Stats::SetPackedBool(31794, TRUE);
			Stats::SetPackedBool(31795, TRUE);
			Stats::SetPackedBool(31796, TRUE);
			Stats::SetPackedBool(34378, TRUE);
			Stats::SetPackedBool(34379, TRUE);
			Stats::SetPackedBool(31791, TRUE);
			Stats::SetPackedBool(31792, TRUE);
			Stats::SetPackedBool(31793, TRUE);
		}
	};

	class FastRunReloadUnlock : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			Stats::SetInt("MPX_CHAR_FM_ABILITY_1_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_ABILITY_1_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_ABILITY_2_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_ABILITY_2_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_FM_ABILITY_3_UNLCK", -1);
			Stats::SetInt("MPX_CHAR_ABILITY_3_UNLCK", -1);
		}
	};

	class UnlockAchievements : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			for (int i = 0; i <= 77; i++)
			{
				PLAYER::GIVE_ACHIEVEMENT_TO_PLAYER(i);
			}
		}
	};

	class UnlockMasks : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			auto base = ScriptGlobal(262145);
			for (int i = 34625; i <= 34643; ++i)
				*base.At(i).As<int*>() = 1;

			*ScriptGlobal(262145).At(12027).As<int*>() = 1;

			static const int unlocks[] = {
			    51222,
			    51223,
			    51224,
			    51225,
			    51226,
			    51227,
			    51228,
			    51229,
			    51230,
			    51231,
			    51232,
			    51233,
			    25178,
			    25225,
			    25259,
			    25260,
			    25261,
			    25262,
			    25263,
			    31783,
			    25264,
			    25371,
			    25373,
			    25374,
			    25375,
			    25377,
			    25378,
			    25379,
			    25382,
			    25383,
			    25386,
			    25390,
			    25391,
			    25392,
			    25393,
			    27087,
			    27088,
			    31826,
			    31842,
			    31860,
			    31877,
			    31893,
			    31910,
			    31927,
			    31950,
			    31975,
			    31997,
			    32017,
			    32094,
			    3599,
			    25368,
			    25369,
			    25370,
			    3598,
			    3597,
			    3596,
			    3595,
			    3594,
			    4256,
			    4255,
			    4254,
			    4253,
			    4252,
			    4251,
			    4250,
			    4249,
			    4248,
			    4247,
			    30293,
			    30292,
			    30291,
			    30290,
			    15401,
			    15388,
			    15391,
			    15413,
			    15397,
			    15411,
			    15408,
			    15400,
			    30587,
			    30588,
			    30589,
			    30590,
			    30591,
			    31766,
			    31767,
			    36780,
			    36781,
			    30684,
			    30683,
			    30687,
			    30691,
			    30690,
			    32298,
			    32303,
			    34429,
			    34424,
			    34421,
			    34416,
			    34704,
			    34423,
			    34419,
			    34422,
			    34418,
			    34415,
			    34437,
			    36758,
			    36761,
			    30611,
			    30609,
			    30614,
			    30605,
			    30615,
			    30639,
			    30657,
			    30640,
			    30672,
			    30643,
			    34469,
			    34477,
			    30598,
			    30594,
			    30602,
			    36766,
			    30626,
			    30621,
			    30627,
			    30625,
			    36777,
			    36778,
			    36779,
			    34390,
			    34391,
			    34392,
			    34393,
			    34730,
			    34731,
			    34732,
			    34733,
			    34734,
			    34735,
			    34736,
			    34737,
			    54628,
			    54624,
			    30613,
			    30616,
			    30631,
			    30645,
			    30652,
			    34445,
			    42192,
			    42202,
			    42210,
			    30593,
			    42212,
			    54622,
			    54630};

			for (int id : unlocks)
				Stats::SetPackedBool(id, TRUE);

			Notifications::Show("Unlock Masks", "All masks and cosmetic items unlocked", NotificationType::Success);
		}
	};

	class UnlockFlightSchool : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;
		virtual void OnCall() override
		{
			for (int i = 0; i < 10; ++i)
			{
				Stats::SetInt(("MPPLY_PILOT_SCHOOL_MEDAL_" + std::to_string(i)).c_str(), -1);
				Stats::SetInt(("MPX_PILOT_SCHOOL_MEDAL_" + std::to_string(i)).c_str(), -1);
				Stats::SetBool(("MPX_PILOT_ASPASSEDLESSON_" + std::to_string(i)).c_str(), true);
			}
			Stats::SetInt("MPPLY_NUM_CAPTURES_CREATED", 100);
		}
	};

	class UnlockCollectables : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			FiberPool::queueJob([] {
				bool isOnline =
				    NETWORK::NETWORK_IS_SESSION_STARTED() && !NETWORK::NETWORK_IS_IN_TRANSITION() && !STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS();

				if (!isOnline)
				{
					Notifications::Show("Offline", "Please join any freemode session.");
					return;
				}

				auto setRange = [](int start, int end) {
					for (int i = start; i <= end; ++i)
						Stats::SetPackedBool(i, true);
				};

				setRange(26811, 26910); // Action Figures
				setRange(34262, 34361); // LD Organics
				setRange(30230, 30251); // Movie Props
				setRange(26911, 26964); // Playing Cards
				setRange(28099, 28148); // Signal Jammers
				setRange(36630, 36654); // Snowmen
				setRange(51302, 51337); // Yuanbao
				setRange(54737, 54761); // Lucky Clovers

				Notifications::Show("Success!", "All collectibles have been unlocked", NotificationType::Success);
			});
		}
	};


	class UnlockIndependenceDay : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			FiberPool::queueJob([] {
				bool isOnline = NETWORK::NETWORK_IS_SESSION_STARTED() && !NETWORK::NETWORK_IS_IN_TRANSITION() && !STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS();

				if (!isOnline)
				{
					Notifications::Show("Offline", "Please join any freemode session.", NotificationType::Error);
					return;
				}

				std::vector<int> tunables = {
				    -648209009,
				    1379333599,
				    906239058,
				    -2144357830,
				    865790036,
				    1172933873,
				    -975368998,
				    -349939864,
				    -63833725,
				    234593558,
				    322731716,
				    -1484555665,
				    1299691672,

				    12134, // Independence Masks
				    8521,  // Monster Truck
				    8520,  // Western Sovereign
				    8544,  // Pisswasser Hat
				    8545,  // Benedict
				    8546,  // Jlager
				    8547,  // Patriot
				    8548,  // Blarneys
				    8549,  // Supa Wat Hat
				    8506   // Enable Independence Pack
				};

				auto get_tunable = [](int idx) -> int {
					return *ScriptGlobal(262145 + idx).As<int*>();
				};

				auto set_tunable = [](int idx, int value) {
					*ScriptGlobal(262145 + idx).As<int*>() = value;
				};

				bool enabled = get_tunable(-648209009) == 1;

				for (int idx : tunables)
					set_tunable(idx, enabled ? 0 : 1);

				if (!enabled)
					Notifications::Show("Independence Day", "Enabled. Rejoin session to apply.", NotificationType::Success);
				else
					Notifications::Show("Independence Day", "Disabled. Rejoin session to apply.", NotificationType::Success);
			});
		}
	};


	class ToggleHalloweenContent : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			FiberPool::queueJob([] {
				bool isOnline = NETWORK::NETWORK_IS_SESSION_STARTED() && !NETWORK::NETWORK_IS_IN_TRANSITION() && !STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS();

				if (!isOnline)
				{
					Notifications::Show("Offline", "Please join any freemode session.", NotificationType::Error);
					return;
				}

				std::vector<int> tunables = {
				    12113, // ENABLE_HEIST_MASKS_HALLOWEEN
				    12118, // TURN_ON_HALLOWEEN_VEHICLES
				    12119, // TURN_ON_HALLOWEEN_MASKS
				    12120, // TURN_ON_HALLOWEEN_FACEPAINT
				    12122, // TURN_ON_HALLOWEEN_BOBBLEHEADS
				    12128, // TURN_ON_HALLOWEEN_CLOTHING
				    12130, // TURN_ON_HALLOWEEN_HORNS
				    17459  // ENABLE_BIKER_SANCTUS
				};

				auto get = [](int idx) -> int {
					return *ScriptGlobal(262145 + idx).As<int*>();
				};

				auto set = [](int idx, int value) {
					*ScriptGlobal(262145 + idx).As<int*>() = value;
				};

				bool enabled = get(12113) == 1;

				for (int idx : tunables)
					set(idx, enabled ? 0 : 1);

				if (!enabled)
					Notifications::Show("Halloween", "Halloween content ENABLED. Rejoin session.", NotificationType::Success);
				else
					Notifications::Show("Halloween", "Halloween content DISABLED. Rejoin session.", NotificationType::Success);
			});
		}
	};

	class ToggleXmasContent : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			FiberPool::queueJob([] {
				bool isOnline = NETWORK::NETWORK_IS_SESSION_STARTED() && !NETWORK::NETWORK_IS_IN_TRANSITION() && !STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS();

				if (!isOnline)
				{
					Notifications::Show("Offline", "Please join any freemode session.", NotificationType::Error);
					return;
				}

				std::vector<int> tunables = {
				    4424, // Main Xmas Unlock

				    12785, // Xmas 2015 Masks
				    12786, // Xmas 2015 Costumes
				    12787, // Xmas 2015 Pyjamas
				    13224, // Xmas 2015 Horns
				    13227, // Xmas 2015 Beast Mask

				    19112, // Xmas 2016 Masks
				    19113, // Xmas 2016 Clothing

				    22797, // Xmas 2017 Masks
				    22798, // Xmas 2017 Clothing

				    25592 // Xmas 2018 Clothing
				};

				auto get = [](int idx) -> int {
					return *ScriptGlobal(262145 + idx).As<int*>();
				};

				auto set = [](int idx, int value) {
					*ScriptGlobal(262145 + idx).As<int*>() = value;
				};

				bool enabled = get(4424) == 1;

				for (int idx : tunables)
					set(idx, enabled ? 0 : 1);

				if (!enabled)
					Notifications::Show("Christmas", "Xmas content ENABLED. Rejoin session.", NotificationType::Success);
				else
					Notifications::Show("Christmas", "Xmas content DISABLED. Rejoin session.", NotificationType::Success);
			});
		}
	};


	static SetRank _SetRank{"setrank", "Set Rank", "Set your RP level"};
	static SetCrewRank _SetCrewRank{"setcrewrank", "Set Crew Rank", "Set your crew rank"};
	static BunkerResearch _BunkerResearch{"bunkerresearch", "Unlock Bunker Research", "Unlocks all Bunker Research Items"};
	static UnlockClothing _UnlockClothing{"unlockclothing", "Unlock Clothing", "Unlocks some Clothing"};
	static UnlockCareerProgressAwards _UnlockCareerProgressAwards{"unlockcareerprogressawards", "Unlock Career Progress and Awards", "Unlocks all Awards"};
	static UnlockEverything _UnlockEverything{"unlockeverything", "Unlock Everything", "Unlocks All"};
	static UnlockAllTattoos _UnlockAllTattoos{"unlockalltattoos", "Unlock Tattoos", "Unlocks Some Tattoos"};
	static UnlockSpecialItems _UnlockSpecialItems{"unlockspecialitems", "Unlock Special Items", "Unlocks Special items like livery, Vech mods and Guns"};
	static UnlockAchievements _UnlockAchievements{"unlockachievements", "Unlock SC Achievements", "Unlocks all Social Club Achievements"};
	static UnlockAllParachutes _UnlockAllParachutes{"unlockallparachutes", "Unlock All Parachutes", "Unlocks all Parachutes"};
	static FastRunReloadUnlock _FastRunReloadUnlock{"fastrunreloadunlock", "Unlock Fast Run", "Unlocks Fast Run"};
	static UnlockMasks _UnlockMasks{"unlockmasks", "Unlock Masks", "Unlocks Some Masks"};
	static UnlockFlightSchool _UnlockFlightSchool{"unlockflightschool", "Unlock Flight School", "Unlocks Flight School"};
	static UnlockCollectables _UnlockCollectables{"unlockcollectables", "Unlock Collectables", "Unlocks Some Collectables"};
	static UnlockIndependenceDay _UnlockIndependenceDay("unlockindependenceday", "Unlock Independence Day Content", "Enables or disables Independence Day DLC content");
	static ToggleHalloweenContent g_ToggleHalloween("togglehalloween", "Toggle Halloween Content", "Enables or disables Halloween tunables");
	static ToggleXmasContent g_ToggleXmas("togglexmas", "Toggle Christmas Content", "Enables or disables all Christmas tunables (rejoin required)");

}
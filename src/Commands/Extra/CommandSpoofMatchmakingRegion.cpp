#include "Commands/LoopedCommand.hpp"
#include "Commands/CommandListSelect.hpp"
#include "Core/Pointers.hpp"

namespace Stand::Features
{
	static std::vector<std::pair<int, const char*>> g_RegionCodes = {
	    {0, "CIS"},
	    {1, "South America"},
	    {2, "US East"},
	    {3, "Europe"},
	    {4, "China"},
	    {5, "Australia"},
	    {6, "US West"},
	    {7, "Japan"},
	    {8, "Unknown"},
	};

	static CommandListSelect _SpoofedRegion{"mmregion", "Matchmaking Region", "Matchmaking region to spoof to", g_RegionCodes, 0};

	class SpoofMatchmakingRegion : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		int m_OldRegion;

		virtual void OnEnable() override
		{
			m_OldRegion = *Pointers.RegionCode;
		}

		virtual void OnTick() override
		{
			*Pointers.RegionCode = _SpoofedRegion.GetState();
		}

		virtual void OnDisable() override
		{
			*Pointers.RegionCode = m_OldRegion;
		}
	};

	static SpoofMatchmakingRegion _SpoofMatchmakingRegion{"spoofmmregion", "Spoof Matchmaking Region", "Change the region you matchmake in to the specified value"};
}
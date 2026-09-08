#include "Commands/CommandListSelect.hpp"
#include "World/Self.hpp"
#include "Scripting/ScriptGlobal.hpp"
#include "Core/Pointers.hpp"
#include "Network/GPBD_FM.hpp"
#include "Network/GPBD_FM_2.hpp"

namespace Stand::Features
{
	enum class eAppVinewoodMenuSafe
	{
		NIGHTCLUB,
		ARCADE,
		AGENCY,
		SALVAGE_YARD,
		BAIL_OFFICE,
		GARMENT_FACTORY,
		HANDS_ON_CAR_WASH
	};

	static std::vector<std::pair<int, const char*>> businessNames = {
	    {static_cast<int>(eAppVinewoodMenuSafe::NIGHTCLUB), "Nightclub"},
	    {static_cast<int>(eAppVinewoodMenuSafe::ARCADE), "Arcade"},
	    {static_cast<int>(eAppVinewoodMenuSafe::AGENCY), "Agency"},
	    {static_cast<int>(eAppVinewoodMenuSafe::SALVAGE_YARD), "Salvage Yard"},
	    {static_cast<int>(eAppVinewoodMenuSafe::BAIL_OFFICE), "Bail Office"},
	    {static_cast<int>(eAppVinewoodMenuSafe::GARMENT_FACTORY), "Garment Factory"},
	    {static_cast<int>(eAppVinewoodMenuSafe::HANDS_ON_CAR_WASH), "Hands on Car Wash"}};

	static CommandListSelect _SelectedBusiness{"businesssafe", "Business", "Business to claim earning from.", businessNames, 0};

	class ClaimSafeEarnings : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			if (!*Pointers.IsSessionStarted)
				return;

			switch (static_cast<eAppVinewoodMenuSafe>(_SelectedBusiness.GetState()))
			{
			case eAppVinewoodMenuSafe::NIGHTCLUB:
			{
				if (GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.NightclubData.SafeCashValue > 0)
					*ScriptGlobal(2708943).As<BOOL*>() = TRUE;
				break;
			}
			case eAppVinewoodMenuSafe::ARCADE:
			{
				if (GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.ArcadeData.SafeCashValue > 0)
					*ScriptGlobal(2708952).As<BOOL*>() = TRUE;
				break;
			}
			case eAppVinewoodMenuSafe::AGENCY:
			{
				if (GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.FixerHQData.SafeCashValue > 0)
					*ScriptGlobal(2708961).As<BOOL*>() = TRUE;
				break;
			}
			case eAppVinewoodMenuSafe::SALVAGE_YARD:
			{
				if (GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.SalvageYardData.TotalEarnings > 0)
					*ScriptGlobal(2708970).As<BOOL*>() = TRUE;
				break;
			}
			case eAppVinewoodMenuSafe::BAIL_OFFICE:
			{
				if (GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.BailShopData.SafeCashValue > 0)
					*ScriptGlobal(2708979).As<BOOL*>() = TRUE;
				break;
			}
			case eAppVinewoodMenuSafe::GARMENT_FACTORY:
			{
				if (GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.HackerDenData.SafeCashValue > 0)
					*ScriptGlobal(2708994).As<BOOL*>() = TRUE;
				break;
			}
			case eAppVinewoodMenuSafe::HANDS_ON_CAR_WASH:
			{
				if (GPBD_FM_2::Get()->Entries[Self::GetPlayer().GetId()].SYVehSaleData.HOWCData.SafeCashValue > 0)
					*ScriptGlobal(2709001).As<BOOL*>() = TRUE;
				break;
			}
			}
		}
	};

	static ClaimSafeEarnings _ClaimSafeEarnings{"claimsafeearnings", "Claim Safe Earnings", "Claims your safe earnings from the selected business."};
}
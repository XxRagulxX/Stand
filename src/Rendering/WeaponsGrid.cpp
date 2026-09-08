#include "Rendering/WeaponsGrid.hpp"

#include "Rendering/AmmuNationGrid.hpp"
#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/Weapons/CommandCustomWeapon.hpp"
#include "Commands/Commands.hpp"
#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/GridItemCommandColourCustom.hpp"
#include "Rendering/GridItemCommandSliderFloat.hpp"
#include "Rendering/GridItemCommandSlider.hpp"
#include "Rendering/GridItemCommandListSelect.hpp"
#include "Rendering/GridItemCommandInput.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/GridItemText.hpp"
#include "Util/Joaat.hpp"
#include "Commands/CommandListSelect.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// Weapons.cpp's own RenderCustomWeaponsMenu() nests
		// customWeapons (gated on customweapon) inside customWeaponsGroup,
		// then further nests cutomWeaponTypes' per-type rows and
		// paintGunGroup (gated on isPaintGunEnabled) inside that -
		// flattened here into combined predicates the same way SelfGrid's
		// Wanted group and VehicleGrid's autodrive rows are, since this
		// system has no literal "Group" container to wrap once (see
		// GridItemConditional's own class comment).
		bool IsCustomWeaponEnabled()
		{
			auto* customweapon = Commands::GetCommand<CommandToggleLegacy>("customweapon"_J);
			return customweapon && customweapon->GetState();
		}

		Features::CustomWeapons CustomWeaponType()
		{
			auto* cmd = Commands::GetCommand<CommandListSelect>("customweapontype"_J);
			return static_cast<Features::CustomWeapons>(cmd ? cmd->GetState() : 0);
		}

		// cutomWeaponTypes' three per-type rows: each needs customweapon
		// on (the group they're nested in) AND its own type predicate
		// (isGravityGunEnabled/isVehicleGunEnabled/isPaintGunEnabled in
		// the original).
		bool IsGravityGunRow()
		{
			return IsCustomWeaponEnabled() && CustomWeaponType() == Features::CustomWeapons::GRAVITY_GUN;
		}

		bool IsVehicleGunRow()
		{
			return IsCustomWeaponEnabled() && CustomWeaponType() == Features::CustomWeapons::VEHICLE_GUN;
		}

		bool IsPaintGunTypeRow()
		{
			return IsCustomWeaponEnabled() && CustomWeaponType() == Features::CustomWeapons::PAINT_GUN;
		}

		// paintguncolor's own row is nested one level deeper still -
		// isPaintGunEnabled AND paintgunrainbowcolorenabled being *off*
		// (the original's inner ConditionalItem negates it).
		bool IsPaintGunColorRow()
		{
			if (!IsPaintGunTypeRow())
				return false;

			auto* rainbow = Commands::GetCommand<CommandToggleLegacy>("paintgunrainbowcolorenabled"_J);
			return !(rainbow && rainbow->GetState());
		}

		// paintGunGroup's own two conditional rows (style/speed) both
		// need customweapon on, PAINT_GUN selected, AND
		// paintgunrainbowcolorenabled on - paintGunGroup itself is only
		// added at all when isPaintGunEnabled (PAINT_GUN selected), and
		// customweapon gates the whole customWeapons group it lives in.
		bool IsPaintGunRainbowRow()
		{
			if (!IsPaintGunTypeRow())
				return false;

			auto* rainbow = Commands::GetCommand<CommandToggleLegacy>("paintgunrainbowcolorenabled"_J);
			return rainbow && rainbow->GetState();
		}
	}

	// Origin (1438, 587) matches every other content Grid's. Spacer is
	// 0, not 3 - confirmed against real Stand's own source (origin/
	// stand-reference) that individual list rows have zero gap between
	// them; the 3-unit spacer real Stand does use is only ever between
	// distinct chrome pieces (addressbar/tabs/list), never between rows -
	// see the comment in MenuGrid.cpp's anonymous namespace for why (no
	// shared header for these yet). Each item below specifies its own
	// width (Theme::kContentWidth) rather than the Grid itself, matching
	// Stand's real Grid - see Grid.hpp's class comment.
	WeaponsGrid::WeaponsGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void WeaponsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// Globals (weaponsGlobalsGroup) - every conditional float/list row
		// is gated on the single CommandToggle right above it.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Globals", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "infiniteammo"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "infiniteclip"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "rapidfire"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "infiniteparachutes"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "ExplosiveAmmo"_J));
		if (watchCondition("ExplosiveAmmo"_J))
		{
			items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "selectedexplosion"_J));
			items_draft.push_back(std::make_unique<GridItemCommandSliderFloat>(Theme::kContentWidth, kItemH, "explosiondamage"_J));
			items_draft.push_back(std::make_unique<GridItemCommandSliderFloat>(Theme::kContentWidth, kItemH, "explosioncamerashake"_J));
		}
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "weapondamage"_J));
		if (watchCondition("weapondamage"_J))
			items_draft.push_back(std::make_unique<GridItemCommandSliderFloat>(Theme::kContentWidth, kItemH, "weapondamagescale"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "meleedamage"_J));
		if (watchCondition("meleedamage"_J))
			items_draft.push_back(std::make_unique<GridItemCommandSliderFloat>(Theme::kContentWidth, kItemH, "meleedamagescale"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "explosionradius"_J));
		if (watchCondition("explosionradius"_J))
			items_draft.push_back(std::make_unique<GridItemCommandSliderFloat>(Theme::kContentWidth, kItemH, "explosionradiusscale"_J));

		// Tools (weaponsToolsGroup) - all plain CommandItem buttons.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Tools", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "giveallweapons"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "givemaxammo"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "opengunlocker"_J));

		// Ammu-Nation (weaponsAmmuNationGroup) - see AddAmmuNationRows()'s
		// own doc comment for what it covers.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Ammu-Nation", Theme::kText));
		AddAmmuNationRows(*this, items_draft, Theme::kContentWidth);

		// Gun Van (weaponsGunVanGroup) - all three items are
		// unconditional (two ListCommandItems, now that
		// GridItemCommandListSelect exists, plus a plain CommandItem button).
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Gun Van", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "gunvan_weapon"_J));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "gunvan_slot"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "gunvan_apply"_J));

		// Aimbot (weaponsAimbotGroup) - the three head/drivers/dead-ped
		// toggles are all gated on aimbot directly.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Aimbot", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "aimbot"_J));
		if (watchCondition("aimbot"_J))
		{
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "aimbotaimforhead"_J));
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "aimbottargetdrivers"_J));
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "aimbotreleasedeadped"_J));
		}

		// Custom Weapons (RenderCustomWeaponsMenu()) - see this file's own
		// IsCustomWeaponEnabled()/CustomWeaponType()/IsGravityGunRow()/
		// IsVehicleGunRow()/IsPaintGunTypeRow()/IsPaintGunColorRow()/
		// IsPaintGunRainbowRow() for how the original's several levels of
		// nested ConditionalItem/Group flatten onto these rows' combined
		// predicates.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Custom Weapons", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "customweapon"_J));
		if (watchCondition(IsCustomWeaponEnabled))
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "customweaponenabledonweaponout"_J));
		if (watchCondition(IsCustomWeaponEnabled))
			items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "customweapontype"_J));
		if (watchCondition(IsGravityGunRow))
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "gravitygunlaunchonrelease"_J));
		if (watchCondition(IsVehicleGunRow))
			items_draft.push_back(std::make_unique<GridItemCommandInput>(Theme::kContentWidth, kItemH, "vehiclegunmodel"_J));
		AddConditionalColorCommandRows(*this, items_draft, Theme::kContentWidth, "paintguncolor"_J, IsPaintGunColorRow);
		if (watchCondition(IsPaintGunTypeRow))
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "paintgunrainbowcolorenabled"_J));
		if (watchCondition(IsPaintGunRainbowRow))
			items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "paintgunrainbowcolorstyle"_J));
		if (watchCondition(IsPaintGunRainbowRow))
			items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "paintgunrainbowcolorspeed"_J));
	}
}

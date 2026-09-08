#pragma once
#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/CommandColourCustom.hpp"
#include "Commands/CommandSliderLegacy.hpp"
#include "Commands/CommandListSelect.hpp"
#include "Commands/LoopedCommand.hpp"
#include "Commands/CommandInput.hpp"
#include "Rendering/RainbowColor.hpp"
#include "Game/ControllerInputs.hpp"

namespace Stand::Features
{
	enum class CustomWeapons
	{
		CAGE_GUN,
		DELETE_GUN,
		GRAVITY_GUN,
		STEAL_VEHICLE_GUN,
		REPAIR_GUN,
		VEHICLE_GUN,
		TELEPORT_GUN,
		PAINT_GUN
	};

	static const ControllerInputs attackControls[] = {
		ControllerInputs::INPUT_WEAPON_WHEEL_NEXT,
		ControllerInputs::INPUT_WEAPON_WHEEL_PREV,
		ControllerInputs::INPUT_ATTACK,
		ControllerInputs::INPUT_ATTACK2,
		ControllerInputs::INPUT_VEH_ATTACK,
		ControllerInputs::INPUT_VEH_ATTACK2,
		ControllerInputs::INPUT_VEH_PASSENGER_ATTACK,
		ControllerInputs::INPUT_VEH_FLY_ATTACK,
		ControllerInputs::INPUT_VEH_FLY_ATTACK2,
	};

	static std::vector<std::pair<int, const char*>> g_CustomWeaponTypes = {
		{static_cast<int>(CustomWeapons::CAGE_GUN), "Cage Gun"},
		{static_cast<int>(CustomWeapons::DELETE_GUN), "Delete Gun"},
		{static_cast<int>(CustomWeapons::GRAVITY_GUN), "Gravity Gun"},
		{static_cast<int>(CustomWeapons::STEAL_VEHICLE_GUN), "Steal Vehicle Gun"},
		{static_cast<int>(CustomWeapons::REPAIR_GUN), "Repair Gun"},
		{static_cast<int>(CustomWeapons::VEHICLE_GUN), "Vehicle Gun"},
		{static_cast<int>(CustomWeapons::TELEPORT_GUN), "Teleport Gun"},
		{static_cast<int>(CustomWeapons::PAINT_GUN), "Paint Gun"},
	};
	static CommandListSelect _CustomWeaponType{"customweapontype", "Weapon Type", "The type of custom weapon to use", g_CustomWeaponTypes, static_cast<int>(CustomWeapons::CAGE_GUN)};

	static std::vector<std::pair<int, const char*>> g_PaintGunRainbowColorStyles = {
		{static_cast<int>(RainbowColorStyle::Fade), "Fade"},
		{static_cast<int>(RainbowColorStyle::Spasm), "Spasm"}
	};
	static CommandListSelect _PaintGunRainbowColorStyle{"paintgunrainbowcolorstyle", "Rainbow Color Style", "Style of rainbow color for paint gun", g_PaintGunRainbowColorStyles, static_cast<int>(RainbowColorStyle::Fade)};

	class CustomWeapon : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override;
	};

	static CustomWeapon _CustomWeapon{"customweapon", "Custom Weapon", "Modifies weapon ability"};
	static CommandToggleLegacy _CustomWeaponEnabledOnWeaponOut{"customweaponenabledonweaponout", "Enable Only if Weapon is Out", "Enables custom weapon only when weapon is out", true};
	static CommandToggleLegacy _GravityGunLaunchOnRelease{"gravitygunlaunchonrelease", "Launch Entity on Release", "Launches entity when released", false};
	static CommandInput _VehicleGunModel{"vehiclegunmodel", "Vehicle Model", "Model of the vehicle"};
	static CommandColourCustom _PaintGunColor{"paintguncolor", "Paint Gun Color", "Color of the paint gun"};
	static CommandToggleLegacy _PaintGunRainbowColorEnabled{"paintgunrainbowcolorenabled", "Rainbow Color", "Enables rainbow color for paint gun", false};
	static CommandSliderLegacy _PaintGunRainbowColorSpeed{"paintgunrainbowcolorspeed", "Rainbow Color Speed", "Speed of rainbow color for paint gun", 1, 10, 1};
}
#pragma once

#include <cstdint>

#include "Util/Label.hpp"

namespace Stand
{
	struct VehicleMods
	{
		static inline constexpr int all[] = {
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
			18, 22, 23, 24, 25, 26, 27, 28, 29, 30, 32, 33, 34, 35, 36,
			37, 38, 39, 40, 41, 42, 43, 44, 45, 48,
		};

		static inline constexpr int performance[] = {
			11, 12, 13, 16, 18,
		};

		enum VehicleModTypes : int
		{
			spoiler,
			front_bumper,
			rear_bumper,
			sideskirt,
			exhaust,
			chassis,
			grille,
			hood,
			fender,
			right_fender,
			roof,
			engine,
			brakes,
			transmission,
			horns,
			suspension,
			armor,
			nitrous,
			turbo,
			subwoofer,
			tiresmoke,
			unk21,
			xenon_lights,
			front_wheels,
			back_wheels,
			plate_holder,
			vanity_plates,
			trim,
			ornaments,
			dashboard,
			dial,
			door_speaker,
			seats,
			steering_wheel,
			shifter_leavers,
			plaques,
			speakers,
			trunk,
			hydraulics,
			engine_block,
			air_filter,
			struts,
			arch_cover,
			aerials,
			trim2,
			tank,
			door_l,
			door_r,
			livery,
			lightbar,

			_NUM_TYPES
		};

		static inline constexpr int visual[] =
		{
			horns, livery, sideskirt, suspension, exhaust, chassis, front_bumper,
			rear_bumper, hood, roof, fender, right_fender, grille, plate_holder,
			vanity_plates, trim, ornaments, dashboard, dial, seats, door_speaker,
			steering_wheel, shifter_leavers, plaques, speakers, trunk, hydraulics,
			engine_block, air_filter, struts, arch_cover, aerials, trim2, tank,
			door_l, door_r, lightbar,
		};

		[[nodiscard]] static constexpr bool isBooleanIncludeLights(const int modType)
		{
			return modType >= 17 && modType <= 22;
		}

		[[nodiscard]] static constexpr bool isBooleanExcludeLights(const int modType)
		{
			return modType >= 17 && modType <= 21;
		}

		[[nodiscard]] static constexpr bool hasVariation(const int modType)
		{
			switch (modType)
			{
			case front_wheels:
			case back_wheels:
				return true;
			}
			return false;
		}

		[[nodiscard]] static Label getName(int modType);
	};
}

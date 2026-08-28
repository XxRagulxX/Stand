#pragma once

#include "Game/hashtype.hpp"
#include "Scripting/ScriptGlobal.hpp"

namespace Stand
{
	struct PersonalVehicle
	{
		uint32_t slot;

		constexpr operator uint32_t() const noexcept
		{
			return slot;
		}

		[[nodiscard]] constexpr ScriptGlobal getBase() const noexcept
		{
			return ScriptGlobal(GLOBAL_PVS).at(slot, GLOBAL_PVS_ELMSIZE);
		}

		[[nodiscard]] hash_t getStatHash(const char* key) const;

		[[nodiscard]] bool getFlag(int bit) const noexcept;
		void setFlag(int bit, bool val) const;
		void writeFlags() const;

		[[nodiscard]] bool getInsuranceFlag(int bit) const noexcept;
		void setInsuranceFlag(int bit, bool val) const;
		void writeInsuranceFlags() const;

		static void saveStats();
	};

	// fmt no longer treats an implicit conversion operator (operator
	// uint32_t() above) as enough to make a type formattable on its own -
	// it needs a format_as() overload it can find via ADL instead.
	constexpr auto format_as(const PersonalVehicle& v) noexcept -> uint32_t
	{
		return v.slot;
	}
}

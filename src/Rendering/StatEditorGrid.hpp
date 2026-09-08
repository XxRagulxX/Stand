#pragma once
#include "Rendering/Grid.hpp"
#include "World/CStatsMgr.hpp"

#include <cstdint>
#include <string>
#include <string_view>

namespace Stand::Rendering
{
	class GridItemTextInput;

	// Content grid for Recovery > Stat Editor: mirrors World/StatEditor.cpp's
	// BuildStatEditorMenu() - Regular (name lookup + typed value editor),
	// Packed, Packed Range, and From Clipboard, in that order, all on one
	// page (the original's own four Groups sit side by side in a single
	// Category, not separate navigable panels, so there's no folder-nesting
	// here either).
	//
	// StatEditor.cpp's own StatInfo/PackedStatInfo/StatValue and its
	// ResolveStat()/ReadStat()/WriteStat()/etc. helpers are all file-
	// static (internal linkage) there, so this class carries its own
	// equivalents (nested types below) rather than reaching into that
	// file - the same "mirror the original's logic, don't reuse it"
	// approach every other port in this series has taken (e.g.
	// SpawnPedGrid vs SpawnPed.cpp).
	//
	// Resolving a new stat name/packed index (or Refresh, which re-reads
	// into the same scratch value) always invalidate()s - the value-edit
	// row's own type (and whether it shows at all) depends on what got
	// resolved, and populate() only runs once (see Grid.hpp's own class
	// comment), same discipline as GlobalsGrid/LocalsGrid's own
	// onShapeChanged.
	//
	// The original's "Write" button, when the stat is
	// IsControlledByNetshop(), stays enabled but disabled-looking with a
	// right-click-to-force-anyway + hover tooltip explaining why - this
	// system has no disabled-button-still-clickable state, no right-click
	// gesture, and no hover tooltip (the whole menu is keyboard-only, see
	// GridRenderer's own no-mouse design). Replaced with two mutually
	// exclusive buttons instead: "Write" (shown only when editable) and
	// "Force Write" (shown only when it's netshop-controlled) - same
	// underlying action either way, just reached through a visible,
	// always-focusable row rather than a disabled one.
	class StatEditorGrid : public Grid
	{
	public:
		StatEditorGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		struct StatInfo
		{
			std::string name;
			std::uint32_t name_hash = 0;
			bool normalized = false;
			sStatData* data = nullptr;

			bool IsValid() const
			{
				return data != nullptr;
			}
		};

		struct PackedStatInfo
		{
			int index = 0;
			bool is_bool_stat = false;
			bool is_valid = false;

			bool IsValid() const
			{
				return is_valid;
			}
		};

		union StatValue {
			float as_float;
			int as_int;
			bool as_bool;
			std::uint64_t as_u64;
			char as_string[12];
		};

		static StatInfo ResolveStat(std::string_view name);
		static void ReadStat(StatValue& value, sStatData* data);
		static void WriteStat(std::uint32_t hash, const StatValue& value, sStatData* data);
		static void WriteStatWithStringValue(std::uint32_t hash, std::string_view value, sStatData* data);

		static PackedStatInfo ResolvePackedStat(int index);
		static void ReadPackedStat(StatValue& value, const PackedStatInfo& info);
		static void WritePackedStat(const StatValue& value, const PackedStatInfo& info);
		static void WritePackedStatRange(int start, int end, int value);

		void AddStatValueEditRow(std::vector<std::unique_ptr<GridItem>>& items_draft, int16_t width, int16_t height, sStatData::Type type, StatValue& value);

		// Regular
		// m_StatNameRaw is what the user actually typed (stat_buf's own
		// role in the original) - kept separately from m_CurStat.name,
		// which ResolveStat() may rewrite (a leading "$" stripped, an
		// "MPx"/character-less name normalized to a real MP0_/MP1_
		// name), so the Name field keeps showing what was typed rather
		// than snapping to the resolved name on every repopulate - the
		// original shows both side by side too (stat_buf's own text
		// field plus a separate "Normalized name to: X" line).
		std::string m_StatNameRaw;
		StatInfo m_CurStat{};
		StatValue m_CurValue{};

		// Packed
		PackedStatInfo m_CurPacked{};
		StatValue m_CurPackedValue{};

		// Packed Range
		int m_RangeStart = 0;
		int m_RangeEnd = 0;
		int m_RangeValue = 0;
	};
}

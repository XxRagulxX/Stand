#include "Rendering/StatEditorGrid.hpp"

#include "Core/Pointers.hpp"
#include "Rendering/Clipboard.hpp"
#include "Rendering/GridItemBoundToggle.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemFloatStepper.hpp"
#include "Rendering/GridItemIntStepper.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/GridItemTextInput.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Invoker.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Joaat.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <format>
#include <limits>
#include <ranges>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// Same trim StatEditor.cpp's own TrimString(std::string_view) does
		// (see that file's own comment linking to the stackoverflow answer
		// this is from) - a distinct name from Localization/Strings.hpp's
		// TrimString(std::string&)/TrimString(char*), which trim in place
		// rather than returning a view.
		std::string_view TrimStringView(std::string_view string)
		{
			return std::string_view{
			    std::ranges::find_if_not(
			        string,
			        [](auto c) {
				        return std::isspace(c);
			        }),
			    std::ranges::find_if_not(
			        string | std::views::reverse,
			        [](auto c) {
				        return std::isspace(c);
			        })
			        .base()};
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
	StatEditorGrid::StatEditorGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	// Mirrors StatEditor.cpp's own GetStatInfo() exactly.
	StatEditorGrid::StatInfo StatEditorGrid::ResolveStat(std::string_view name_str)
	{
		StatInfo info{};
		auto len = name_str.length();

		// not sure why people do this
		if (len > 1 && name_str[0] == '$')
		{
			name_str = name_str.substr(1);
			len--;
			info.normalized = true;
		}

		info.name = name_str;

		if (len > 3 && tolower(name_str[0]) == 'm' && tolower(name_str[1]) == 'p' && tolower(name_str[2]) == 'x')
		{
			if (auto* last_char = Pointers.StatsMgr->GetStat("MPPLY_LAST_MP_CHAR"_J))
			{
				info.name[2] = static_cast<char>('0' + last_char->GetInt());
				info.normalized = true;
			}
		}

		info.name_hash = Joaat(info.name);
		info.data = Pointers.StatsMgr->GetStat(info.name_hash);

		if (info.data == nullptr && len > 3 && (tolower(name_str[0]) != 'm' || tolower(name_str[1]) != 'p' || !(tolower(name_str[2]) == '0' || tolower(name_str[2]) == '1')))
		{
			// stat names without a character prefix
			auto* last_char = Pointers.StatsMgr->GetStat("MPPLY_LAST_MP_CHAR"_J);
			auto char_index = last_char ? last_char->GetInt() : 0;
			auto* char_prefix = char_index == 0 ? "MP0_" : "MP1_";
			auto new_hash = Joaat(char_prefix + info.name);
			auto* new_stat = Pointers.StatsMgr->GetStat(new_hash);

			if (new_stat)
			{
				info.name = char_prefix + info.name;
				info.name_hash = new_hash;
				info.data = new_stat;
				info.normalized = true;
			}
		}

		return info;
	}

	// Mirrors StatEditor.cpp's own ReadStat() exactly.
	void StatEditorGrid::ReadStat(StatValue& value, sStatData* data)
	{
		std::memset(&value, 0, sizeof(StatValue));

		switch (data->GetType())
		{
		case sStatData::Type::_BOOL:
			value.as_bool = data->GetBool();
			return;
		case sStatData::Type::FLOAT:
			value.as_float = data->GetFloat();
			return;
		case sStatData::Type::INT:
		case sStatData::Type::UINT32:
		case sStatData::Type::UINT16:
		case sStatData::Type::UINT8:
			value.as_int = data->GetInt();
			return;
		case sStatData::Type::INT64:
			value.as_u64 = data->GetInt64();
			return;
		case sStatData::Type::UINT64:
			value.as_u64 = data->GetUInt64();
			return;
		case sStatData::Type::STRING:
			std::strncpy(value.as_string, data->GetString(), sizeof(value.as_string));
			return;
		default:
			return; // data type not supported
		}
	}

	// Mirrors StatEditor.cpp's own WriteStat() exactly.
	void StatEditorGrid::WriteStat(std::uint32_t hash, const StatValue& value, sStatData* data)
	{
		switch (data->GetType())
		{
		case sStatData::Type::_BOOL:
			STATS::STAT_SET_BOOL(hash, value.as_bool, true);
			return;
		case sStatData::Type::FLOAT:
			STATS::STAT_SET_FLOAT(hash, value.as_float, true);
			return;
		case sStatData::Type::INT:
		case sStatData::Type::UINT32:
		case sStatData::Type::UINT16:
		case sStatData::Type::UINT8:
			STATS::STAT_SET_INT(hash, value.as_int, true);
			return;
		case sStatData::Type::INT64:
			data->SetInt64(value.as_u64); // TODO this isn't a good idea! natives can't set this
			return;
		case sStatData::Type::UINT64:
			STATS::STAT_SET_MASKED_INT(hash, (std::uint32_t)value.as_u64, 0, 32, true);
			STATS::STAT_SET_MASKED_INT(hash, (std::uint32_t)(value.as_u64 >> 32), 32, 32, true);
			return;
		case sStatData::Type::STRING:
			STATS::STAT_SET_STRING(hash, value.as_string, true);
			return;
		default:
			return; // data type not supported
		}
	}

	// Mirrors StatEditor.cpp's own WriteStatWithStringValue() exactly.
	// TODO: don't call std::string_view::data()
	void StatEditorGrid::WriteStatWithStringValue(std::uint32_t hash, std::string_view value, sStatData* data)
	{
		switch (data->GetType())
		{
		case sStatData::Type::_BOOL:
		{
			bool as_bool = false;
			std::string as_string(value);
			std::transform(as_string.begin(), as_string.end(), as_string.begin(), [](char c) {
				return tolower(c);
			});

			if (as_string == "true" || as_string == "1")
				as_bool = true;

			STATS::STAT_SET_BOOL(hash, as_bool, true);
			return;
		}
		case sStatData::Type::FLOAT:
		{
			auto as_float = std::strtof(value.data(), nullptr);
			STATS::STAT_SET_FLOAT(hash, as_float, true);
			return;
		}
		case sStatData::Type::INT:
		case sStatData::Type::UINT32:
		case sStatData::Type::UINT16:
		case sStatData::Type::UINT8:
		{
			auto as_int = std::strtol(value.data(), nullptr, 10);
			STATS::STAT_SET_INT(hash, as_int, true);
			return;
		}
		case sStatData::Type::INT64:
		{
			auto as_int64 = std::strtoll(value.data(), nullptr, 10);
			data->SetInt64(as_int64); // TODO this isn't a good idea! natives can't set this
			return;
		}
		case sStatData::Type::UINT64:
		{
			auto as_uint64 = std::strtoull(value.data(), nullptr, 10);

			STATS::STAT_SET_MASKED_INT(hash, (std::uint32_t)as_uint64, 0, 32, true);
			STATS::STAT_SET_MASKED_INT(hash, (std::uint32_t)(as_uint64 >> 32), 32, 32, true);
			return;
		}
		case sStatData::Type::STRING:
			STATS::STAT_SET_STRING(hash, value.data(), true);
			return;
		default:
			return; // data type not supported
		}
	}

	// Mirrors StatEditor.cpp's own GetPackedStatInfo() exactly.
	StatEditorGrid::PackedStatInfo StatEditorGrid::ResolvePackedStat(int index)
	{
		PackedStatInfo info{};
		int row;
		bool unk;

		info.index = index;
		Pointers.GetPackedStatData(index, &row, &info.is_bool_stat, &unk);

		if (row != 0 || index <= 191)
			info.is_valid = true;

		return info;
	}

	// Mirrors StatEditor.cpp's own ReadPackedStat() exactly.
	void StatEditorGrid::ReadPackedStat(StatValue& value, const PackedStatInfo& info)
	{
		if (info.is_bool_stat)
			value.as_bool = STATS::GET_PACKED_STAT_BOOL_CODE(info.index, -1);
		else
			value.as_int = STATS::GET_PACKED_STAT_INT_CODE(info.index, -1);
	}

	// Mirrors StatEditor.cpp's own WritePackedStat() exactly.
	void StatEditorGrid::WritePackedStat(const StatValue& value, const PackedStatInfo& info)
	{
		if (info.is_bool_stat)
			STATS::SET_PACKED_STAT_BOOL_CODE(info.index, value.as_bool, -1);
		else
			STATS::SET_PACKED_STAT_INT_CODE(info.index, value.as_int, -1);
	}

	// Mirrors StatEditor.cpp's own WritePackedStatRange() exactly -
	// including only ever writing the bool-stat entries in the range,
	// silently skipping any non-bool index (the original's own behaviour,
	// not something this port changed).
	void StatEditorGrid::WritePackedStatRange(int start, int end, int value)
	{
		for (int i = start; i <= end; i++)
		{
			auto info = ResolvePackedStat(i);
			if (!info.IsValid())
				break; // the rest are probably not valid, either

			if (info.is_bool_stat)
				STATS::SET_PACKED_STAT_BOOL_CODE(info.index, static_cast<bool>(value), -1);
		}
	}

	// Mirrors RenderStatEditor() - one row (or a static "not supported"
	// text) matching data's current type, edited entirely in `value`
	// (this method's own caller reads it back at Write-click time, never
	// this widget directly) - see this class's own header comment on the
	// no-64-bit-stepper tradeoff (same as SavedVariableGridRows.cpp's
	// identical one for Debug > Globals/Locals).
	void StatEditorGrid::AddStatValueEditRow(std::vector<std::unique_ptr<GridItem>>& items_draft, int16_t width, int16_t height, sStatData::Type type, StatValue& value)
	{
		switch (type)
		{
		case sStatData::Type::_BOOL:
			items_draft.push_back(std::make_unique<GridItemBoundToggle>(
			    width,
			    height,
			    "Value",
			    [&value] {
				    return value.as_bool;
			    },
			    [&value](bool newValue) {
				    value.as_bool = newValue;
			    }));
			break;
		case sStatData::Type::FLOAT:
			items_draft.push_back(std::make_unique<GridItemFloatStepper>(width,
			    height,
			    "Value",
			    value.as_float,
			    -std::numeric_limits<float>::max(),
			    std::numeric_limits<float>::max(),
			    0.1f,
			    [&value](float newValue) {
				    value.as_float = newValue;
			    }));
			break;
		case sStatData::Type::INT:
		case sStatData::Type::UINT32:
		case sStatData::Type::UINT16:
		case sStatData::Type::UINT8:
			items_draft.push_back(std::make_unique<GridItemIntStepper>(width,
			    height,
			    "Value",
			    value.as_int,
			    std::numeric_limits<int>::min(),
			    std::numeric_limits<int>::max(),
			    [&value](int newValue) {
				    value.as_int = newValue;
			    }));
			break;
		case sStatData::Type::INT64:
		case sStatData::Type::UINT64:
		{
			const int clamped = static_cast<int>(std::clamp<std::uint64_t>(value.as_u64, 0, static_cast<std::uint64_t>(std::numeric_limits<int>::max())));
			items_draft.push_back(std::make_unique<GridItemIntStepper>(width,
			    height,
			    "Value",
			    clamped,
			    0,
			    std::numeric_limits<int>::max(),
			    [&value](int newValue) {
				    value.as_u64 = static_cast<std::uint64_t>(newValue);
			    }));
			break;
		}
		case sStatData::Type::STRING:
		{
			std::string initial(value.as_string, strnlen(value.as_string, sizeof(value.as_string)));
			items_draft.push_back(std::make_unique<GridItemTextInput>(width,
			    height,
			    "Value",
			    initial,
			    [&value](const std::string& newValue) {
				    std::memset(value.as_string, 0, sizeof(value.as_string));
				    std::memcpy(value.as_string, newValue.data(), std::min(newValue.size(), sizeof(value.as_string) - 1));
			    }));
			break;
		}
		default:
			items_draft.push_back(std::make_unique<GridItemText>(width, height, "Data type not supported", Theme::kError));
			break;
		}
	}

	void StatEditorGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// Every one of the original's four Groups starts with the exact
		// same "natives not cached yet" early return - checked once here
		// instead, via watchCondition() so the whole page automatically
		// switches over to the real editor the moment handlers actually
		// do get cached, without the user needing to do anything (see
		// this class's own header comment).
		if (!watchCondition([] {
			    return NativeInvoker::AreHandlersCached();
		    }))
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Natives not cached yet", Theme::kError));
			return;
		}

		// Regular
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Regular", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemTextInput>(Theme::kContentWidth,
		    kItemH,
		    "Name",
		    m_StatNameRaw,
		    [this](const std::string& value) {
			    m_StatNameRaw = value;
			    m_CurStat = ResolveStat(value);
			    if (m_CurStat.IsValid())
				    ReadStat(m_CurValue, m_CurStat.data);
			    invalidate();
		    }));

		if (!m_CurStat.IsValid())
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "Stat not found", Theme::kError));
		}
		else
		{
			if (m_CurStat.normalized)
				items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, std::format("Normalized to: {}", m_CurStat.name), Theme::kText));

			AddStatValueEditRow(items_draft, Theme::kContentWidth, kItemH, m_CurStat.data->GetType(), m_CurValue);

			items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Refresh", [this] {
				ReadStat(m_CurValue, m_CurStat.data);
				invalidate();
			}));

			// "Write"/"Force Write" - see this class's own header comment
			// for why this replaces the original's single disabled-plus-
			// right-click-override button.
			if (!m_CurStat.data->IsControlledByNetshop())
			{
				items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Write", [this] {
					FiberPool::queueJob([this] {
						WriteStat(m_CurStat.name_hash, m_CurValue, m_CurStat.data);
					});
				}));
			}
			else
			{
				items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "This stat should not be edited by the client", Theme::kError));
				items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Force Write", [this] {
					FiberPool::queueJob([this] {
						WriteStat(m_CurStat.name_hash, m_CurValue, m_CurStat.data);
					});
				}));
			}
		}

		// Packed
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Packed", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
		    kItemH,
		    "Index",
		    m_CurPacked.index,
		    0,
		    std::numeric_limits<int>::max(),
		    [this](int value) {
			    m_CurPacked = ResolvePackedStat(value);
			    if (m_CurPacked.IsValid())
				    ReadPackedStat(m_CurPackedValue, m_CurPacked);
			    invalidate();
		    }));

		if (!m_CurPacked.IsValid())
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kItemH, "Index not valid", Theme::kError));
		}
		else
		{
			if (m_CurPacked.is_bool_stat)
			{
				items_draft.push_back(std::make_unique<GridItemBoundToggle>(
				    Theme::kContentWidth,
				    kItemH,
				    "Value",
				    [this] {
					    return m_CurPackedValue.as_bool;
				    },
				    [this](bool value) {
					    m_CurPackedValue.as_bool = value;
				    }));
			}
			else
			{
				items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
				    kItemH,
				    "Value",
				    m_CurPackedValue.as_int,
				    0,
				    255,
				    [this](int value) {
					    m_CurPackedValue.as_int = value;
				    }));
			}

			items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Refresh", [this] {
				ReadPackedStat(m_CurPackedValue, m_CurPacked);
				invalidate();
			}));
			items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Write", [this] {
				FiberPool::queueJob([this] {
					WritePackedStat(m_CurPackedValue, m_CurPacked);
				});
			}));
		}

		// Packed Range - Start/End/Value are plain scratch ints, same as
		// the original's own function-local statics with no live
		// resolution until Write is actually clicked.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Packed Range", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
		    kItemH,
		    "Start",
		    m_RangeStart,
		    0,
		    std::numeric_limits<int>::max(),
		    [this](int value) {
			    m_RangeStart = value;
		    }));
		items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
		    kItemH,
		    "End",
		    m_RangeEnd,
		    0,
		    std::numeric_limits<int>::max(),
		    [this](int value) {
			    m_RangeEnd = value;
		    }));
		items_draft.push_back(std::make_unique<GridItemIntStepper>(Theme::kContentWidth,
		    kItemH,
		    "Value",
		    m_RangeValue,
		    0,
		    255,
		    [this](int value) {
			    m_RangeValue = value;
		    }));
		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Write", [this] {
			const int start = m_RangeStart;
			const int end = m_RangeEnd;
			const int value = m_RangeValue;

			FiberPool::queueJob([start, end, value] {
				WritePackedStatRange(start, end, value);
			});
		}));

		// From Clipboard - clipboard text is read synchronously here
		// (same thread ImGui::GetClipboardText() ran on in the original),
		// then handed by value into the FiberPool job that parses and
		// writes it.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "From Clipboard", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Load from Clipboard", [] {
			auto clipText = Clipboard::GetText();

			FiberPool::queueJob([clipText] {
				for (auto line : clipText | std::ranges::views::split('\n'))
				{
					auto trimmedLine = TrimStringView(std::string_view{line.begin(), line.end()});
					auto components = trimmedLine | std::ranges::views::split('=') | std::ranges::to<std::vector<std::string>>();

					if (components.size() != 2)
					{
						LOGF(WARNING, "Load From Clipboard: line \"{}\" is malformed", trimmedLine);
						continue;
					}

					auto info = ResolveStat(TrimStringView(components[0]));
					if (!info.IsValid())
					{
						LOGF(WARNING, "Load From Clipboard: cannot find stat {}", components[0]);
						continue;
					}

					WriteStatWithStringValue(info.name_hash, TrimStringView(components[1]), info.data);
				}
			});
		}));
	}
}

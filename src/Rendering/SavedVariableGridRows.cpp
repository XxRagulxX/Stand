#include "Rendering/SavedVariableGridRows.hpp"

#include "Rendering/GridItemBoundToggle.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemFloatStepper.hpp"
#include "Rendering/GridItemIntStepper.hpp"
#include "Rendering/GridItemLocalList.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/GridItemTextInput.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/scrVector.hpp"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <format>
#include <limits>

namespace Stand::Rendering
{
	namespace
	{
		// Matches DrawSavedVariable()'s own Combo order exactly ("Int\0
		// Int64\0Bitset\0Bool\0Float\0Vector\0String\0") - index i is
		// VariableType(i).
		std::vector<std::string> TypeNames()
		{
			return {"Int", "Int64", "Bitset", "Bool", "Float", "Vector", "String"};
		}

		// The original's own 255-byte fixed buffer convention
		// (outfitName/localName/globalName/DrawSavedVariableEdit's
		// InputText all use 255) - STRING variables point at a raw
		// buffer of this size, not a std::string.
		constexpr size_t kStringBufferSize = 255;
	}

	void AddSavedVariableDefinitionRows(std::vector<std::unique_ptr<GridItem>>& items_draft, int16_t width, SavedVariableBase& var, std::function<void()> onShapeChanged)
	{
		const int initialBase = static_cast<int>(std::min<std::uint32_t>(var.base, static_cast<std::uint32_t>(std::numeric_limits<int>::max())));
		items_draft.push_back(std::make_unique<GridItemIntStepper>(width,
		    Theme::kContentItemHeight,
		    "Index",
		    initialBase,
		    0,
		    std::numeric_limits<int>::max(),
		    [&var](int newValue) {
			    var.base = static_cast<std::uint32_t>(std::max(0, newValue));
		    }));

		items_draft.push_back(std::make_unique<GridItemLocalList>(width,
		    Theme::kContentItemHeight,
		    "Type",
		    TypeNames(),
		    static_cast<int>(var.type),
		    [&var, onShapeChanged](int index) {
			    var.type = static_cast<VariableType>(index);
			    if (onShapeChanged)
				    onShapeChanged();
		    }));

		for (size_t i = 0; i < var.appendages.size(); i++)
		{
			auto& appendage = var.appendages[i];
			switch (appendage.type)
			{
			case VariableAppendage::Type::OFFSET:
				items_draft.push_back(std::make_unique<GridItemIntStepper>(width,
				    Theme::kContentItemHeight,
				    std::format("Offset {} At", i),
				    appendage.offset,
				    std::numeric_limits<int>::min(),
				    std::numeric_limits<int>::max(),
				    [&appendage](int newValue) {
					    appendage.offset = newValue;
				    }));
				items_draft.push_back(std::make_unique<GridItemIntStepper>(width,
				    Theme::kContentItemHeight,
				    std::format("Offset {} Size", i),
				    appendage.size,
				    0,
				    std::numeric_limits<int>::max(),
				    [&appendage](int newValue) {
					    appendage.size = newValue;
				    }));
				break;
			case VariableAppendage::Type::PLAYER_ID:
				items_draft.push_back(std::make_unique<GridItemIntStepper>(width,
				    Theme::kContentItemHeight,
				    std::format("Player Id {} Size", i),
				    appendage.size,
				    0,
				    std::numeric_limits<int>::max(),
				    [&appendage](int newValue) {
					    appendage.size = newValue;
				    }));
				break;
			}
		}

		items_draft.push_back(std::make_unique<GridItemButton>(width, Theme::kContentItemHeight, "Add Offset", [&var, onShapeChanged] {
			var.appendages.push_back({VariableAppendage::Type::OFFSET, 0, 0});
			if (onShapeChanged)
				onShapeChanged();
		}));
		items_draft.push_back(std::make_unique<GridItemButton>(width, Theme::kContentItemHeight, "Add Read Player Id", [&var, onShapeChanged] {
			var.appendages.push_back({VariableAppendage::Type::PLAYER_ID, 0, 0});
			if (onShapeChanged)
				onShapeChanged();
		}));

		// Remove Offset only shown with at least one appendage to remove -
		// same early-return the original's own ImGui::Button("Remove
		// Offset") has, just decided once here at populate() time instead
		// of every frame, since anything that changes appendages.size()
		// already goes through onShapeChanged() (a full repopulate) - see
		// this function's own header comment.
		if (!var.appendages.empty())
		{
			items_draft.push_back(std::make_unique<GridItemButton>(width, Theme::kContentItemHeight, "Remove Offset", [&var, onShapeChanged] {
				var.appendages.pop_back();
				if (onShapeChanged)
					onShapeChanged();
			}));
		}

		items_draft.push_back(std::make_unique<GridItemButton>(width, Theme::kContentItemHeight, "Clear", [&var, onShapeChanged] {
			var.base = 0;
			var.appendages.clear();
			if (onShapeChanged)
				onShapeChanged();
		}));
	}

	void AddSavedVariableValueEditRows(std::vector<std::unique_ptr<GridItem>>& items_draft, int16_t width, int16_t height, const SavedVariableBase& var, std::function<void*()> readValue)
	{
		void* initial = readValue ? readValue() : nullptr;
		if (!initial)
		{
			items_draft.push_back(std::make_unique<GridItemText>(width, height, "Invalid", Theme::kError));
			return;
		}

		switch (var.type)
		{
		case VariableType::INT:
		{
			items_draft.push_back(std::make_unique<GridItemIntStepper>(width,
			    height,
			    "Value",
			    *static_cast<int*>(initial),
			    std::numeric_limits<int>::min(),
			    std::numeric_limits<int>::max(),
			    [readValue](int newValue) {
				    if (auto* v = readValue ? readValue() : nullptr)
					    *static_cast<int*>(v) = newValue;
			    }));
			break;
		}
		case VariableType::INT64:
		{
			// No 64-bit stepper widget exists yet (GridItemIntStepper is
			// int32) - narrows the same way every other stepper's
			// generous-bound tradeoff does elsewhere in this project
			// (see e.g. OutfitEditorGrid's own header comment); a value
			// outside int32 range can't be displayed or set exactly.
			const auto initial64 = *static_cast<std::int64_t*>(initial);
			const int clamped = static_cast<int>(std::clamp<std::int64_t>(initial64, std::numeric_limits<int>::min(), std::numeric_limits<int>::max()));
			items_draft.push_back(std::make_unique<GridItemIntStepper>(width,
			    height,
			    "Value",
			    clamped,
			    std::numeric_limits<int>::min(),
			    std::numeric_limits<int>::max(),
			    [readValue](int newValue) {
				    if (auto* v = readValue ? readValue() : nullptr)
					    *static_cast<std::int64_t*>(v) = static_cast<std::int64_t>(newValue);
			    }));
			break;
		}
		case VariableType::BITSET:
		{
			// Reuses the plain integer stepper (see this file's own
			// header comment) - no per-bit toggle widget yet, but the
			// underlying storage is still a plain int either way, so the
			// value actually applied is exact, just edited as a raw
			// integer instead of individual bit checkboxes.
			items_draft.push_back(std::make_unique<GridItemIntStepper>(width,
			    height,
			    "Value",
			    *static_cast<int*>(initial),
			    std::numeric_limits<int>::min(),
			    std::numeric_limits<int>::max(),
			    [readValue](int newValue) {
				    if (auto* v = readValue ? readValue() : nullptr)
					    *static_cast<int*>(v) = newValue;
			    }));
			break;
		}
		case VariableType::BOOL:
		{
			items_draft.push_back(std::make_unique<GridItemBoundToggle>(
			    width,
			    height,
			    "Value",
			    [readValue] {
				    auto* v = readValue ? readValue() : nullptr;
				    return v && *static_cast<bool*>(v);
			    },
			    [readValue](bool newValue) {
				    if (auto* v = readValue ? readValue() : nullptr)
					    *static_cast<bool*>(v) = newValue;
			    }));
			break;
		}
		case VariableType::FLOAT:
		{
			items_draft.push_back(std::make_unique<GridItemFloatStepper>(width,
			    height,
			    "Value",
			    *static_cast<float*>(initial),
			    -std::numeric_limits<float>::max(),
			    std::numeric_limits<float>::max(),
			    0.1f,
			    [readValue](float newValue) {
				    if (auto* v = readValue ? readValue() : nullptr)
					    *static_cast<float*>(v) = newValue;
			    }));
			break;
		}
		case VariableType::VECTOR:
		{
			auto* initialVec = static_cast<rage::scrVector*>(initial);

			items_draft.push_back(std::make_unique<GridItemFloatStepper>(width,
			    height,
			    "X",
			    initialVec->x,
			    -std::numeric_limits<float>::max(),
			    std::numeric_limits<float>::max(),
			    0.5f,
			    [readValue](float newValue) {
				    if (auto* v = readValue ? readValue() : nullptr)
					    static_cast<rage::scrVector*>(v)->x = newValue;
			    }));
			items_draft.push_back(std::make_unique<GridItemFloatStepper>(width,
			    height,
			    "Y",
			    initialVec->y,
			    -std::numeric_limits<float>::max(),
			    std::numeric_limits<float>::max(),
			    0.5f,
			    [readValue](float newValue) {
				    if (auto* v = readValue ? readValue() : nullptr)
					    static_cast<rage::scrVector*>(v)->y = newValue;
			    }));
			items_draft.push_back(std::make_unique<GridItemFloatStepper>(width,
			    height,
			    "Z",
			    initialVec->z,
			    -std::numeric_limits<float>::max(),
			    std::numeric_limits<float>::max(),
			    0.5f,
			    [readValue](float newValue) {
				    if (auto* v = readValue ? readValue() : nullptr)
					    static_cast<rage::scrVector*>(v)->z = newValue;
			    }));
			break;
		}
		case VariableType::STRING:
		{
			auto* buffer = static_cast<char*>(initial);
			std::string initialStr(buffer, strnlen(buffer, kStringBufferSize));

			items_draft.push_back(std::make_unique<GridItemTextInput>(width,
			    height,
			    "Value",
			    initialStr,
			    [readValue](const std::string& newValue) {
				    auto* v = readValue ? readValue() : nullptr;
				    if (!v)
					    return;

				    auto* buf = static_cast<char*>(v);
				    std::memset(buf, 0, kStringBufferSize);
				    std::memcpy(buf, newValue.data(), std::min(newValue.size(), kStringBufferSize - 1));
			    }));
			break;
		}
		}
	}

	std::string FormatSavedVariablePreview(const SavedVariableBase& var, void* value)
	{
		if (!value)
			return "Invalid";

		switch (var.type)
		{
		case VariableType::INT:
			return std::to_string(*static_cast<int*>(value));
		case VariableType::INT64:
			return std::to_string(*static_cast<std::int64_t*>(value));
		case VariableType::BITSET:
			return std::format("0x{:X}", *static_cast<unsigned int*>(value));
		case VariableType::BOOL:
			return *static_cast<bool*>(value) ? "True" : "False";
		case VariableType::FLOAT:
			return std::format("{:.2f}", *static_cast<float*>(value));
		case VariableType::VECTOR:
		{
			auto* v = static_cast<rage::scrVector*>(value);
			return std::format("X: {:.2f} Y: {:.2f} Z: {:.2f}", v->x, v->y, v->z);
		}
		case VariableType::STRING:
			return std::string(static_cast<char*>(value), strnlen(static_cast<char*>(value), kStringBufferSize));
		}

		return "?";
	}
}

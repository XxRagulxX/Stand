#pragma once
#include "Config/SavedVariables.hpp"
#include "Rendering/GridItem.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace Stand::Rendering
{
	// Ports DrawVariable.cpp's DrawSavedVariable()/DrawSavedVariableEdit()/
	// DrawSavedVariablePreview() to Grid rows - shared between GlobalsGrid
	// and LocalsGrid, same as the original DrawVariable.cpp is shared
	// between Globals.cpp/Locals.cpp (the only difference between a global
	// and a local is how its live pointer is computed - SavedGlobal::Read()
	// vs SavedLocal::Read(thread) - which stays entirely on the caller's
	// side here, via readValue).

	// DrawSavedVariable(): Index (var.base) stepper, Type cycler (the
	// seven VariableType values), each appendage's own row(s) (OFFSET:
	// At/Size steppers; PLAYER_ID: Size stepper), and Add Offset/Add Read
	// Player Id/Remove Offset (only when var.appendages isn't empty)/Clear
	// buttons.
	//
	// onShapeChanged fires whenever Type or the appendage list itself
	// changes - anything that changes which/how many widgets this
	// function (or AddSavedVariableValueEditRows(), whose value-edit
	// row(s) depend on var.type) would build, since populate() only runs
	// once (see Grid.hpp's own class comment) and can't add/remove/
	// retype a row after the fact. The owning Grid supplies this as e.g.
	// [this]{ invalidate(); } - only it can reach its own protected
	// Grid::invalidate(), the same reason Grid::watchCondition() is
	// public instead (see its own doc comment) for a function that only
	// ever has a Grid&, not a derived-class this.
	void AddSavedVariableDefinitionRows(std::vector<std::unique_ptr<GridItem>>& items_draft, int16_t width, SavedVariableBase& var, std::function<void()> onShapeChanged);

	// DrawSavedVariableEdit(): the single live-value editor matching
	// var.type (three rows for VECTOR's X/Y/Z, one otherwise). readValue
	// is called fresh on every read and every write - never cached across
	// calls, since the owning Grid may return a different address (or
	// null) from one call to the next (a local's stack address is only
	// meaningful against whatever thread is current right now).
	//
	// BITSET reuses the same plain integer stepper as INT (there's no
	// per-bit toggle widget in this system yet - see this function's own
	// implementation comment) - still fully correct (the underlying
	// storage is a plain int either way), just without the original's
	// individual bit-toggle convenience.
	void AddSavedVariableValueEditRows(std::vector<std::unique_ptr<GridItem>>& items_draft, int16_t width, int16_t height, const SavedVariableBase& var, std::function<void*()> readValue);

	// DrawSavedVariablePreview(): the same formatting, as a plain string
	// rather than a disabled ImGui::Selectable - used to embed a live
	// value preview directly into a saved-variable list row's own label
	// (see GlobalsGrid/LocalsGrid's own "Saved" section) instead of a
	// second list box the way the original shows side-by-side, since
	// GridItemSelectList's rows() is already re-read fresh on every call
	// (see its own class comment) - folding the preview into the same
	// row keeps it just as live with one widget instead of two.
	std::string FormatSavedVariablePreview(const SavedVariableBase& var, void* value);
}

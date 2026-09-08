#include "Scripting/LuaLibrary.hpp"
#include "Scripting/LuaMainScript.hpp"
#include "Scripting/LuaMainUtils.hpp"

namespace Stand::Lua
{
	// Lua-facing "ImGui" binding - used to wrap the real Dear ImGui C API
	// 1:1 (~260 draw/layout/widget/introspection functions) for scripts
	// that called cat:imgui()/group:imgui() or drew directly onto ImGui
	// windows of their own. ImGui itself has been removed from this
	// project entirely (see Menu/UIManager.hpp's own class comment) and
	// Lua's own port onto the DirectXTK/Grid renderer is being deferred to
	// its own future pass rather than rebuilt twice here - so every one of
	// these ~260 functions is kept registered under its original name
	// (a script calling imgui.Button(...) still gets a real function call,
	// not a "attempt to call a nil value" error) but now does nothing,
	// via the single shared NoOp() below, rather than drawing anything.
	//
	// The enum constant tables at the bottom of Register() (ImGuiCol,
	// ImGuiWindowFlags, ImGuiCond, ...) are untouched - they were always
	// plain name->int literal tables with no dependency on the real ImGui
	// library, so scripts referencing e.g. ImGui.Col.Text still get the
	// same integer they always did.
	class ImGuiBind : LuaLibrary
	{
		using LuaLibrary::LuaLibrary;

		static int NoOp(lua_State*)
		{
			return 0;
		}

		virtual void Register(lua_State* state) override
		{
			lua_newtable(state);

			// Internal / drawing
			SetFunction(state, NoOp, "AddCircle");
			SetFunction(state, NoOp, "AddCircleFilled");
			SetFunction(state, NoOp, "AddLine");
			SetFunction(state, NoOp, "AddRect");
			SetFunction(state, NoOp, "AddRectFilled");
			SetFunction(state, NoOp, "AddRectFilledMultiColor");
			SetFunction(state, NoOp, "AddText");
			SetFunction(state, NoOp, "AddTriangle");
			SetFunction(state, NoOp, "AddTriangleFilled");

			// Tables
			SetFunction(state, NoOp, "BeginTable");
			SetFunction(state, NoOp, "EndTable");
			SetFunction(state, NoOp, "TableNextColumn");
			SetFunction(state, NoOp, "TableNextRow");
			SetFunction(state, NoOp, "TableSetColumnIndex");
			SetFunction(state, NoOp, "TableSetupColumn");
			SetFunction(state, NoOp, "TableHeadersRow");

			// Color conversions
			SetFunction(state, NoOp, "ColorConvertFloat4ToU32");
			SetFunction(state, NoOp, "ColorConvertRGBAToU32");
			SetFunction(state, NoOp, "ColorConvertU32ToFloat4");
			SetFunction(state, NoOp, "ColorConvertRGBtoHSV");
			SetFunction(state, NoOp, "ColorConvertHSVtoRGB");

			// Display
			SetFunction(state, NoOp, "GetDisplaySize");
			SetFunction(state, NoOp, "GetFrameRate");

			// Windows
			SetFunction(state, NoOp, "Begin");
			SetFunction(state, NoOp, "End");
			SetFunction(state, NoOp, "BeginChild");
			SetFunction(state, NoOp, "EndChild");

			// Window utilities
			SetFunction(state, NoOp, "IsWindowAppearing");
			SetFunction(state, NoOp, "IsWindowCollapsed");
			SetFunction(state, NoOp, "IsWindowFocused");
			SetFunction(state, NoOp, "IsWindowHovered");
			SetFunction(state, NoOp, "GetWindowPos");
			SetFunction(state, NoOp, "GetWindowSize");
			SetFunction(state, NoOp, "GetWindowWidth");
			SetFunction(state, NoOp, "GetWindowHeight");
			SetFunction(state, NoOp, "SetNextWindowPos");
			SetFunction(state, NoOp, "SetNextWindowSize");
			SetFunction(state, NoOp, "SetNextWindowSizeConstraints");
			SetFunction(state, NoOp, "SetNextWindowContentSize");
			SetFunction(state, NoOp, "SetNextWindowCollapsed");
			SetFunction(state, NoOp, "SetNextWindowFocus");
			SetFunction(state, NoOp, "SetNextWindowBgAlpha");
			SetFunction(state, NoOp, "SetWindowPos");
			SetFunction(state, NoOp, "SetWindowSize");
			SetFunction(state, NoOp, "SetWindowCollapsed");
			SetFunction(state, NoOp, "SetWindowFocus");
			SetFunction(state, NoOp, "SetWindowFontScale");

			// Content region
			SetFunction(state, NoOp, "GetContentRegionMax");
			SetFunction(state, NoOp, "GetContentRegionAvail");
			SetFunction(state, NoOp, "GetWindowContentRegionMin");
			SetFunction(state, NoOp, "GetWindowContentRegionMax");

			// Scrolling
			SetFunction(state, NoOp, "GetScrollX");
			SetFunction(state, NoOp, "GetScrollY");
			SetFunction(state, NoOp, "GetScrollMaxX");
			SetFunction(state, NoOp, "GetScrollMaxY");
			SetFunction(state, NoOp, "SetScrollX");
			SetFunction(state, NoOp, "SetScrollY");
			SetFunction(state, NoOp, "SetScrollHereX");
			SetFunction(state, NoOp, "SetScrollHereY");
			SetFunction(state, NoOp, "SetScrollFromPosX");
			SetFunction(state, NoOp, "SetScrollFromPosY");

			// Parameter stacks (shared)
			SetFunction(state, NoOp, "PushStyleColor");
			SetFunction(state, NoOp, "PopStyleColor");
			SetFunction(state, NoOp, "PushStyleVar");
			SetFunction(state, NoOp, "PopStyleVar");
			SetFunction(state, NoOp, "GetStyleColorVec4");
			SetFunction(state, NoOp, "GetFontSize");
			SetFunction(state, NoOp, "GetFontTexUvWhitePixel");

			// Parameter stacks (current window)
			SetFunction(state, NoOp, "PushItemWidth");
			SetFunction(state, NoOp, "PopItemWidth");
			SetFunction(state, NoOp, "SetNextItemWidth");
			SetFunction(state, NoOp, "CalcItemWidth");
			SetFunction(state, NoOp, "PushTextWrapPos");
			SetFunction(state, NoOp, "PopTextWrapPos");
			SetFunction(state, NoOp, "PushButtonRepeat");
			SetFunction(state, NoOp, "PopButtonRepeat");

			// Cursor / layout
			SetFunction(state, NoOp, "Separator");
			SetFunction(state, NoOp, "SeparatorText");
			SetFunction(state, NoOp, "BeginDisabled");
			SetFunction(state, NoOp, "EndDisabled");
			SetFunction(state, NoOp, "GetStyle");
			SetFunction(state, NoOp, "SameLine");
			SetFunction(state, NoOp, "NewLine");
			SetFunction(state, NoOp, "Spacing");
			SetFunction(state, NoOp, "Dummy");
			SetFunction(state, NoOp, "Indent");
			SetFunction(state, NoOp, "Unindent");
			SetFunction(state, NoOp, "BeginGroup");
			SetFunction(state, NoOp, "EndGroup");
			SetFunction(state, NoOp, "GetCursorPos");
			SetFunction(state, NoOp, "GetCursorPosX");
			SetFunction(state, NoOp, "GetCursorPosY");
			SetFunction(state, NoOp, "SetCursorPos");
			SetFunction(state, NoOp, "SetCursorPosX");
			SetFunction(state, NoOp, "SetCursorPosY");
			SetFunction(state, NoOp, "GetCursorStartPos");
			SetFunction(state, NoOp, "GetCursorScreenPos");
			SetFunction(state, NoOp, "SetCursorScreenPos");
			SetFunction(state, NoOp, "AlignTextToFramePadding");
			SetFunction(state, NoOp, "GetTextLineHeight");
			SetFunction(state, NoOp, "GetTextLineHeightWithSpacing");
			SetFunction(state, NoOp, "GetFrameHeight");
			SetFunction(state, NoOp, "GetFrameHeightWithSpacing");

			// ID stack
			SetFunction(state, NoOp, "PushID");
			SetFunction(state, NoOp, "PopID");
			SetFunction(state, NoOp, "GetID");

			// Text widgets
			SetFunction(state, NoOp, "TextUnformatted");
			SetFunction(state, NoOp, "Text");
			SetFunction(state, NoOp, "TextColored");
			SetFunction(state, NoOp, "TextDisabled");
			SetFunction(state, NoOp, "TextWrapped");
			SetFunction(state, NoOp, "LabelText");
			SetFunction(state, NoOp, "BulletText");

			// Main widgets
			SetFunction(state, NoOp, "Button");
			SetFunction(state, NoOp, "SmallButton");
			SetFunction(state, NoOp, "InvisibleButton");
			SetFunction(state, NoOp, "ArrowButton");
			SetFunction(state, NoOp, "Checkbox");
			SetFunction(state, NoOp, "RadioButton");
			SetFunction(state, NoOp, "ProgressBar");
			SetFunction(state, NoOp, "Bullet");

			// Combo
			SetFunction(state, NoOp, "BeginCombo");
			SetFunction(state, NoOp, "EndCombo");
			SetFunction(state, NoOp, "Combo");

			// Drag
			SetFunction(state, NoOp, "DragFloat");
			SetFunction(state, NoOp, "DragFloat2");
			SetFunction(state, NoOp, "DragFloat3");
			SetFunction(state, NoOp, "DragFloat4");
			SetFunction(state, NoOp, "DragInt");
			SetFunction(state, NoOp, "DragInt2");
			SetFunction(state, NoOp, "DragInt3");
			SetFunction(state, NoOp, "DragInt4");

			// Sliders
			SetFunction(state, NoOp, "SliderFloat");
			SetFunction(state, NoOp, "SliderFloat2");
			SetFunction(state, NoOp, "SliderFloat3");
			SetFunction(state, NoOp, "SliderFloat4");
			SetFunction(state, NoOp, "SliderAngle");
			SetFunction(state, NoOp, "SliderInt");
			SetFunction(state, NoOp, "SliderInt2");
			SetFunction(state, NoOp, "SliderInt3");
			SetFunction(state, NoOp, "SliderInt4");
			SetFunction(state, NoOp, "VSliderFloat");
			SetFunction(state, NoOp, "VSliderInt");

			// Input with keyboard
			SetFunction(state, NoOp, "InputText");
			SetFunction(state, NoOp, "InputTextMultiline");
			SetFunction(state, NoOp, "InputTextWithHint");
			SetFunction(state, NoOp, "InputFloat");
			SetFunction(state, NoOp, "InputFloat2");
			SetFunction(state, NoOp, "InputFloat3");
			SetFunction(state, NoOp, "InputFloat4");
			SetFunction(state, NoOp, "InputInt");
			SetFunction(state, NoOp, "InputInt2");
			SetFunction(state, NoOp, "InputInt3");
			SetFunction(state, NoOp, "InputInt4");
			SetFunction(state, NoOp, "InputDouble");

			// Color editor / picker
			SetFunction(state, NoOp, "ColorEdit3");
			SetFunction(state, NoOp, "ColorEdit4");
			SetFunction(state, NoOp, "ColorPicker3");
			SetFunction(state, NoOp, "ColorPicker4");
			SetFunction(state, NoOp, "ColorButton");
			SetFunction(state, NoOp, "SetColorEditOptions");

			// Trees
			SetFunction(state, NoOp, "TreeNode");
			SetFunction(state, NoOp, "TreeNodeEx");
			SetFunction(state, NoOp, "TreePush");
			SetFunction(state, NoOp, "TreePop");
			SetFunction(state, NoOp, "GetTreeNodeToLabelSpacing");
			SetFunction(state, NoOp, "CollapsingHeader");
			SetFunction(state, NoOp, "SetNextItemOpen");

			// Selectables
			SetFunction(state, NoOp, "Selectable");

			// List boxes
			SetFunction(state, NoOp, "ListBox");
			SetFunction(state, NoOp, "BeginListBox");
			SetFunction(state, NoOp, "EndListBox");

			// Value()
			SetFunction(state, NoOp, "Value");

			// Menus
			SetFunction(state, NoOp, "BeginMenuBar");
			SetFunction(state, NoOp, "EndMenuBar");
			SetFunction(state, NoOp, "BeginMainMenuBar");
			SetFunction(state, NoOp, "EndMainMenuBar");
			SetFunction(state, NoOp, "BeginMenu");
			SetFunction(state, NoOp, "EndMenu");
			SetFunction(state, NoOp, "MenuItem");

			// Tooltips
			SetFunction(state, NoOp, "BeginTooltip");
			SetFunction(state, NoOp, "EndTooltip");
			SetFunction(state, NoOp, "SetTooltip");

			// Popups / modals
			SetFunction(state, NoOp, "BeginPopup");
			SetFunction(state, NoOp, "BeginPopupModal");
			SetFunction(state, NoOp, "EndPopup");
			SetFunction(state, NoOp, "OpenPopup");
			SetFunction(state, NoOp, "OpenPopupContextItem");
			SetFunction(state, NoOp, "CloseCurrentPopup");
			SetFunction(state, NoOp, "BeginPopupContextItem");
			SetFunction(state, NoOp, "BeginPopupContextWindow");
			SetFunction(state, NoOp, "BeginPopupContextVoid");
			SetFunction(state, NoOp, "IsPopupOpen");

			// Columns
			SetFunction(state, NoOp, "Columns");
			SetFunction(state, NoOp, "NextColumn");
			SetFunction(state, NoOp, "GetColumnIndex");
			SetFunction(state, NoOp, "GetColumnWidth");
			SetFunction(state, NoOp, "SetColumnWidth");
			SetFunction(state, NoOp, "GetColumnOffset");
			SetFunction(state, NoOp, "SetColumnOffset");
			SetFunction(state, NoOp, "GetColumnsCount");

			// Tab bars
			SetFunction(state, NoOp, "BeginTabBar");
			SetFunction(state, NoOp, "EndTabBar");
			SetFunction(state, NoOp, "BeginTabItem");
			SetFunction(state, NoOp, "EndTabItem");
			SetFunction(state, NoOp, "SetTabItemClosed");

			// Logging
			SetFunction(state, NoOp, "LogToTTY");
			SetFunction(state, NoOp, "LogToFile");
			SetFunction(state, NoOp, "LogToClipboard");
			SetFunction(state, NoOp, "LogFinish");
			SetFunction(state, NoOp, "LogButtons");
			SetFunction(state, NoOp, "LogText");

			// Clipping
			SetFunction(state, NoOp, "PushClipRect");
			SetFunction(state, NoOp, "PopClipRect");

			// Focus / activation
			SetFunction(state, NoOp, "SetItemDefaultFocus");
			SetFunction(state, NoOp, "SetKeyboardFocusHere");

			// Item utilities
			SetFunction(state, NoOp, "IsItemHovered");
			SetFunction(state, NoOp, "IsItemActive");
			SetFunction(state, NoOp, "IsItemFocused");
			SetFunction(state, NoOp, "IsItemClicked");
			SetFunction(state, NoOp, "IsItemVisible");
			SetFunction(state, NoOp, "IsItemEdited");
			SetFunction(state, NoOp, "IsItemActivated");
			SetFunction(state, NoOp, "IsItemDeactivated");
			SetFunction(state, NoOp, "IsItemDeactivatedAfterEdit");
			SetFunction(state, NoOp, "IsItemToggledOpen");
			SetFunction(state, NoOp, "IsAnyItemHovered");
			SetFunction(state, NoOp, "IsAnyItemActive");
			SetFunction(state, NoOp, "IsAnyItemFocused");
			SetFunction(state, NoOp, "GetItemRectMin");
			SetFunction(state, NoOp, "GetItemRectMax");
			SetFunction(state, NoOp, "GetItemRectSize");

			// Miscellaneous utilities
			SetFunction(state, NoOp, "IsRectVisible");
			SetFunction(state, NoOp, "GetTime");
			SetFunction(state, NoOp, "GetFrameCount");
			SetFunction(state, NoOp, "GetStyleColorName");
			SetFunction(state, NoOp, "BeginChildFrame");
			SetFunction(state, NoOp, "EndChildFrame");

			// Text utilities
			SetFunction(state, NoOp, "CalcTextSize");

			// Keyboard inputs
			SetFunction(state, NoOp, "IsKeyDown");
			SetFunction(state, NoOp, "IsKeyPressed");
			SetFunction(state, NoOp, "IsKeyReleased");
			SetFunction(state, NoOp, "GetKeyPressedAmount");
			SetFunction(state, NoOp, "SetNextFrameWantCaptureKeyboard");

			// Mouse inputs
			SetFunction(state, NoOp, "IsMouseDown");
			SetFunction(state, NoOp, "IsMouseClicked");
			SetFunction(state, NoOp, "IsMouseReleased");
			SetFunction(state, NoOp, "IsMouseDoubleClicked");
			SetFunction(state, NoOp, "IsMouseHoveringRect");
			SetFunction(state, NoOp, "IsAnyMouseDown");
			SetFunction(state, NoOp, "GetMousePos");
			SetFunction(state, NoOp, "GetMousePosOnOpeningCurrentPopup");
			SetFunction(state, NoOp, "IsMouseDragging");
			SetFunction(state, NoOp, "GetMouseDragDelta");
			SetFunction(state, NoOp, "ResetMouseDragDelta");
			SetFunction(state, NoOp, "GetMouseCursor");
			SetFunction(state, NoOp, "SetMouseCursor");
			SetFunction(state, NoOp, "SetNextFrameWantCaptureMouse");

			// Clipboard
			SetFunction(state, NoOp, "GetClipboardText");
			SetFunction(state, NoOp, "SetClipboardText");

			lua_setglobal(state, "ImGui");

			static const EnumEntry windowFlags[] = {
				{"None", 0}, {"NoTitleBar", 1}, {"NoResize", 2}, {"NoMove", 4},
				{"NoScrollbar", 8}, {"NoScrollWithMouse", 16}, {"NoCollapse", 32},
				{"AlwaysAutoResize", 64}, {"NoBackground", 128}, {"NoSavedSettings", 256},
				{"NoMouseInputs", 512}, {"MenuBar", 1024}, {"HorizontalScrollbar", 2048},
				{"NoFocusOnAppearing", 4096}, {"NoBringToFrontOnFocus", 8192},
				{"AlwaysVerticalScrollbar", 16384}, {"AlwaysHorizontalScrollbar", 32768},
				{"NoNavInputs", 65536}, {"NoNavFocus", 131072}, {"UnsavedDocument", 262144},
				{"NoNav", 196608}, {"NoDecoration", 43}, {"NoInputs", 197120},
				{"ChildWindow", 16777216}, {"Tooltip", 33554432}, {"Popup", 67108864},
				{"Modal", 134217728}, {"ChildMenu", 268435456}, {"NavFlattened", 536870912}
			};
			RegisterEnum(state, "ImGuiWindowFlags", windowFlags, std::size(windowFlags));

			static const EnumEntry childFlags[] = {
				{"None", 0}, {"Borders", 1}, {"Border", 1},
				{"AlwaysUseWindowPadding", 2}, {"ResizeX", 4},
				{"ResizeY", 8}, {"AutoResizeX", 16},
				{"AutoResizeY", 32}, {"AlwaysAutoResize", 64},
				{"FrameStyle", 128}, {"NavFlattened", 256}
			};
			RegisterEnum(state, "ImGuiChildFlags", childFlags, std::size(childFlags));

			static const EnumEntry mouseCursor[] = {
				{"None", -1}, {"Arrow", 0}, {"TextInput", 1}, {"ResizeAll", 2},
				{"ResizeNS", 3}, {"ResizeEW", 4}, {"ResizeNESW", 5}, {"ResizeNWSE", 6},
				{"Hand", 7}, {"Wait", 8}, {"Progress", 9}, {"NotAllowed", 10}, {"COUNT", 11}
			};
			RegisterEnum(state, "ImGuiMouseCursor", mouseCursor, std::size(mouseCursor));

			static const EnumEntry mouseButton[] = {
				{"ImGuiMouseButton_COUNT", 5}, {"ImGuiMouseButton_Left", 0},
				{"ImGuiMouseButton_Middle", 2}, {"ImGuiMouseButton_Right", 1}
			};
			RegisterEnum(state, "ImGuiMouseButton", mouseButton, std::size(mouseButton));

			static const EnumEntry imKey[] = {
				{"A", 546}, {"Backspace", 523}, {"C", 548}, {"COUNT", 645},
				{"Delete", 522}, {"DownArrow", 516}, {"End", 520}, {"Enter", 525},
				{"Escape", 526}, {"Home", 519}, {"Insert", 521}, {"KeyPadEnter", 615},
				{"LeftArrow", 513}, {"PageDown", 518}, {"PageUp", 517}, {"RightArrow", 514},
				{"Space", 524}, {"Tab", 512}, {"UpArrow", 515}, {"V", 567},
				{"X", 569}, {"Y", 570}, {"Z", 571}
			};
			RegisterEnum(state, "ImGuiKey", imKey, std::size(imKey));

			static const EnumEntry hoveredFlags[] = {
				{"None", 0}, {"ChildWindows", 1}, {"RootWindow", 2}, {"AnyWindow", 4},
				{"NoPopupHierarchy", 8}, {"AllowWhenBlockedByPopup", 32},
				{"AllowWhenBlockedByActiveItem", 128}, {"AllowWhenOverlappedByItem", 256},
				{"AllowWhenOverlappedByWindow", 512}, {"AllowWhenDisabled", 1024},
				{"NoNavOverride", 2048}, {"AllowWhenOverlapped", 768}, {"RectOnly", 928},
				{"RootAndChildWindows", 3}, {"ForTooltip", 4096}, {"Stationary", 8192},
				{"DelayNone", 16384}, {"DelayShort", 32768}, {"DelayNormal", 65536},
				{"NoSharedDelay", 131072}
			};
			RegisterEnum(state, "ImGuiHoveredFlags", hoveredFlags, std::size(hoveredFlags));

			static const EnumEntry focusedFlags[] = {
				{"AnyWindow", 4}, {"ChildWindows", 1}, {"None", 0},
				{"RootAndChildWindows", 3}, {"RootWindow", 2}
			};
			RegisterEnum(state, "ImGuiFocusedFlags", focusedFlags, std::size(focusedFlags));

			static const EnumEntry cond[] = {
				{"Always", 1}, {"Appearing", 8}, {"FirstUseEver", 4},
				{"None", 0}, {"Once", 2}
			};
			RegisterEnum(state, "ImGuiCond", cond, std::size(cond));

			static const EnumEntry col[] = {
				{"Text", 0}, {"TextDisabled", 1}, {"WindowBg", 2}, {"ChildBg", 3},
				{"PopupBg", 4}, {"Border", 5}, {"BorderShadow", 6}, {"FrameBg", 7},
				{"FrameBgHovered", 8}, {"FrameBgActive", 9}, {"TitleBg", 10},
				{"TitleBgActive", 11}, {"TitleBgCollapsed", 12}, {"MenuBarBg", 13},
				{"ScrollbarBg", 14}, {"ScrollbarGrab", 15}, {"ScrollbarGrabHovered", 16},
				{"ScrollbarGrabActive", 17}, {"CheckMark", 18}, {"SliderGrab", 19},
				{"SliderGrabActive", 20}, {"Button", 21}, {"ButtonHovered", 22},
				{"ButtonActive", 23}, {"Header", 24}, {"HeaderHovered", 25},
				{"HeaderActive", 26}, {"Separator", 27}, {"SeparatorHovered", 28},
				{"SeparatorActive", 29}, {"ResizeGrip", 30}, {"ResizeGripHovered", 31},
				{"ResizeGripActive", 32}, {"InputTextCursor", 33}, {"TabHovered", 34},
				{"Tab", 35}, {"TabSelected", 36}, {"TabSelectedOverline", 37},
				{"TabDimmed", 38}, {"TabDimmedSelected", 39}, {"TabDimmedSelectedOverline", 40},
				{"PlotLines", 41}, {"PlotLinesHovered", 42}, {"PlotHistogram", 43},
				{"PlotHistogramHovered", 44}, {"TableHeaderBg", 45}, {"TableBorderStrong", 46},
				{"TableBorderLight", 47}, {"TableRowBg", 48}, {"TableRowBgAlt", 49},
				{"TextLink", 50}, {"TextSelectedBg", 51}, {"TreeLines", 52},
				{"DragDropTarget", 53}, {"NavCursor", 54}, {"NavWindowingHighlight", 55},
				{"NavWindowingDimBg", 56}, {"ModalWindowDimBg", 57}, {"COUNT", 58},
				// renamed aliases kept for script compatibility
				{"TabActive", 36}, {"TabUnfocused", 38}, {"TabUnfocusedActive", 39},
				{"NavHighlight", 54}, {"ModalWindowDarkening", 57}
			};
			RegisterEnum(state, "ImGuiCol", col, std::size(col));

			static const EnumEntry dir[] = {
				{"COUNT", 4}, {"Down", 3}, {"Left", 0},
				{"None", -1}, {"Right", 1}, {"Up", 2}
			};
			RegisterEnum(state, "ImGuiDir", dir, std::size(dir));

			static const EnumEntry comboFlags[] = {
				{"HeightLarge", 8}, {"HeightLargest", 16}, {"HeightMask", 30},
				{"HeightRegular", 4}, {"HeightSmall", 2}, {"NoArrowButton", 32},
				{"NoPreview", 64}, {"None", 0}, {"PopupAlignLeft", 1}
			};
			RegisterEnum(state, "ImGuiComboFlags", comboFlags, std::size(comboFlags));

			static const EnumEntry inputFlags[] = {
				{"None", 0}, {"CharsDecimal", 1}, {"CharsHexadecimal", 2},
				{"CharsScientific", 4}, {"CharsUppercase", 8}, {"CharsNoBlank", 16},
				{"AllowTabInput", 32}, {"EnterReturnsTrue", 64}, {"EscapeClearsAll", 128},
				{"CtrlEnterForNewLine", 256}, {"ReadOnly", 512}, {"Password", 1024},
				{"AlwaysOverwrite", 2048}, {"AutoSelectAll", 4096}, {"ParseEmptyRefVal", 8192},
				{"DisplayEmptyRefVal", 16384}, {"NoHorizontalScroll", 32768},
				{"NoUndoRedo", 65536}, {"ElideLeft", 131072}, {"CallbackCompletion", 262144},
				{"CallbackHistory", 524288}, {"CallbackAlways", 1048576},
				{"CallbackCharFilter", 2097152}, {"CallbackResize", 4194304},
				{"CallbackEdit", 8388608}
			};
			RegisterEnum(state, "ImGuiInputTextFlags", inputFlags, std::size(inputFlags));

			static const EnumEntry colorEdit[] = {
				{"None", 0}, {"NoAlpha", 2}, {"NoPicker", 4}, {"NoOptions", 8},
				{"NoSmallPreview", 16}, {"NoInputs", 32}, {"NoTooltip", 64}, {"NoLabel", 128},
				{"NoSidePreview", 256}, {"NoDragDrop", 512}, {"NoBorder", 1024},
				{"AlphaOpaque", 2048}, {"AlphaNoBg", 4096}, {"AlphaPreviewHalf", 8192},
				{"AlphaBar", 65536}, {"HDR", 524288}, {"DisplayRGB", 1048576},
				{"DisplayHSV", 2097152}, {"DisplayHex", 4194304}, {"Uint8", 8388608},
				{"Float", 16777216}, {"PickerHueBar", 33554432}, {"PickerHueWheel", 67108864},
				{"InputRGB", 134217728}, {"InputHSV", 268435456}, {"DefaultOptions_", 177209344},
				{"DisplayMask_", 7340032}, {"DataTypeMask_", 25165824}, {"PickerMask_", 100663296},
				{"InputMask_", 402653184}
			};
			RegisterEnum(state, "ImGuiColorEditFlags", colorEdit, std::size(colorEdit));

			static const EnumEntry treeFlags[] = {
				{"None", 0}, {"Selected", 1}, {"Framed", 2}, {"AllowOverlap", 4},
				{"NoTreePushOnOpen", 8}, {"NoAutoOpenOnLog", 16}, {"DefaultOpen", 32},
				{"OpenOnDoubleClick", 64}, {"OpenOnArrow", 128}, {"Leaf", 256},
				{"Bullet", 512}, {"FramePadding", 1024}, {"SpanAvailWidth", 2048},
				{"SpanFullWidth", 4096}, {"SpanLabelWidth", 8192}, {"SpanAllColumns", 16384},
				{"LabelSpanAllColumns", 32768}, {"NavLeftJumpsToParent", 131072},
				{"CollapsingHeader", 26},
				// renamed aliases kept for script compatibility
				{"AllowItemOverlap", 4}, {"NavLeftJumpsBackHere", 131072}
			};
			RegisterEnum(state, "ImGuiTreeNodeFlags", treeFlags, std::size(treeFlags));

			static const EnumEntry selectFlags[] = {
				{"AllowDoubleClick", 4}, {"AllowItemOverlap", 16},
				{"Disabled", 8}, {"DontClosePopups", 1},
				{"None", 0}, {"SpanAllColumns", 2}
			};
			RegisterEnum(state, "ImGuiSelectableFlags", selectFlags, std::size(selectFlags));

			static const EnumEntry popupFlags[] = {
				{"None", 0}, {"MouseButtonLeft", 0}, {"MouseButtonRight", 1},
				{"MouseButtonMiddle", 2}, {"MouseButtonMask_", 31}, {"MouseButtonDefault_", 1},
				{"NoReopen", 32}, {"NoOpenOverExistingPopup", 128}, {"NoOpenOverItems", 256},
				{"AnyPopupId", 1024}, {"AnyPopupLevel", 2048}, {"AnyPopup", 3072}
			};
			RegisterEnum(state, "ImGuiPopupFlags", popupFlags, std::size(popupFlags));

			static const EnumEntry tabBar[] = {
				{"None", 0}, {"Reorderable", 1}, {"AutoSelectNewTabs", 2},
				{"TabListPopupButton", 4}, {"NoCloseWithMiddleMouseButton", 8},
				{"NoTabListScrollingButtons", 16}, {"NoTooltip", 32},
				{"DrawSelectedOverline", 64}, {"FittingPolicyResizeDown", 128},
				{"FittingPolicyScroll", 256}, {"FittingPolicyMask_", 384},
				{"FittingPolicyDefault_", 128}
			};
			RegisterEnum(state, "ImGuiTabBarFlags", tabBar, std::size(tabBar));

			static const EnumEntry tableColumn[] = {
				{"DefaultSort", 4}, {"Disabled", 1},
				{"IndentDisabled", 131072}, {"IndentEnable", 65536},
				{"IndentMask_", 196608}, {"IsEnabled", 16777216},
				{"IsHovered", 134217728}, {"IsSorted", 67108864},
				{"IsVisible", 33554432}, {"NoClip", 256},
				{"NoDirectResize_", 1073741824}, {"NoHeaderLabel", 4096},
				{"NoHeaderWidth", 8192}, {"NoHide", 128},
				{"NoReorder", 64}, {"NoResize", 32},
				{"NoSort", 512}, {"NoSortAscending", 1024},
				{"NoSortDescending", 2048}, {"None", 0},
				{"PreferSortAscending", 16384}, {"PreferSortDescending", 32768},
				{"StatusMask_", 251658240}, {"WidthFixed", 16},
				{"WidthMask_", 24}, {"WidthStretch", 8}
			};
			RegisterEnum(state, "ImGuiTableColumnFlags", tableColumn, std::size(tableColumn));

			static const EnumEntry tableFlags[] = {
				{"Borders", 1920}, {"BordersH", 384},
				{"BordersInner", 640}, {"BordersInnerH", 128},
				{"BordersInnerV", 512}, {"BordersOuter", 1280},
				{"BordersOuterH", 256}, {"BordersOuterV", 1024},
				{"BordersV", 1536}, {"ContextMenuInBody", 32},
				{"Hideable", 4}, {"NoBordersInBody", 2048},
				{"NoBordersInBodyUntilResize", 4096}, {"NoClip", 1048576},
				{"NoHostExtendX", 65536}, {"NoHostExtendY", 131072},
				{"NoKeepColumnsVisible", 262144}, {"NoPadInnerX", 8388608},
				{"NoPadOuterX", 4194304}, {"NoSavedSettings", 16},
				{"None", 0}, {"PadOuterX", 2097152},
				{"PreciseWidths", 524288}, {"Reorderable", 2},
				{"Resizable", 1}, {"RowBg", 64},
				{"ScrollX", 16777216}, {"ScrollY", 33554432},
				{"SizingFixedFit", 8192}, {"SizingFixedSame", 16384},
				{"SizingMask_", 57344}, {"SizingStretchProp", 24576},
				{"SizingStretchSame", 32768}, {"SortMulti", 67108864},
				{"SortTristate", 134217728}, {"Sortable", 8}
			};
			RegisterEnum(state, "ImGuiTableFlags", tableFlags, std::size(tableFlags));

			static const EnumEntry styleVar[] = {
				{"Alpha", 0}, {"DisabledAlpha", 1}, {"WindowPadding", 2}, {"WindowRounding", 3},
				{"WindowBorderSize", 4}, {"WindowMinSize", 5}, {"WindowTitleAlign", 6},
				{"ChildRounding", 7}, {"ChildBorderSize", 8}, {"PopupRounding", 9},
				{"PopupBorderSize", 10}, {"FramePadding", 11}, {"FrameRounding", 12},
				{"FrameBorderSize", 13}, {"ItemSpacing", 14}, {"ItemInnerSpacing", 15},
				{"IndentSpacing", 16}, {"CellPadding", 17}, {"ScrollbarSize", 18},
				{"ScrollbarRounding", 19}, {"GrabMinSize", 20}, {"GrabRounding", 21},
				{"ImageBorderSize", 22}, {"TabRounding", 23}, {"TabBorderSize", 24},
				{"TabBarBorderSize", 25}, {"TabBarOverlineSize", 26}, {"TableAngledHeadersAngle", 27},
				{"TableAngledHeadersTextAlign", 28}, {"TreeLinesSize", 29}, {"TreeLinesRounding", 30},
				{"ButtonTextAlign", 31}, {"SelectableTextAlign", 32}, {"SeparatorTextBorderSize", 33},
				{"SeparatorTextAlign", 34}, {"SeparatorTextPadding", 35}, {"COUNT", 36}
			};
			RegisterEnum(state, "ImGuiStyleVar", styleVar, std::size(styleVar));

			static const EnumEntry tabItem[] = {
				{"NoCloseWithMiddleMouseButton", 4}, {"NoPushId", 8},
				{"NoTooltip", 16}, {"None", 0},
				{"SetSelected", 2}, {"UnsavedDocument", 1}
			};
			RegisterEnum(state, "ImGuiTabItemFlags", tabItem, std::size(tabItem));
		}
	};

	ImGuiBind _ImGuiBind;
}

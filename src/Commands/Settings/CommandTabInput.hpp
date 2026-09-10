#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Config/HotkeySystem.hpp"
#include "Rendering/Theme.hpp"
#include "Menu/Click.hpp"
#include <algorithm>
#include <climits>

namespace Stand
{
	class CommandInputPresetList : public CommandList
	{
	public:
		explicit CommandInputPresetList(CommandList* const parent)
			: CommandList(parent, LIT("Presets"), CMDNAMES("inputpreset"))
		{
		}
	};

	class CommandInputScheme : public CommandList
	{
	public:
		CommandInputPresetList* const presets;

		CommandLink keyOpenClose;
		CommandLink keyPrevTab;
		CommandLink keyNextTab;
		CommandLink keyUp;
		CommandLink keyDown;
		CommandLink keyLeft;
		CommandLink keyRight;
		CommandLink keyClick;
		CommandLink keyBack;
		CommandLink keyContextMenu;
		CommandLink keyCommandBox;

		explicit CommandInputScheme(CommandList* const parent)
			: CommandList(parent, LIT("Keyboard Input Scheme"), CMDNAMES("inputscheme"))
			, presets(createChild<CommandInputPresetList>())
		{
		}
	};

	class CommandMouse : public CommandToggle
	{
	public:
		explicit CommandMouse(CommandList* const parent)
			: CommandToggle(parent, LIT("Mouse Support"), CMDNAMES("mouse"),
				LIT("Note that this does not work with the \"Windows\" input method due to it constantly centring your cursor."),
				false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kMouse = m_on;
		}
	};

	class CommandMouseCursorOnNav : public CommandToggle
	{
	public:
		explicit CommandMouseCursorOnNav(CommandList* const parent)
			: CommandToggle(parent, LIT("Set Cursor Position On Non-Mouse Navigation"), CMDNAMES(), NOLABEL, true)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kMouseCursorOnNavigation = m_on;
		}
	};

	class CommandMouseSupportList : public CommandList
	{
	public:
		CommandMouse* const mouse;
		CommandMouseCursorOnNav* const cursorOnNav;

		explicit CommandMouseSupportList(CommandList* const parent)
			: CommandList(parent, LIT("Mouse Support"), CMDNAMES())
			, mouse(createChild<CommandMouse>())
			, cursorOnNav(createChild<CommandMouseCursorOnNav>())
		{
		}
	};

	class CommandControllerSupport : public CommandToggle
	{
	public:
		explicit CommandControllerSupport(CommandList* const parent)
			: CommandToggle(parent, LIT("Controller Support"), CMDNAMES("controller"), NOLABEL, false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kControllerSupport = m_on;
		}
	};

	class CommandControllerOpenClose1 : public CommandSlider
	{
	public:
		explicit CommandControllerOpenClose1(CommandList* const parent)
			: CommandSlider(parent, LIT("Open/Close Menu 1"), CMDNAMES(), NOLABEL, 0, 15, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			static constexpr const char* k[16] = {
				"None","A","B","X","Y","LB","RB","LT","RT","L3","R3",
				"D-Pad Up","D-Pad Down","D-Pad Left","D-Pad Right","Start"
			};
			return k[std::clamp(value, 0, 15)];
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kControllerOpenClose1 = static_cast<uint8_t>(value);
		}
	};

	class CommandControllerOpenClose2 : public CommandSlider
	{
	public:
		explicit CommandControllerOpenClose2(CommandList* const parent)
			: CommandSlider(parent, LIT("Open/Close Menu 2"), CMDNAMES(), NOLABEL, 0, 15, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			static constexpr const char* k[16] = {
				"None","A","B","X","Y","LB","RB","LT","RT","L3","R3",
				"D-Pad Up","D-Pad Down","D-Pad Left","D-Pad Right","Start"
			};
			return k[std::clamp(value, 0, 15)];
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kControllerOpenClose2 = static_cast<uint8_t>(value);
		}
	};

	class CommandControllerPrevTab : public CommandSlider
	{
	public:
		explicit CommandControllerPrevTab(CommandList* const parent)
			: CommandSlider(parent, LIT("Previous Tab"), CMDNAMES(), NOLABEL, 0, 15, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			static constexpr const char* k[16] = {
				"None","A","B","X","Y","LB","RB","LT","RT","L3","R3",
				"D-Pad Up","D-Pad Down","D-Pad Left","D-Pad Right","Start"
			};
			return k[std::clamp(value, 0, 15)];
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kControllerPrevTab = static_cast<uint8_t>(value);
		}
	};

	class CommandControllerNextTab : public CommandSlider
	{
	public:
		explicit CommandControllerNextTab(CommandList* const parent)
			: CommandSlider(parent, LIT("Next Tab"), CMDNAMES(), NOLABEL, 0, 15, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			static constexpr const char* k[16] = {
				"None","A","B","X","Y","LB","RB","LT","RT","L3","R3",
				"D-Pad Up","D-Pad Down","D-Pad Left","D-Pad Right","Start"
			};
			return k[std::clamp(value, 0, 15)];
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kControllerNextTab = static_cast<uint8_t>(value);
		}
	};

	class CommandControllerUp : public CommandSlider
	{
	public:
		explicit CommandControllerUp(CommandList* const parent)
			: CommandSlider(parent, LIT("Up"), CMDNAMES(), NOLABEL, 0, 15, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			static constexpr const char* k[16] = {
				"None","A","B","X","Y","LB","RB","LT","RT","L3","R3",
				"D-Pad Up","D-Pad Down","D-Pad Left","D-Pad Right","Start"
			};
			return k[std::clamp(value, 0, 15)];
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kControllerUp = static_cast<uint8_t>(value);
		}
	};

	class CommandControllerDown : public CommandSlider
	{
	public:
		explicit CommandControllerDown(CommandList* const parent)
			: CommandSlider(parent, LIT("Down"), CMDNAMES(), NOLABEL, 0, 15, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			static constexpr const char* k[16] = {
				"None","A","B","X","Y","LB","RB","LT","RT","L3","R3",
				"D-Pad Up","D-Pad Down","D-Pad Left","D-Pad Right","Start"
			};
			return k[std::clamp(value, 0, 15)];
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kControllerDown = static_cast<uint8_t>(value);
		}
	};

	class CommandControllerLeft : public CommandSlider
	{
	public:
		explicit CommandControllerLeft(CommandList* const parent)
			: CommandSlider(parent, LIT("Left"), CMDNAMES(), NOLABEL, 0, 15, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			static constexpr const char* k[16] = {
				"None","A","B","X","Y","LB","RB","LT","RT","L3","R3",
				"D-Pad Up","D-Pad Down","D-Pad Left","D-Pad Right","Start"
			};
			return k[std::clamp(value, 0, 15)];
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kControllerLeft = static_cast<uint8_t>(value);
		}
	};

	class CommandControllerRight : public CommandSlider
	{
	public:
		explicit CommandControllerRight(CommandList* const parent)
			: CommandSlider(parent, LIT("Right"), CMDNAMES(), NOLABEL, 0, 15, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			static constexpr const char* k[16] = {
				"None","A","B","X","Y","LB","RB","LT","RT","L3","R3",
				"D-Pad Up","D-Pad Down","D-Pad Left","D-Pad Right","Start"
			};
			return k[std::clamp(value, 0, 15)];
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kControllerRight = static_cast<uint8_t>(value);
		}
	};

	class CommandControllerClick : public CommandSlider
	{
	public:
		explicit CommandControllerClick(CommandList* const parent)
			: CommandSlider(parent, LIT("Click"), CMDNAMES(), NOLABEL, 0, 15, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			static constexpr const char* k[16] = {
				"None","A","B","X","Y","LB","RB","LT","RT","L3","R3",
				"D-Pad Up","D-Pad Down","D-Pad Left","D-Pad Right","Start"
			};
			return k[std::clamp(value, 0, 15)];
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kControllerClick = static_cast<uint8_t>(value);
		}
	};

	class CommandControllerBack : public CommandSlider
	{
	public:
		explicit CommandControllerBack(CommandList* const parent)
			: CommandSlider(parent, LIT("Back"), CMDNAMES(), NOLABEL, 0, 15, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			static constexpr const char* k[16] = {
				"None","A","B","X","Y","LB","RB","LT","RT","L3","R3",
				"D-Pad Up","D-Pad Down","D-Pad Left","D-Pad Right","Start"
			};
			return k[std::clamp(value, 0, 15)];
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kControllerBack = static_cast<uint8_t>(value);
		}
	};

	class CommandControllerContextMenu : public CommandSlider
	{
	public:
		explicit CommandControllerContextMenu(CommandList* const parent)
			: CommandSlider(parent, LIT("Context Menu"), CMDNAMES(), NOLABEL, 0, 15, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			static constexpr const char* k[16] = {
				"None","A","B","X","Y","LB","RB","LT","RT","L3","R3",
				"D-Pad Up","D-Pad Down","D-Pad Left","D-Pad Right","Start"
			};
			return k[std::clamp(value, 0, 15)];
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kControllerContextMenu = static_cast<uint8_t>(value);
		}
	};

	class CommandControllerCommandBox : public CommandSlider
	{
	public:
		explicit CommandControllerCommandBox(CommandList* const parent)
			: CommandSlider(parent, LIT("Command Box"), CMDNAMES(), NOLABEL, 0, 16, 0, 1)
		{
		}

		std::string getValueText() const override
		{
			static constexpr const char* k[17] = {
				"None","A","B","X","Y","LB","RB","LT","RT","L3","R3",
				"D-Pad Up","D-Pad Down","D-Pad Left","D-Pad Right","Start","Back"
			};
			return k[std::clamp(value, 0, 16)];
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kControllerCommandBox = static_cast<uint8_t>(value);
		}
	};

	class CommandControllerScheme : public CommandList
	{
	public:
		CommandControllerOpenClose1* const openClose1;
		CommandControllerOpenClose2* const openClose2;
		CommandControllerPrevTab* const prevTab;
		CommandControllerNextTab* const nextTab;
		CommandControllerUp* const up;
		CommandControllerDown* const down;
		CommandControllerLeft* const left;
		CommandControllerRight* const right;
		CommandControllerClick* const click;
		CommandControllerBack* const back;
		CommandControllerContextMenu* const contextMenu;
		CommandControllerCommandBox* const commandBox;

		explicit CommandControllerScheme(CommandList* const parent)
			: CommandList(parent, LIT("Controller Input Scheme"), CMDNAMES())
			, openClose1(createChild<CommandControllerOpenClose1>())
			, openClose2(createChild<CommandControllerOpenClose2>())
			, prevTab(createChild<CommandControllerPrevTab>())
			, nextTab(createChild<CommandControllerNextTab>())
			, up(createChild<CommandControllerUp>())
			, down(createChild<CommandControllerDown>())
			, left(createChild<CommandControllerLeft>())
			, right(createChild<CommandControllerRight>())
			, click(createChild<CommandControllerClick>())
			, back(createChild<CommandControllerBack>())
			, contextMenu(createChild<CommandControllerContextMenu>())
			, commandBox(createChild<CommandControllerCommandBox>())
		{
		}
	};

	class CommandBackReset : public CommandToggle
	{
	public:
		explicit CommandBackReset(CommandList* const parent)
			: CommandToggle(parent, LIT("Back Resets Cursor"), CMDNAMES("backreset"),
				LIT("Puts the cursor at the top of the list when back is pressed while there's nothing to go back to."),
				false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kBackReset = m_on;
		}
	};

	class CommandBackClose : public CommandToggle
	{
	public:
		explicit CommandBackClose(CommandList* const parent)
			: CommandToggle(parent, LIT("Back Closes Menu"), CMDNAMES("backclose"),
				LIT("Allows Stand's in-game UI to be closed by pressing back when there's nothing to go back to."),
				false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kBackClose = m_on;
		}
	};

	class CommandInterruptHoldWrap : public CommandToggle
	{
	public:
		explicit CommandInterruptHoldWrap(CommandList* const parent)
			: CommandToggle(parent, LIT("Interrupt Holding On Wrap-Around"), CMDNAMES(), NOLABEL, true)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kInterruptHoldWrapAround = m_on;
		}
	};

	class CommandTabsKeepCursor : public CommandToggle
	{
	public:
		explicit CommandTabsKeepCursor(CommandList* const parent)
			: CommandToggle(parent, LIT("Tabs"), CMDNAMES("tabskeepcursor"),
				LIT("Will make tabs like \"Self\" remember the cursor position you had when you last visited them."),
				true)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kTabsKeepCursor = m_on;
		}
	};

	class CommandListsKeepCursor : public CommandToggle
	{
	public:
		explicit CommandListsKeepCursor(CommandList* const parent)
			: CommandToggle(parent, LIT("Lists"), CMDNAMES("listskeepcursor"),
				LIT("Will make lists like \"Settings\" remember the cursor position you had when you last visited them."),
				true)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kListsKeepCursor = m_on;
		}
	};

	class CommandListsKeepCursorReducedHuge : public CommandToggle
	{
	public:
		explicit CommandListsKeepCursorReducedHuge(CommandList* const parent)
			: CommandToggle(parent, LIT("Lists: Reduced Effect For Huge Lists"), CMDNAMES(), NOLABEL, true)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kListsKeepCursorReducedHuge = m_on;
		}
	};

	class CommandKeepCursorWhenRevisiting : public CommandList
	{
	public:
		CommandTabsKeepCursor* const tabs;
		CommandListsKeepCursor* const lists;
		CommandListsKeepCursorReducedHuge* const reducedHuge;

		explicit CommandKeepCursorWhenRevisiting(CommandList* const parent)
			: CommandList(parent, LIT("Keep Cursor When Revisiting"), CMDNAMES())
			, tabs(createChild<CommandTabsKeepCursor>())
			, lists(createChild<CommandListsKeepCursor>())
			, reducedHuge(createChild<CommandListsKeepCursorReducedHuge>())
		{
		}
	};

	class CommandKeyRepeatInterval : public CommandSlider
	{
	public:
		explicit CommandKeyRepeatInterval(CommandList* const parent)
			: CommandSlider(parent, LIT("Key Repeat Interval"), CMDNAMES("keyrepeatinterval"),
				LIT("The interval at which your keyboard input is repeated while you're holding a key down."),
				0, 10000, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kKeyRepeatInterval = value;
		}
	};

	class CommandKeyLongRepeatInterval : public CommandSlider
	{
	public:
		explicit CommandKeyLongRepeatInterval(CommandList* const parent)
			: CommandSlider(parent, LIT("Key Long-Hold Repeat Interval"), CMDNAMES("keylongrepeatinterval"),
				LIT("The interval at which your keyboard input is repeated when you've held a key down for a while."),
				0, 10000, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kKeyLongRepeatInterval = value;
		}
	};

	class CommandTabRepeatInterval : public CommandSlider
	{
	public:
		explicit CommandTabRepeatInterval(CommandList* const parent)
			: CommandSlider(parent, LIT("Key Repeat Interval For Tabs"), CMDNAMES("tabrepeatinterval"),
				NOLABEL, 0, 10000, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kTabRepeatInterval = value;
		}
	};

	class CommandDisableHotkeys : public CommandToggle
	{
	public:
		explicit CommandDisableHotkeys(CommandList* const parent)
			: CommandToggle(parent, LIT("Disable Hotkeys"), CMDNAMES("disablehotkeys"), NOLABEL, false)
		{
		}

		void onChange(Click& click) override
		{
			Rendering::Theme::kDisableHotkeys = m_on;
		}
	};

	class CommandModifierKeyMatching : public CommandSlider
	{
	public:
		explicit CommandModifierKeyMatching(CommandList* const parent)
			: CommandSlider(parent, LIT("Modifier Key Matching"), CMDNAMES(),
				LIT("How ctrl, shift, & alt are handled."), 0, 2, 1, 1)
		{
		}

		std::string getValueText() const override
		{
			static constexpr const char* k[3] = {"Strict", "Smart", "Lax"};
			return k[std::clamp(value, 0, 2)];
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kModifierKeyMatching = static_cast<Rendering::Theme::ModifierKeyMatching>(value);
		}
	};

	class CommandTabInput : public CommandList
	{
	public:
		CommandInputScheme* const inputScheme;

		CommandLink ctxSaveState;
		CommandLink ctxLoadState;
		CommandLink ctxApplyDefault;
		CommandLink ctxApplyDefaultChildren;
		CommandLink ctxSetMin;
		CommandLink ctxSetMax;

		CommandMouseSupportList* const mouseSupport;
		CommandControllerSupport* const controllerSupport;
		CommandControllerScheme* const controllerScheme;
		CommandBackReset* const backReset;
		CommandBackClose* const backClose;
		CommandInterruptHoldWrap* const interruptHoldWrap;
		CommandKeepCursorWhenRevisiting* const keepCursor;
		CommandKeyRepeatInterval* const keyRepeatInterval;
		CommandKeyLongRepeatInterval* const keyLongRepeatInterval;
		CommandTabRepeatInterval* const tabRepeatInterval;
		CommandDisableHotkeys* const disableHotkeys;
		CommandModifierKeyMatching* const modifierKeyMatching;

		explicit CommandTabInput()
			: CommandList(nullptr, LIT("Input"), CMDNAMES())
			, inputScheme(createChild<CommandInputScheme>())
			, mouseSupport(createChild<CommandMouseSupportList>())
			, controllerSupport(createChild<CommandControllerSupport>())
			, controllerScheme(createChild<CommandControllerScheme>())
			, backReset(createChild<CommandBackReset>())
			, backClose(createChild<CommandBackClose>())
			, interruptHoldWrap(createChild<CommandInterruptHoldWrap>())
			, keepCursor(createChild<CommandKeepCursorWhenRevisiting>())
			, keyRepeatInterval(createChild<CommandKeyRepeatInterval>())
			, keyLongRepeatInterval(createChild<CommandKeyLongRepeatInterval>())
			, tabRepeatInterval(createChild<CommandTabRepeatInterval>())
			, disableHotkeys(createChild<CommandDisableHotkeys>())
			, modifierKeyMatching(createChild<CommandModifierKeyMatching>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabInput& GetCommandTabInput();
}

#pragma once
#include "Menu/UIItem.hpp"
#include "Util/Joaat.hpp"

namespace Stand
{
	class CommandToggleLegacy;
	class PlayerCommand;
	class CommandListSelect;
	class CommandSliderLegacy;
	class CommandSliderFloatLegacy;
	class CommandPosition3d;
	class CommandLegacy;
	class CommandColourCustom;
	class CommandInput;

	class Button : public UIItem
	{
	public:
		explicit Button(const std::string_view& Name, std::function<void()> OnClick, ImVec2 size = {0, 25}, const std::string_view& information = "Empty");
		void Draw() override;

	private:
		std::string_view m_Name;
		std::string_view m_Information;
		std::function<void()> m_OnClick;
		ImVec2 m_Size;
	};

	class CommandItem : public UIItem
	{
	public:
		explicit CommandItem(joaat_t id, std::optional<std::string> label_override = std::nullopt);
		void Draw() override;

	private:
		CommandLegacy* m_Command;
		std::optional<std::string> m_LabelOverride;
	};

	class PlayerCommandItem : public UIItem
	{
	public:
		explicit PlayerCommandItem(joaat_t id, std::optional<std::string> label_override = std::nullopt);
		void Draw() override;

	private:
		PlayerCommand* m_Command;
		std::optional<std::string> m_LabelOverride;
	};

	class BoolCommandItem : public UIItem
	{
	public:
		explicit BoolCommandItem(joaat_t id, std::optional<std::string> label_override = std::nullopt);
		void Draw() override;

	private:
		CommandToggleLegacy* m_Command;
		std::optional<std::string> m_LabelOverride;
	};

	class IntCommandItem : public UIItem
	{
	public:
		explicit IntCommandItem(joaat_t id, std::optional<std::string> label_override = std::nullopt, bool use_slider = true);
		void Draw() override;

	private:
		bool m_useSlider;
		CommandSliderLegacy* m_Command;
		std::optional<std::string> m_LabelOverride;
	};

	class FloatCommandItem : public UIItem
	{
	public:
		explicit FloatCommandItem(joaat_t id, std::optional<std::string> label_override = std::nullopt, bool use_slider = true);
		void Draw() override;

	private:
		bool m_useSlider;
		CommandSliderFloatLegacy* m_Command;
		std::optional<std::string> m_LabelOverride;
	};

	class Vector3CommandItem : public UIItem
	{
	public:
		explicit Vector3CommandItem(joaat_t id, std::optional<std::string> label_override = std::nullopt);
		void Draw() override;

	private:
		CommandPosition3d* m_Command;
		std::optional<std::string> m_LabelOverride;
	};

	class ListCommandItem : public UIItem
	{
	public:
		explicit ListCommandItem(joaat_t id, std::optional<std::string> label_override = std::nullopt);
		void Draw() override;

	private:
		CommandListSelect* m_Command;
		std::optional<std::string> m_LabelOverride;
	};

	class ConditionalItem : public UIItem
	{
	public:
		explicit ConditionalItem(joaat_t bool_cmd_id, std::shared_ptr<UIItem> to_draw, bool negate = false);
		explicit ConditionalItem(std::function<bool()> condition_fn, std::shared_ptr<UIItem> to_draw, bool negate = false);
		void Draw() override;
		bool CanDraw() override;

	private:
		CommandToggleLegacy* m_Condition;
		std::function<bool()> m_ConditionFn;
		std::shared_ptr<UIItem> m_Item;
		bool m_Negate;
	};

	class ImGuiItem : public UIItem
	{
	public:
		explicit ImGuiItem(std::function<void()> callback);
		void Draw() override;

	private:
		std::function<void()> m_Callback;
	};

	class Group : public UIItem
	{
	public:
		explicit Group(const std::string& name, int items_per_row = 7);
		void Draw() override;

		void AddItem(std::shared_ptr<UIItem>&& item)
		{
			m_Items.push_back(std::move(item));
		}

		void RemoveItem(const std::shared_ptr<UIItem>& item)
		{
			std::erase(m_Items, item);
		}

		const std::string& GetName() const
		{
			return m_Name;
		}

	private:
		std::string m_Name;
		int m_ItemsPerColumn;
		std::vector<std::shared_ptr<UIItem>> m_Items;
	};

	class InputTextWithHint : public UIItem
	{
	public:
		explicit InputTextWithHint(std::string label, std::string hint, std::string* buf, std::function<void()> cb = nullptr);
		void Draw() override;

	private:
		std::string m_Id;
		std::string m_Hint;
		std::string* m_Buf;
		std::function<void()> m_Callback;
	};

	class ColorCommandItem : public UIItem
	{
	public:
		explicit ColorCommandItem(joaat_t id, std::optional<std::string> label_override = std::nullopt);
		void Draw() override;

	private:
		CommandColourCustom* m_Command;
		std::optional<std::string> m_LabelOverride;
	};

	class StringCommandItem : public UIItem
	{
	public:
		explicit StringCommandItem(joaat_t id, std::optional<std::string> label_override = std::nullopt);
		void Draw() override;

	private:
		CommandInput* m_Command;
		std::optional<std::string> m_LabelOverride;
	};

	class TabItem : public UIItem
	{
	public:
		explicit TabItem(const std::string& name);
		void Draw() override;

		void AddItem(std::shared_ptr<UIItem>&& item)
		{
			m_Items.push_back(std::move(item));
		}

		// Needed by TabBarItem to draw this tab's own header button - see
		// TabBarItem.cpp's own class comment for why it now owns the
		// active/inactive switch that used to be real ImGui tab state.
		const std::string& GetName() const
		{
			return m_Name;
		}

	private:
		std::string m_Name;
		std::vector<std::shared_ptr<UIItem>> m_Items;
	};

	class TabBarItem : public UIItem
	{
	public:
		explicit TabBarItem(const std::string& id);
		void Draw() override;

		void AddItem(std::shared_ptr<TabItem>&& tab)
		{
			m_Tabs.push_back(std::move(tab));
		}

	private:
		std::string m_Id;
		std::vector<std::shared_ptr<TabItem>> m_Tabs;

		// Real ImGui tab bars own which tab is selected internally - this
		// tree has no such state of its own any more, so TabBarItem keeps
		// it here instead (see TabBarItem.cpp's own class comment).
		int m_ActiveIndex = 0;
	};

	class CollapsingHeaderItem : public UIItem
	{
	public:
		explicit CollapsingHeaderItem(const std::string& name);
		void Draw() override;

		void AddItem(std::shared_ptr<UIItem>&& item)
		{
			m_Items.push_back(std::move(item));
		}

	private:
		std::string m_Name;
		std::vector<std::shared_ptr<UIItem>> m_Items;

		// Real ImGui CollapsingHeader owns its own expanded/collapsed
		// state internally, keyed off m_Name - this tree has no such
		// state of its own any more, so this keeps it here instead.
		bool m_Expanded = false;
	};
}
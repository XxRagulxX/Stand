#include "Rendering/GridListSelectOptions.hpp"

#include "Commands/Widgets/CommandListAction.hpp"
#include "Commands/Widgets/CommandListActionItem.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Menu/Click.hpp"
#include "Rendering/GridItemListGrid.hpp"
#include "Rendering/MenuNavigation.hpp"

#include <memory>
#include <unordered_map>

namespace Stand
{
	class CommandListSelectProxy : public CommandListAction
	{
	public:
		CommandListSelect* const m_Select;

		explicit CommandListSelectProxy(CommandListSelect* sel)
			: CommandListAction(nullptr, Label(sel->menu_name), {}, NOLABEL,
			                    std::vector<CommandListActionItemData>(sel->options))
			, m_Select(sel)
		{
		}

		void onItemClick(Click& click, CommandListActionItem* item) override
		{
			m_Select->setValue(click, item->value);
			Rendering::MenuNavigation::Pop();
		}
	};
}

namespace Stand::Rendering
{
	static std::unordered_map<Stand::CommandListSelect*, std::unique_ptr<Stand::CommandListSelectProxy>> s_Proxies;

	static Stand::CommandListSelectProxy& GetOrCreateProxy(Stand::CommandListSelect* sel)
	{
		auto it = s_Proxies.find(sel);
		if (it == s_Proxies.end())
			it = s_Proxies.emplace(sel, std::make_unique<Stand::CommandListSelectProxy>(sel)).first;
		return *it->second;
	}

	void OpenListSelectOptions(Stand::CommandListSelect* sel)
	{
		auto& proxy = GetOrCreateProxy(sel);

		cursor_t targetCursor = 0;
		for (cursor_t i = 0; i < static_cast<cursor_t>(proxy.children.size()); ++i)
		{
			if (proxy.children[i]->as<Stand::CommandListActionItem>()->value == sel->value)
			{
				targetCursor = i;
				break;
			}
		}
		proxy.m_cursor = targetCursor;
		proxy.m_offset = 0;

		MenuNavigation::Push(sel->menu_name.getLocalisedUtf8(), &GridItemListGrid::GetOrCreate(&proxy));
	}
}

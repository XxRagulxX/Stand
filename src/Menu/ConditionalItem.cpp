#include "Menu/Items.hpp"
#include "Commands/Commands.hpp"
#include "Commands/CommandToggleLegacy.hpp"

namespace Stand
{
	ConditionalItem::ConditionalItem(joaat_t bool_cmd_id, std::shared_ptr<UIItem> to_draw, bool negate) :
		m_Condition(Commands::GetCommand<CommandToggleLegacy>(bool_cmd_id)),
		m_ConditionFn(nullptr),
		m_Item(to_draw),
		m_Negate(negate)
	{
	}

	ConditionalItem::ConditionalItem(std::function<bool()> condition_fn, std::shared_ptr<UIItem> to_draw, bool negate) :
		m_Condition(nullptr),
		m_ConditionFn(condition_fn),
		m_Item(to_draw),
		m_Negate(negate)
	{
	}

	void ConditionalItem::Draw()
	{
		if (CanDraw())
			m_Item->Draw();
	}

	bool ConditionalItem::CanDraw()
	{
		if (m_Condition)
			return m_Condition->GetState() ^ m_Negate;
		else if (m_ConditionFn)
			return m_ConditionFn() ^ m_Negate;

		return false;
	}
}
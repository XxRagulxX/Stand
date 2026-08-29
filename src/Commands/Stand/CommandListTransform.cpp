#include "Commands/Stand/CommandListTransform.hpp"

#include "Commands/Widgets/CommandDivider.hpp"
#include "Commands/Extra/CommandFindModel.hpp"
#include "Commands/Extra/CommandTransformModel.hpp"
#include "Ped/PedModel.hpp"
#include "Commands/Stand/CommandListTransform.hpp"

namespace Stand
{
	CommandListTransform::CommandListTransform(CommandList* const parent)
		: CommandList(parent, LOC("CHNGMDL"))
	{
		this->createChild<CommandFindModel>();
		hash_t category_name = 0;
		CommandList* category_list;
		for (const auto& model : PedModel::all)
		{
			if (category_name != model.category)
			{
				category_name = model.category;
				category_list = this->createChild<CommandList>(LOC_RT(category_name));
			}
			category_list->createChild<CommandTransformModel>(&model);
		}
	}
}

#include "Commands/Context/CommandQuickCtx.hpp"

#include "Commands/Context/CommandQuickCtxSave.hpp"
#include "Commands/Context/CommandQuickCtxLoad.hpp"
#include "Commands/Context/CommandQuickCtxDefault.hpp"
#include "Commands/Context/CommandQuickCtxRDefault.hpp"
#include "Commands/Context/CommandQuickCtxMin.hpp"
#include "Commands/Context/CommandQuickCtxMax.hpp"

namespace Stand
{
	CommandQuickCtx::CommandQuickCtx(CommandList* const parent)
		: CommandList(parent, LOC("QWQCTX"))
	{
		instance = this;

		save_state = this->createChild<CommandQuickCtxSave>();
		load_state = this->createChild<CommandQuickCtxLoad>();
		apply_default_state = this->createChild<CommandQuickCtxDefault>();
		apply_default_state_to_children = this->createChild<CommandQuickCtxRDefault>();
		min = this->createChild<CommandQuickCtxMin>();
		max = this->createChild<CommandQuickCtxMax>();
	}

	CommandQuickCtx::~CommandQuickCtx()
	{
		if (instance == this)
		{
			instance = nullptr;
		}
	}
}

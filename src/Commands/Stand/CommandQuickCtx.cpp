#include "Commands/Stand/CommandQuickCtx.hpp"

#include "Commands/Stand/CommandQuickCtxSave.hpp"
#include "Commands/Stand/CommandQuickCtxLoad.hpp"
#include "Commands/Stand/CommandQuickCtxDefault.hpp"
#include "Commands/Stand/CommandQuickCtxRDefault.hpp"
#include "Commands/Stand/CommandQuickCtxMin.hpp"
#include "Commands/Stand/CommandQuickCtxMax.hpp"

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

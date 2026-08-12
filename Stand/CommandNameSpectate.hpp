#pragma once

#include "CommandTextsliderJoin.hpp"

#include "JoinUtil.hpp"
#include "ScAccount.hpp"

namespace Stand
{
	class CommandNameSpectate : public CommandTextsliderJoin
	{
	public:
		explicit CommandNameSpectate(CommandList* const parent)
			: CommandTextsliderJoin(parent, LOC("SPEC_N"), CMDNAMES_OBF("namespectate"))
		{
		}

		std::wstring getCommandSyntax() const final
		{
			return std::move(LANG_GET_W("CMD").append(L": ").append(cmdNameToUtf16(command_names.at(0))).append(L" ").append(LANG_GET_W("ARGNME")));
		}

		void onClick(Click& click) final
		{
			if (click.isBasicEdition())
			{
				CommandTextsliderJoin::onClick(click);
			}
		}

		void onCommand(Click& click, std::wstring& args) final
		{
			auto name = StringUtils::utf16_to_utf8(args);
			args.clear();
			if (click.isBasicEdition() && ScAccount::name2rid(click, std::move(name), [this](const ScAccount& a)
			{
				JoinUtil::spectate(a.rid, value);
			}))
			{
				return onClick(click);
			}
		}
	};
}

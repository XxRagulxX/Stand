#pragma once
#include "Commands/Widgets/CommandSlider.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	class CommandExplosionType final : public CommandSlider
	{
	public:
		struct Entry { int type_int; const char* name; };
		static const Entry kTypes[];
		static const int   kCount;
		static const int   kDefaultIndex;

		explicit CommandExplosionType(CommandList* parent)
			: CommandSlider(parent, LIT("Explosion Type"), CMDNAMES("selectedexplosion"),
			                NOLABEL, 0, kCount - 1, kDefaultIndex, 1, 0)
		{
		}

		[[nodiscard]] std::string getValueText() const override
		{
			return kTypes[value].name;
		}

		[[nodiscard]] int getTypeInt() const
		{
			return kTypes[value].type_int;
		}
	};
}

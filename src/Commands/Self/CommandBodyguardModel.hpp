#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	class CommandBodyguardModelEntry : public CommandPhysical
	{
		Hash* m_target;
		Hash m_hash;
	public:
		CommandBodyguardModelEntry(CommandList* parent, Label&& label, Hash hash, Hash* target)
			: CommandPhysical(COMMAND_ACTION, parent, std::move(label), {}, NOLABEL)
			, m_target(target), m_hash(hash)
		{}
		void onClick(Click&) override { *m_target = m_hash; }
	};

	class CommandBGModelPlayable : public CommandList
	{
	public:
		CommandBGModelPlayable(CommandList* parent, Hash* target);
	};

	class CommandBGModelGroundAnimals : public CommandList
	{
	public:
		CommandBGModelGroundAnimals(CommandList* parent, Hash* target);
	};

	class CommandBGModelWaterAnimals : public CommandList
	{
	public:
		CommandBGModelWaterAnimals(CommandList* parent, Hash* target);
	};

	class CommandBGModelFlyingAnimals : public CommandList
	{
	public:
		CommandBGModelFlyingAnimals(CommandList* parent, Hash* target);
	};

	class CommandBGModelNPCs : public CommandList
	{
	public:
		CommandBGModelNPCs(CommandList* parent, Hash* target);
	};

	class CommandBodyguardModel : public CommandList
	{
		Hash m_model;
	public:
		explicit CommandBodyguardModel(CommandList* parent);
		[[nodiscard]] Hash GetModel() const { return m_model; }
		void SetModel(Hash h) { m_model = h; }
	};
}

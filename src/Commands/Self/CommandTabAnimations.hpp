#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/Self/CommandAnimation.hpp"
#include "Commands/Self/CommandAutoCancelAnim.hpp"
#include "Commands/Self/CommandListAnimations.hpp"
#include "Commands/Self/CommandScenarios.hpp"
#include "Menu/Click.hpp"
#include "Scripting/FiberPool.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	class CommandCancelAnim final : public CommandPhysical
	{
	public:
		explicit CommandCancelAnim(CommandList* parent)
			: CommandPhysical(COMMAND_ACTION, parent,
				LIT("Cancel"), CMDNAMES("cancelanim"),
				NOLABEL, CMDFLAGS_ACTION)
		{
		}

		void onClick(Click& click) override
		{
			FiberPool::queueJob([] { Animations::reset(); });
		}
	};

	class CommandPartialMovement final : public CommandToggle
	{
	public:
		explicit CommandPartialMovement(CommandList* parent)
			: CommandToggle(parent, LIT("Partial Movement"), CMDNAMES("partialmoveanim"),
				NOLABEL, false)
		{
		}

		void onEnable(Click& click) override  { Animations::partial_movement = true; }
		void onDisable(Click& click) override { Animations::partial_movement = false; }
		void applyDefaultState() override
		{
			m_on = false;
			Animations::partial_movement = false;
		}
	};

	class CommandLoopedAnim final : public CommandToggle
	{
	public:
		explicit CommandLoopedAnim(CommandList* parent)
			: CommandToggle(parent, LIT("Looped Animations"), CMDNAMES("loopanim"),
				NOLABEL, true)
		{
		}

		void onEnable(Click& click) override  { Animations::looped = true; }
		void onDisable(Click& click) override { Animations::looped = false; }
		void applyDefaultState() override
		{
			m_on = true;
			Animations::looped = true;
		}
	};

	class CommandTabAnimations final : public CommandList
	{
	public:
		explicit CommandTabAnimations(CommandList* parent)
			: CommandList(parent, LIT("Animations"), CMDNAMES("animlist"))
		{
			createChild<CommandCancelAnim>();
			createChild<CommandScenarios>();
			createChild<CommandListAnimations>();

			auto* objects = createChild<CommandList>(
				LIT("Object Animations"), CMDNAMES("objectanimations"));
			for (const auto& a : Animations::object_animations)
				objects->createChild<CommandAnimation<ComplexObjectAnimation>>(a);

			createChild<CommandPartialMovement>();
			createChild<CommandLoopedAnim>();
			createChild<CommandAutoCancelAnim>();
		}
	};
}
